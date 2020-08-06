// <copyright file="WindowsDetourProcess.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "EventListener.h"

namespace Monitor
{
	/// <summary>
	/// The Pipe Server Instance that keeps track of the required state for each individual connection
	/// to a client.
	/// </summary>
	struct ServerPipeInstance
	{
		OVERLAPPED Overlap;
		Opal::System::SmartHandle EventHandle;
		Opal::System::SmartHandle PipeHandle;
		DetourMessage Message;
		bool IsConnected;
		bool HasPendingIO;
	};

	/// <summary>
	/// A windows platform specific detour process executable
	/// </summary>
	export class WindowsDetourProcess : public Opal::System::IProcess
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='WindowsDetourProcess'/> class.
		/// </summary>
		WindowsDetourProcess(
			const Path& executable,
			const std::string& arguments,
			const Path& workingDirectory,
			std::shared_ptr<IDetourCallback> callback) :
			m_executable(executable),
			m_arguments(arguments),
			m_workingDirectory(workingDirectory),
			m_eventListener(std::move(callback)),
			m_pipes(),
			m_rawEventHandles(),
			m_workerThread(),
			m_workerFailed(false),
			m_threadHandle(),
			m_processHandle(),
			m_stdOutReadHandle(),
			m_stdOutWriteHandle(),
			m_stdErrReadHandle(),
			m_stdErrWriteHandle(),
			m_stdInReadHandle(),
			m_stdInWriteHandle(),
			m_isFinished(false),
			m_stdOut(),
			m_stdErr(),
			m_exitCode(-1)
		{
		}

		/// <summary>
		/// Execute a process for the provided
		/// </summary>
		void Start() override final
		{
			DebugTrace("Start");
			std::stringstream argumentsValue;
			argumentsValue << "\"" << m_executable.ToAlternateString() << "\"" << " " << m_arguments;
			std::string argumentsString = argumentsValue.str();

			// Setup the input/output streams
			// TODO: We need to read from the buffer to ensure it doesn't deadlock on the wait forever
			int pipeBufferSize = 5 * 1024 * 1024;

			// Set the bInheritHandle flag so pipe handles are inherited.
			SECURITY_ATTRIBUTES securityAttributes;
			securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
			securityAttributes.bInheritHandle = true;
			securityAttributes.lpSecurityDescriptor = nullptr;

			// Create a pipe for the child process's STDOUT.
			HANDLE childStdOutRead;
			HANDLE childStdOutWrite;
			if (!CreatePipe(&childStdOutRead, &childStdOutWrite, &securityAttributes, pipeBufferSize))
				throw std::runtime_error("Execute CreatePipe Failed");
			m_stdOutReadHandle = Opal::System::SmartHandle(childStdOutRead);
			m_stdOutWriteHandle = Opal::System::SmartHandle(childStdOutWrite);

			// Ensure the read handle to the pipe for STDOUT is not inherited.
			if (!SetHandleInformation(m_stdOutReadHandle.Get(), HANDLE_FLAG_INHERIT, 0))
				throw std::runtime_error("Execute SetHandleInformation Failed");

			// Create a pipe for the child process's STDERR.
			HANDLE childStdErrRead;
			HANDLE childStdErrWrite;
			if (!CreatePipe(&childStdErrRead, &childStdErrWrite, &securityAttributes, pipeBufferSize))
				throw std::runtime_error("Execute CreatePipe Failed");
			m_stdErrReadHandle = Opal::System::SmartHandle(childStdErrRead);
			m_stdErrWriteHandle = Opal::System::SmartHandle(childStdErrWrite);

			// Ensure the read handle to the pipe for STDERR is not inherited.
			if (!SetHandleInformation(m_stdErrReadHandle.Get(), HANDLE_FLAG_INHERIT, 0))
				throw std::runtime_error("Execute SetHandleInformation Failed");

			// Create a pipe for the child process's STDIN.
			HANDLE childStdInRead;
			HANDLE childStdInWrite;
			if (!CreatePipe(&childStdInRead, &childStdInWrite, &securityAttributes, 0))
				throw std::runtime_error("Execute CreatePipe Failed");
			m_stdInReadHandle = Opal::System::SmartHandle(childStdInRead);
			m_stdInWriteHandle = Opal::System::SmartHandle(childStdInWrite);

			// Ensure the write handle to the pipe for STDIN is not inherited.
			if (!SetHandleInformation(m_stdInWriteHandle.Get(), HANDLE_FLAG_INHERIT, 0))
				throw std::runtime_error("Execute SetHandleInformation Failed");

			// Initialize the pipes so they are ready for the process and the worker thread
			InitializePipes();

			// Create the worker thread that will act as the pipe server
			m_processRunning = true;
			m_workerFailed = false;
			m_workerThread = std::thread(&WindowsDetourProcess::WorkerThread, std::ref(*this));

			// Build up the detour dlls absolute path
			auto moduleName = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto moduleFolder = moduleName.GetParent();
			auto dllPath = moduleFolder + Path("Monitor.Detours.64.dll");
			auto dllPathString = dllPath.ToAlternateString();

			// Setup the process creation parameters
			LPSECURITY_ATTRIBUTES processAttributes = nullptr;
			LPSECURITY_ATTRIBUTES threadAttributes = nullptr;
			bool inheritHandles = true;
			DWORD creationFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;
			void* environment = nullptr;

			STARTUPINFOA startupInfo = {};
			ZeroMemory(&startupInfo, sizeof(STARTUPINFOA));
			startupInfo.cb = sizeof(startupInfo);
			startupInfo.hStdError = m_stdErrWriteHandle.Get();
			startupInfo.hStdOutput = m_stdOutWriteHandle.Get();
			startupInfo.hStdInput = m_stdInReadHandle.Get();
			startupInfo.dwFlags |= STARTF_USESTDHANDLES;

			PROCESS_INFORMATION processInfo = {};
			ZeroMemory(&processInfo, sizeof(PROCESS_INFORMATION));

			// Create the requested process with our detour dll loaded
			if (!DetourCreateProcessWithDllExA(
				m_executable.ToString().c_str(),
				argumentsString.data(),
				processAttributes,
				threadAttributes,
				inheritHandles,
				creationFlags,
				environment,
				m_workingDirectory.ToString().c_str(),
				&startupInfo,
				&processInfo,
				dllPathString.c_str(),
				nullptr))
			{
				auto error = GetLastError();
				switch (error)
				{
				case ERROR_FILE_NOT_FOUND:
					throw std::runtime_error("Execute DetourCreateProcessWithDllExA the requested executable does not exist");
				default:
					throw std::runtime_error("Execute DetourCreateProcessWithDllExA Failed: " + std::to_string(error));
				}
			}

			// Store the runtime handles
			m_processHandle = Opal::System::SmartHandle(processInfo.hProcess);
			m_threadHandle = Opal::System::SmartHandle(processInfo.hThread);

			// Set the detoured process payload
			DetourPayload payload;
			ZeroMemory(&payload, sizeof(payload));
			payload.nParentProcessId = GetCurrentProcessId();
			payload.nTraceProcessId = GetCurrentProcessId();
			payload.nGeneology = 1;
			payload.rGeneology[0] = 0;
			payload.wzParents[0] = 0;
			if (!DetourCopyPayloadToProcess(
				m_processHandle.Get(),
				GuidTrace,
				&payload,
				sizeof(payload)))
			{
				throw std::runtime_error("DetourCopyPayloadToProcess failed: " + std::to_string(GetLastError()));
			}

			// Restart the process
			auto hr = ResumeThread(m_threadHandle.Get());
		}

		/// <summary>
		/// Wait for the process to exit
		/// </summary>
		void WaitForExit() override final
		{
			// Wait until child process exits.
			DebugTrace("WaitForExit");
			auto waitResult = WaitForSingleObject(m_processHandle.Get(), INFINITE);
			DebugTrace("WaitForExit Signal");
			m_processRunning = false;
			switch (waitResult)
			{
				case WAIT_OBJECT_0:
					// All good
					break;
				case WAIT_ABANDONED:
					throw std::runtime_error("Execute WaitForSingleObject Abandoned");
					break;
				case WAIT_TIMEOUT:
					throw std::runtime_error("Execute WaitForSingleObject Timeout");
					break;
				case WAIT_FAILED:
					throw std::runtime_error("Execute WaitForSingleObject Failed: " + std::to_string(GetLastError()));
					break;
				default:
					throw std::runtime_error("Execute WaitForSingleObject Failed Unknown");
			}

			// Get the exit code
			DWORD exitCode;
			if (!GetExitCodeProcess(m_processHandle.Get(), &exitCode))
			{
				auto error = GetLastError();
				throw std::runtime_error("Execute GetExitCodeProcess Failed: " + std::to_string(error));
			}
			m_exitCode = exitCode;

			// Close the child write handle to ensure we stop reading
			m_stdOutWriteHandle.Close();
			m_stdErrWriteHandle.Close();

			// Read all and write to stdout
			// TODO: May want to switch over to a background thread with peak to read in order
			DWORD dwRead = -1;
			const int BufferSize = 256;
			char buffer[BufferSize + 1];

			// Read on output
			while (true)
			{
				if(!ReadFile(m_stdOutReadHandle.Get(), buffer, BufferSize, &dwRead, nullptr))
					break;
				if (dwRead == 0)
					break;

				m_stdOut << std::string_view(buffer, dwRead);
			}

			// Read all errors
			while (true)
			{
				if(!ReadFile(m_stdErrReadHandle.Get(), buffer, BufferSize, &dwRead, nullptr))
					break;
				if (dwRead == 0)
					break;

				// Make the string null terminated
				m_stdErr << std::string_view(buffer, dwRead);
			}

			// Wait for the worker thread to exit
			m_workerThread.join();

			if (m_workerFailed)
			{
				std::rethrow_exception(m_workerException);
			}

			m_isFinished = true;
		}

		/// <summary>
		/// Get the exit code
		/// </summary>
		int GetExitCode() override final
		{
			if (!m_isFinished)
				throw std::runtime_error("Process has not finished.");
			return m_exitCode;
		}

		/// <summary>
		/// Get the standard output
		/// </summary>
		std::string GetStandardOutput() override final
		{
			if (!m_isFinished)
				throw std::runtime_error("Process has not finished.");
			return m_stdOut.str();
		}

		/// <summary>
		/// Get the standard error output
		/// </summary>
		std::string GetStandardError() override final
		{
			if (!m_isFinished)
				throw std::runtime_error("Process has not finished.");
			return m_stdErr.str();
		}

	private:
		/// <summary>
		/// The main entry point for the worker thread that will monitor incoming messages from all
		/// client connections.
		/// </summary>
		void WorkerThread()
		{
			try
			{
				DebugTrace("WorkerThread Start");

				// Read until we get a client and then all clients disconnect
				m_hasAnyClients = false;
				m_activeClientCount = 0;
				while ((!m_hasAnyClients || m_activeClientCount > 0) && m_processRunning)
				{
					// Wait for any of the pipe instances to signal
					// This indicates that either a client connected to wrote to
					// and open connection.
					// Check every 5 seconds if the process has terminated unexpectedly
					bool waitForAll = false;
					DWORD timoutMilliseconds = 5000;
					DebugTrace("WorkerThread WaitForMultipleObjects");
					auto waitResult = WaitForMultipleObjects(
						m_rawEventHandles.size(),
						m_rawEventHandles.data(),
						waitForAll,
						timoutMilliseconds);
					switch (waitResult)
					{
						case WAIT_TIMEOUT:
							if (!m_processRunning)
							{
								DebugTrace("Main process exited while children still running");
								continue;
							}
							else
							{
								// The process hasn't done anything for awhile.
								// Continue waiting...
								continue;
							}

						case WAIT_FAILED:
							throw std::runtime_error("WaitForMultipleObjects failed: " +  std::to_string(GetLastError()));
					}

					// Determine which pipe completed the operation.
					auto pipeIndex = waitResult - WAIT_OBJECT_0;
					if (pipeIndex < 0 || pipeIndex > (m_rawEventHandles.size() - 1))
					{
						throw std::runtime_error("The event signaled outside the range of pipes");
					}

					// Handle the event
					DebugTrace("Handle Event ", pipeIndex);
					HandlePipeEvent(m_pipes[pipeIndex]);
				}
			}
			catch (...)
			{
				DebugTrace("WorkerThread Failed");
				m_workerException = std::current_exception();
				m_workerFailed = true;
			}

			// Cleanup
			CleanupConnections();
		}

		/// <summary>
		/// Initialize the default set of connections
		/// </summary>
		void InitializePipes()
		{
			DebugTrace("InitializePipes");
			for (int i = 0; i < m_pipes.size(); i++)
			{
				// Create an unamed event object for this instance that is in the signaled state
				SECURITY_ATTRIBUTES* eventAttributes = nullptr;
				bool isManualReset = true;
				bool initialState = true;
				const char* eventName = nullptr;
				auto eventHandle = CreateEventA(
					eventAttributes,
					isManualReset,
					initialState,
					eventName);
				if (eventHandle == NULL)
				{
					throw std::runtime_error("CreateEventA failed: " +  std::to_string(GetLastError()));
				}

				// Save the event handle on the pipe instance and register it in the raw array to
				// allow for wait all calls and finally register this event in he overlap object
				// so it will be signaled during overlapped calls to the pipe object
				m_pipes[i].EventHandle = Opal::System::SmartHandle(eventHandle);
				m_rawEventHandles[i] = m_pipes[i].EventHandle.Get();
				m_pipes[i].Overlap.hEvent = m_pipes[i].EventHandle.Get();

				// Create the new pipe object instance
				m_pipes[i].PipeHandle = CreateNamedPipeInstance();

				// Create the connection so the clients can connect
				// Note: This call will either connect to a client or initialize the event to notify
				// when a client is connected.
				ConnectToNewClient(m_pipes[i]);
			}
		}

		/// <summary>
		/// Create a single instance of the named pipe that will allow a
		/// single client to connect to the pipe server.
		/// </summary>
		Opal::System::SmartHandle CreateNamedPipeInstance()
		{
			// Create a name for the pipe
			DebugTrace("CreateNamedPipeInstance");
			std::stringstream pipeName;
			pipeName << TBLOG_PIPE_NAMEA << "." << GetCurrentProcessId();
			std::string pipeNameString = pipeName.str();

			// Open in Read-Only mode with overlapped operations enabled
			// Turn on message type pipe in read mode with blocking waits
			DWORD openMode = PIPE_ACCESS_INBOUND | FILE_FLAG_OVERLAPPED;
			DWORD pipeMode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT;
			DWORD maxInstances = PIPE_UNLIMITED_INSTANCES;
			DWORD outBufferSize = 0;
			DWORD inBufferSize = 0;
			DWORD defaultTimeOut = 20000;
			HANDLE hPipe = CreateNamedPipeA(
				pipeNameString.c_str(),
				openMode,
				pipeMode,
				maxInstances,
				outBufferSize,
				inBufferSize,
				defaultTimeOut,
				nullptr);
			if (hPipe == INVALID_HANDLE_VALUE)
			{
				DWORD error = GetLastError();
				throw std::runtime_error("CreateNamedPipeA failed: " + std::to_string(error));
			}

			return Opal::System::SmartHandle(hPipe);
		}

		/// <summary>
		/// Attempt to connect to a new client or register the event callback when a client
		/// connects in the future.
		/// </summary>
		void ConnectToNewClient(ServerPipeInstance& pipe)
		{
			// Connect to the pipe
			DebugTrace("ConnectNamedPipe");
			if (ConnectNamedPipe(pipe.PipeHandle.Get(), &pipe.Overlap))
			{
				// Asynchronous call to Connect Named Pipe always return zero
				throw std::runtime_error("ConnectNamedPipe should not succeed.");
			}

			// Check the error to determine the actual result
			DWORD error = GetLastError();
			switch (error)
			{
			case ERROR_PIPE_CONNECTED:
				// The client connected in the time between create named pipe and connect
				// Rare, but possible.
				DebugTrace("ConnectNamedPipe - Connected");
				pipe.IsConnected = true;
				pipe.HasPendingIO = false;
				m_hasAnyClients = true;
				m_activeClientCount++;

				// Signal the event manually to force the update loop to signal on wait all events
				if (!SetEvent(pipe.EventHandle.Get()))
				{
					throw std::runtime_error("Client already connected -> SetEvent failed: " + std::to_string(GetLastError()));
				}
	
				break;
			case ERROR_IO_PENDING:
				DebugTrace("ConnectNamedPipe - Pending");
				pipe.IsConnected = false;
				pipe.HasPendingIO = true;
				break;
			default:
				throw std::runtime_error("ConnectNamedPipe failed: " + std::to_string(error));
			}
		}

		/// <summary>
		/// Disconnect from a client and reset the pipe to prepare for a new client to connect.
		/// </summary>
		VOID DisconnectAndReconnect(ServerPipeInstance& pipe)
		{
			// Disconnect the old pipe instance
				DebugTrace("DisconnectAndReconnect");
			if (!DisconnectNamedPipe(pipe.PipeHandle.Get()))
			{
				throw std::runtime_error("DisconnectNamedPipe failed: " + std::to_string(GetLastError()));
			}

			m_activeClientCount--;

			// Connect to a new client
			ConnectToNewClient(pipe);
		}

		void HandlePipeEvent(ServerPipeInstance& pipe)
		{
			// Check if a pending IO operation requires a call to get Overlapped result
			DebugTrace("HandlePipeEvent");
			if (pipe.HasPendingIO)
			{
				bool shouldWait = false;
				DWORD bytesTransferred = 0;
				DebugTrace("HandlePipeEvent - GetOverlappedResult");
				bool overlappedResult = GetOverlappedResult(
					pipe.PipeHandle.Get(),
					&pipe.Overlap,
					&bytesTransferred,
					shouldWait);

				if (!pipe.IsConnected)
				{
					if (!overlappedResult)
					{
						throw std::runtime_error("GetOverlappedResult failed when connecting: " + std::to_string(GetLastError()));
					}

					// Connected to a new client
					// Fall through and perform an initial read check
					DebugTrace("HandlePipeEvent - GetOverlappedResult - Connected");
					pipe.IsConnected = true;
					m_hasAnyClients = true;
					m_activeClientCount++;
				}
				else
				{
					// Check if the client is gone
					DebugTrace("HandlePipeEvent - GetOverlappedResult - Read Finished");
					if (!overlappedResult || bytesTransferred == 0)
					{
						DebugTrace("HandlePipeEvent - GetOverlappedResult - Error: " + std::to_string(GetLastError()));
						DisconnectAndReconnect(pipe);
						return;
					}

					DWORD expectedSize = pipe.Message.ContentSize +
						sizeof(Monitor::DetourMessage::Type) +
						sizeof(Monitor::DetourMessage::ContentSize);
					if (bytesTransferred != expectedSize)
					{
						DebugTrace("HandlePipeEvent - GetOverlappedResult - Size Mismatched");
						DisconnectAndReconnect(pipe);
						return;
					}

					// Otherwise the Pending read has finished succesfully
					LogMessage(pipe.Message);
				}
			}

			// If we are connected attempt to perform a read
			if (pipe.IsConnected)
			{
				// Read the next message
				DebugTrace("HandlePipeEvent - ReadFile");
				DWORD bytesRead = 0;
				if (!ReadFile(
					pipe.PipeHandle.Get(),
					&pipe.Message,
					sizeof(pipe.Message),
					&bytesRead,
					&pipe.Overlap))
				{
					DWORD error = GetLastError();
					switch (error)
					{
						case ERROR_IO_PENDING:
							DebugTrace("HandlePipeEvent - ReadFile - Pending");
							pipe.HasPendingIO = true;
							break;
						case ERROR_BROKEN_PIPE:
							// The client is gone
							DebugTrace("HandlePipeEvent - ReadFile - Pipe Ended");
							DisconnectAndReconnect(pipe);
							break;
						default:
							// Unknown error
							DebugTrace("HandlePipeEvent - ReadFile - Error: " + std::to_string(GetLastError()));
							DisconnectAndReconnect(pipe);
							break;
					}

					return;
				}

				if (bytesRead == 0)
				{
					throw std::runtime_error("Bytes read did not match expected");
				}

				DWORD expectedSize = pipe.Message.ContentSize +
					sizeof(Monitor::DetourMessage::Type) +
					sizeof(Monitor::DetourMessage::ContentSize);
				if (bytesRead != expectedSize)
				{
					throw std::runtime_error("HandlePipeEvent - GetOverlappedResult - Size Mismatched");
				}

				LogMessage(pipe.Message);
				pipe.HasPendingIO = false;
			}
		}

		void LogMessage(DetourMessage& message)
		{
			DebugTrace("LogMessage");
			m_eventListener.LogMessage(message);
		}

		void CleanupConnections()
		{
			DebugTrace("CleanupConnections");
			for (int i = 0; i < m_pipes.size(); i++)
			{
				m_pipes[i].EventHandle.Close();
				m_rawEventHandles[i] = NULL;
				m_pipes[i].Overlap.hEvent = NULL;
				m_pipes[i].PipeHandle.Close();
			}
		}


		void DebugTrace(std::string_view message, uint32_t value)
		{
#ifdef TRACE_DETOUR_SERVER
			std::cout << "DETOUR-SERVER: " << message << " " << value << std::endl;
#endif
		}

		void DebugTrace(std::string_view message)
		{
#ifdef TRACE_DETOUR_SERVER
			std::cout << "DETOUR-SERVER: " << message << std::endl;
#endif
		}

	private:
		// Input
		Path m_executable;
		std::string m_arguments;
		Path m_workingDirectory;
		EventListener m_eventListener;

		// Runtime
		std::thread m_workerThread;
		std::array<ServerPipeInstance, 4> m_pipes;
		std::array<HANDLE, 4> m_rawEventHandles;
		bool m_hasAnyClients;
		int m_activeClientCount;

		std::atomic<bool> m_processRunning;
		std::atomic<bool> m_workerFailed;
		std::exception_ptr m_workerException = nullptr;

		Opal::System::SmartHandle m_processHandle;
		Opal::System::SmartHandle m_threadHandle;
		Opal::System::SmartHandle m_stdOutReadHandle;
		Opal::System::SmartHandle m_stdOutWriteHandle;
		Opal::System::SmartHandle m_stdErrReadHandle;
		Opal::System::SmartHandle m_stdErrWriteHandle;
		Opal::System::SmartHandle m_stdInReadHandle;
		Opal::System::SmartHandle m_stdInWriteHandle;

		// Result
		bool m_isFinished;
		std::stringstream m_stdOut;
		std::stringstream m_stdErr;
		int m_exitCode;
	};
}
