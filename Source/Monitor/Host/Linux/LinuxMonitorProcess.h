// <copyright file="LinuxMonitorProcess.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "DetourMonitorCallback.h"
#include "DetourCallbackLogger.h"
#include "DetourForkCallback.h"
#include "EventListener.h"

namespace Monitor::Linux
{
	/// <summary>
	/// A Linux platform specific process executable using system
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class LinuxMonitorProcess : public Opal::System::IProcess
	{
	private:
		// Input
		Path m_executable;
		std::vector<std::string> m_arguments;
		Path m_workingDirectory;
		EventListener m_eventListener;

		// Runtime
		pid_t m_processId;
		int m_stdOutReadHandle;
		int m_stdErrReadHandle;
		int m_pipeHandle;

		std::thread m_workerThread;
		std::atomic<bool> m_processRunning;
		std::atomic<bool> m_workerFailed;
		std::exception_ptr m_workerException = nullptr;

		// Result
		bool m_isFinished;
		std::stringstream m_stdOut;
		std::stringstream m_stdErr;
		int m_exitCode;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='LinuxMonitorProcess'/> class.
		/// </summary>
		LinuxMonitorProcess(
			const Path& executable,
			std::vector<std::string> arguments,
			const Path& workingDirectory,
			std::shared_ptr<IMonitorCallback> callback) :
			m_executable(executable),
			m_arguments(std::move(arguments)),
			m_workingDirectory(workingDirectory),
#ifdef TRACE_DETOUR_SERVER
			m_eventListener(std::make_shared<DetourForkCallback>(
				std::make_shared<DetourCallbackLogger>(std::cout),
				std::make_shared<DetourMonitorCallback>(std::move(callback)))),
#else
			m_eventListener(std::make_shared<DetourMonitorCallback>(std::move(callback))),
#endif
			m_processId(),
			m_stdOutReadHandle(),
			m_stdErrReadHandle(),
			m_pipeHandle(),
			m_workerThread(),
			m_processRunning(),
			m_workerFailed(),
			m_isFinished(false),
			m_exitCode(-1)
		{
		}

		/// <summary>
		/// Execute a process for the provided
		/// </summary>
		void Start() override final
		{
			// Set current working directory that will be inherited by the child process
			auto currentWorkingDirectory = std::filesystem::current_path();
			if (chdir(m_workingDirectory.ToString().c_str()) == -1)
				throw std::runtime_error("Failed to set working directory");

			// Create a pipe to send stdout to parent
			int stdOutPipe[2];
			if (pipe(stdOutPipe) < 0)
				throw std::runtime_error("Failed to create stdOutPipe");

			// Create a pipe to send stderr to parent
			int stdErrPipe[2];
			if (pipe(stdErrPipe) < 0)
				throw std::runtime_error("Failed to create stdErrPipe");

			// Initialize the pipes so they are ready for the process and the worker thread
			DebugTrace("Create fifo");
			auto pipeName = std::string("/tmp/soupbuildfifo");
			if (mkfifo(pipeName.c_str(), 0666) != 0)
				throw std::runtime_error("Failed to create pipe");

			// Create a child process
			DebugTrace("Fork");
			pid_t processId = fork();
			if (processId == 0)
			{
				SetupChildProcess(stdOutPipe, stdErrPipe);
			}
			else
			{
				// Parent process still
				DebugTrace("Parent");

				// Reset working directory
				if (chdir(currentWorkingDirectory.string().c_str()) == -1)
					throw std::runtime_error("Failed to reset working directory");

				m_processId = processId;

				// Close our handle on the write end
				close(stdOutPipe[1]);
				close(stdErrPipe[1]);
				m_stdOutReadHandle = stdOutPipe[0];
				m_stdErrReadHandle = stdErrPipe[0];

				// Create the worker thread that will act as the pipe server
				m_processRunning = true;
				m_workerFailed = false;
				DebugTrace("Thread");
				m_workerThread = std::thread(&LinuxMonitorProcess::WorkerThread, std::ref(*this));
				
				DebugTrace("Parent done");
			}
		}

		/// <summary>
		/// Wait for the process to exit
		/// </summary>
		void WaitForExit() override final
		{
			// Wait until child process exits.
			int status;
			auto waitResult = waitpid(m_processId, &status, 0);
			m_processRunning = false;
			if (!waitResult)
				throw std::runtime_error("Execute waitpid Failed Unknown");

			// Read all and write to stdout
			// TODO: May want to switch over to a background thread with peak to read in order
			int dwRead;
			const int BufferSize = 256;
			char buffer[BufferSize + 1];

			// Read on output
			while (true)
			{
				dwRead = read(m_stdOutReadHandle, buffer, BufferSize);
				if(dwRead < 0)
					break;
				if (dwRead == 0)
					break;

				m_stdOut << std::string_view(buffer, dwRead);
			}

			close(m_stdOutReadHandle);

			// Read all errors
			while (true)
			{
				dwRead = read(m_stdErrReadHandle, buffer, BufferSize);
				if(dwRead < 0)
					break;
				if (dwRead == 0)
					break;

				// Make the string null terminated
				m_stdErr << std::string_view(buffer, dwRead);
			}

			close(m_stdErrReadHandle);

			m_exitCode = status;
			m_isFinished = true;

			// Wait for the worker thread to exit
			m_workerThread.join();

			if (m_workerFailed)
			{
				std::rethrow_exception(m_workerException);
			}
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
		void SetupChildProcess(int stdOutPipe[2], int stdErrPipe[2])
		{
			try
			{
				// We are the child process
				DebugTrace("Child");

				// Close the read pipe
				close(stdOutPipe[0]);
				close(stdErrPipe[0]);

				// Redirect stdout to the pipe write
				if (dup2(stdOutPipe[1], STDOUT_FILENO) != STDOUT_FILENO)
					throw std::runtime_error("dup2 error to stdout");

				// Redirect stderr to the pipe write
				if (dup2(stdErrPipe[1], STDERR_FILENO) != STDERR_FILENO)
					throw std::runtime_error("dup2 error to stderr");

				// Close our handle on the write end
				close(stdOutPipe[1]);
				close(stdErrPipe[1]);

				auto environment = std::vector<std::string>();

				environment.push_back("HOME=/");
				environment.push_back("USER=USERNAME");
				environment.push_back("PAHT=/usr/bin");

				// Preload the monitor client first
				environment.push_back("LD_PRELOAD=/home/mwasplund/dev/repos/Soup/out/run/Monitor.Client.64.so");

				std::vector<const char*> arguments;
				arguments.push_back(m_executable.ToString().c_str());
				for (auto& argument : m_arguments)
					arguments.push_back(argument.c_str());
				arguments.push_back(nullptr);

				auto environmentArray = std::vector<const char*>();
				for (auto& value : environment)
					environmentArray.push_back(value.c_str());
				environmentArray.push_back(nullptr);

				// Replace runtime with child program
				DebugTrace("child exec");
				execve(
					m_executable.ToString().c_str(),
					const_cast<char**>(arguments.data()),
					const_cast<char**>(environmentArray.data()));
			}
			catch(const std::exception& e)
			{
				std::cerr << e.what() << '\n';
				exit(1234);
			}

			// Running in other program now
		}

		/// <summary>
		/// The main entry point for the worker thread that will monitor incoming messages from all
		/// client connections.
		/// </summary>
		void WorkerThread()
		{
			auto pipeName = std::string("/tmp/soupbuildfifo");

			try
			{
				Log::Diag("WorkerThread Start");

				Log::Diag("Open read pipe");
				m_pipeHandle = open(pipeName.c_str(), O_RDONLY | O_NONBLOCK);
				if (m_pipeHandle < 0)
				{
					Log::Error("Open read pipe failed");
					throw std::runtime_error("Open read pipe failed");
				}

				Log::Diag("Waiting on first message");

				// Read until we get a client and then all clients disconnect
				struct pollfd waiter = {.fd = m_pipeHandle, .events = POLLIN};
				bool writerClosed = false;
				while (m_processRunning && !writerClosed)
				{
					// 10 seconds
					auto waitStatus = poll(&waiter, 1, 10 * 1000);
					switch (waitStatus)
					{
						case 0:
							Log::Error("Read pipe timeout");
							writerClosed = true;
						case 1:
							if (waiter.revents & POLLIN)
							{
								ReadMessage();
							}
							else if (waiter.revents & POLLERR)
							{
								Log::Error("POLLERR");
								throw std::runtime_error("POLLERR");
							}
							else if (waiter.revents & POLLHUP)
							{
								Log::Diag("Writer closed");
								writerClosed = true;
							}
							break;
						default:
							Log::Error("Unknown poll status");
							throw std::runtime_error("Unknown poll status");
					}
				}
			}
			catch (...)
			{
				Log::Error("WorkerThread Failed");
				m_workerException = std::current_exception();
				m_workerFailed = true;
			}

			// Cleanup
			Log::Diag("close pipe");
			if (close(m_pipeHandle) != 0)
			{
				Log::Error("Close pipe failed");
				throw std::runtime_error("Close pipe failed");
			}

			Log::Diag("delete pipe");
			if (unlink(pipeName.c_str()) != 0)
			{
				Log::Error("unlink pipe failed");
				throw std::runtime_error("unlink pipe failed");
			}
		}

		void ReadMessage()
		{
			DebugTrace("Read message");

			Message message;
			int expectedHeaderSize = sizeof(Message::Type) + sizeof(Message::ContentSize);
			int bytesRead = read(m_pipeHandle, &message, expectedHeaderSize);
			if (bytesRead > 0)
			{
				// Handle the event
				DebugTrace("Handle Event");
				if (bytesRead != expectedHeaderSize)
				{
					throw std::runtime_error(
						std::format("HandlePipeEvent - Header size wrong: {} {}",
							bytesRead,
							expectedHeaderSize));
				}

				// Read the raw content
				auto expectedSize = message.ContentSize +
					sizeof(Message::Type) +
					sizeof(Message::ContentSize);
				bytesRead = read(m_pipeHandle, &(message.Content), message.ContentSize);
				if (bytesRead != message.ContentSize)
				{
					throw std::runtime_error(
						std::format(
							"HandlePipeEvent - Size Mismatched: {} {}",
							bytesRead,
							message.ContentSize));
				}

				LogMessage(message);
			}
		}

		void LogMessage(Message& message)
		{
			DebugTrace("LogMessage");
			m_eventListener.SafeLogMessage(message);
		}

		void DebugTrace(std::string_view message, uint32_t value)
		{
#ifdef TRACE_MONITOR_HOST
			std::cout << "Monitor-HOST: " << message << " " << value << std::endl;
#endif
		}

		void DebugTrace(std::string_view message)
		{
#ifdef TRACE_MONITOR_HOST
			std::cout << "Monitor-HOST: " << message << std::endl;
#endif
		}
	};
}
