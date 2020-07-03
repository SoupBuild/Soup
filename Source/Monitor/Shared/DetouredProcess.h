#pragma once 
#include "IDetourCallback.h"

namespace Monitor
{
	// TODO: May want to create a project that is not shared but only contains consumer code
	export class DetouredProcess
	{
	public:
		DetouredProcess(IDetourCallback& callback) :
			m_callback(callback)
		{
		}

		int RunProcess(
			const Path& application,
			const std::vector<std::string>& arguments)
		{
			auto applicationString = application.ToAlternateString();

			std::stringstream argumentsValue;
			argumentsValue << "\"" << applicationString << "\"";
			for (auto& arg : arguments)
			{
				argumentsValue << " " << arg;
			}

			std::string argumentsString = argumentsValue.str();

			// Create a name for the pipe
			std::stringstream pipeName;
			pipeName << TBLOG_PIPE_NAMEA << "." << GetCurrentProcessId();
			std::string pipeNameString = pipeName.str();

			// Create completion port worker threads.
			CreatePipeConnection(pipeNameString);

			m_workerFailed = false;
			auto workerThread = std::thread(&DetouredProcess::WorkerThread, std::ref(*this));

			// Build up the detour dlls absolute path
			auto moduleName = System::IProcessManager::Current().GetProcessFileName();
			auto moduleFolder = moduleName.GetParent();
			auto dllPath = moduleFolder + Path("Monitor.Detours.64.dll");
			auto dllPathString = dllPath.ToAlternateString();

			// Create the requested process with our detour dll loaded
			STARTUPINFOA si;
			PROCESS_INFORMATION pi;
			ZeroMemory(&si, sizeof(si));
			ZeroMemory(&pi, sizeof(pi));
			si.cb = sizeof(si);
			DWORD dwFlags = CREATE_DEFAULT_ERROR_MODE | CREATE_SUSPENDED;
			if (!DetourCreateProcessWithDllExA(
				applicationString.c_str(),
				argumentsString.data(),
				nullptr,
				nullptr,
				true,
				dwFlags,
				nullptr,
				nullptr,
				&si,
				&pi,
				dllPathString.c_str(),
				nullptr))
			{
				auto error = GetLastError();
				throw std::runtime_error("DetourCreateProcessWithDllEx failed: " + std::to_string(error));
			}

			// Set the detoured process payload
			DetourPayload payload;
			ZeroMemory(&payload, sizeof(payload));
			payload.nParentProcessId = GetCurrentProcessId();
			payload.nTraceProcessId = GetCurrentProcessId();
			payload.nGeneology = 1;
			payload.rGeneology[0] = 0;
			payload.wzParents[0] = 0;
			if (!DetourCopyPayloadToProcess(
				pi.hProcess,
				GuidTrace,
				&payload,
				sizeof(payload)))
			{
				throw std::runtime_error("DetourCopyPayloadToProcess failed: " + std::to_string(GetLastError()));
			}

			// Restert the process
			auto hr = ResumeThread(pi.hThread);

			// Wait for the process to finish
			WaitForSingleObject(pi.hProcess, INFINITE);

			// Get the output result
			DWORD result = 0;
			if (!GetExitCodeProcess(pi.hProcess, &result))
			{
				throw std::runtime_error("GetExitCodeProcess failed: " + std::to_string(GetLastError()));
			}

			// Wait for the worker thread to exit
			workerThread.join();

			if (m_workerFailed)
				throw std::runtime_error("Worker failed");

			return result;
		}

	private:
		void LogMessage(DetourMessage* pMessage, DWORD nBytes)
		{
			// Sanity check the size of the message.
			if (nBytes > pMessage->ContentSize)
			{
				nBytes = pMessage->ContentSize;
			}

			if (nBytes >= sizeof(*pMessage))
			{
				nBytes = sizeof(*pMessage) - 1;
			}

			// Don't log message if there isn't and message text.
			DWORD cbWrite = nBytes - offsetof(DetourMessage, Content);
			if (cbWrite <= 0)
			{
				throw std::runtime_error("Not enough data.");
			}

			switch (pMessage->Type)
			{
				case DetourMessageType::Exit:
					m_callback.OnExit();
					break;
				case DetourMessageType::Error:
					m_callback.OnError();
					break;
				case DetourMessageType::CopyFile:
					m_callback.OnCopyFile();
					break;
				case DetourMessageType::CreateDirectory:
				{
					// Null terminate the string
					pMessage->Content[nBytes] = 0;
					auto directory = std::string_view(reinterpret_cast<char*>(pMessage->Content));
					m_callback.OnCreateDirectory(directory);
					break;
				}
				case DetourMessageType::CreateFile:
				{
					// Null terminate the string
					pMessage->Content[nBytes] = 0;
					auto file = std::string_view(reinterpret_cast<char*>(pMessage->Content));
					m_callback.OnCreateFile(file);
					break;
				}
				case DetourMessageType::CreateHardLink:
					m_callback.OnCreateHardLink();
					break;
				case DetourMessageType::CreateProcess:
					m_callback.OnCreateProcess();
					break;
				case DetourMessageType::DeleteFile:
				{
					// Null terminate the string
					pMessage->Content[nBytes] = 0;
					auto file = std::string_view(reinterpret_cast<char*>(pMessage->Content));
					m_callback.OnDeleteFile(file);
					break;
				}
				case DetourMessageType::GetEnvironmentVariable:
					m_callback.OnGetEnvironmentVariable();
					break;
				case DetourMessageType::GetFileAttributes:
				{
					// Null terminate the string
					pMessage->Content[nBytes] = 0;
					auto directory = std::string_view(reinterpret_cast<char*>(pMessage->Content));
					m_callback.OnGetFileAttributes(directory);
					break;
				}
				case DetourMessageType::LoadLibrary:
					m_callback.OnLoadLibrary();
					break;
				case DetourMessageType::MoveFile:
					m_callback.OnMoveFile();
					break;
				case DetourMessageType::OpenFile:
				{
					// Null terminate the string
					pMessage->Content[nBytes] = 0;
					auto file = std::string_view(reinterpret_cast<char*>(pMessage->Content));
					m_callback.OnOpenFile(file);
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown message type");
				}
			}
		}

		void CloseConnection()
		{
			if (m_pipeHandle != INVALID_HANDLE_VALUE)
			{
				//FlushFileBuffers(m_pipeHandle);
				if (!DisconnectNamedPipe(m_pipeHandle))
				{
					DWORD error = GetLastError();
					throw std::runtime_error("DisconnectNamedPipe Failed: " + std::to_string(error));
				}

				CloseHandle(m_pipeHandle);
				m_pipeHandle = INVALID_HANDLE_VALUE;
			}
		}

		void CreatePipeConnection(const std::string& pipeName)
		{
			// Open in Read-Only mode with synchronous operations enabled
			// Turn on message type pipe in read mode with blocking waits
			DWORD openMode = PIPE_ACCESS_INBOUND;
			DWORD pipeMode = PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT;
			DWORD maxInstances = PIPE_UNLIMITED_INSTANCES;
			DWORD outBufferSize = 0;
			DWORD inBufferSize = 0;
			DWORD defaultTimeOut = 20000;
			HANDLE hPipe = CreateNamedPipeA(
				pipeName.c_str(),
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

			m_pipeHandle = hPipe;
		}

		void WorkerThread()
		{
			try
			{
				// Connect to the pipe
				if (!ConnectNamedPipe(m_pipeHandle, nullptr))
				{
					// If completed then check the actual error
					DWORD error = GetLastError();
					switch (error)
					{
					case ERROR_PIPE_CONNECTED:
						// All good
						break;
					default:
						throw std::runtime_error("ConnectNamedPipe failed: " + std::to_string(error));
					}
				}

				// Read until we run out of messages
				bool isDone = false;
				DetourMessage message;
				while (!isDone || m_processExited)
				{
					// Read the next message
					DWORD bytesRead = 0;
					if (!ReadFile(
						m_pipeHandle,
						&message,
						sizeof(message),
						&bytesRead,
						nullptr))
					{
						DWORD error = GetLastError();
						throw std::runtime_error("ReadFile failed: " + std::to_string(error));
					}

					auto offset = offsetof(Monitor::DetourMessage, Content);
					if (bytesRead <= offset)
					{
						isDone = true;
					}
					else
					{
						LogMessage(&message, bytesRead - offset);
					}
				}

				CloseConnection();
			}
			catch (...)
			{
				m_workerFailed = true;
			}
		}

	private:
		IDetourCallback& m_callback;
		HANDLE m_pipeHandle;
		std::atomic<bool> m_processExited;
		std::atomic<bool> m_workerFailed;
	};
}
