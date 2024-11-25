// <copyright file="LinuxMonitorProcess.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "LinuxSystemAccessMonitor.h"
#include "LinuxSystemLoggerMonitor.h"
#include "LinuxSystemMonitorFork.h"
#include "LinuxTraceEventListener.h"

namespace Monitor::Linux
{
	/// <summary>
	/// A Linux platform specific process executable using system
	/// </summary>
	export class LinuxMonitorProcess : public Opal::System::IProcess
	{
	private:
		// Input
		Path m_executable;
		std::vector<std::string> m_arguments;
		Path m_workingDirectory;
		LinuxTraceEventListener m_eventListener;

		// Runtime
		pid_t m_processId;
		int m_stdOutReadHandle;
		int m_stdErrReadHandle;

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
			std::shared_ptr<ISystemAccessMonitor> monitor) :
			m_executable(executable),
			m_arguments(std::move(arguments)),
			m_workingDirectory(workingDirectory),
	#ifdef TRACE_DETOUR_SERVER
			m_eventListener(std::make_shared<LinuxSystemMonitorFork>(
				std::make_shared<LinuxSystemLoggerMonitor>(std::cout),
				std::make_shared<LinuxSystemAccessMonitor>(std::move(monitor)))),
	#else
			m_eventListener(std::make_shared<LinuxSystemAccessMonitor>(std::move(monitor))),
	#endif
			m_processId(),
			m_stdOutReadHandle(),
			m_stdErrReadHandle(),
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

				// Create the worker thread that will monitor the child process
				m_processRunning = true;
				m_workerFailed = false;
				DebugTrace("Thread");
				// m_workerThread = std::thread(&LinuxMonitorProcess::WorkerThread, std::ref(*this));
				
				WorkerThread();

				DebugTrace("Parent done");
			}
		}

		/// <summary>
		/// Wait for the process to exit
		/// </summary>
		void WaitForExit() override final
		{
			// Wait until child process exits.
			// m_workerThread.join();

			m_processRunning = false;

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

			m_stdOut << std::flush;

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

			m_stdErr << std::flush;

			close(m_stdErrReadHandle);

			m_isFinished = true;

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

				scmp_filter_ctx ctx;
				try
				{
					scmp_filter_ctx ctx = seccomp_init(SCMP_ACT_ALLOW);
					if (ctx == NULL)
						throw std::runtime_error("seccomp_init failed");

					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(open), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(openat), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(openat2), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(creat), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(link), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(linkat), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(rename), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(renameat), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(renameat2), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(unlink), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(mkdir), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(mkdirat), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(rmdir), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(fork), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(vfork), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(clone), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(clone3), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");
					// TODO: Allow first execve when the parent has not connected yet to allow it
					// Maybe try to filter to the known exe and only allow that and trace others
					// https://lore.kernel.org/lkml/20201029075841.GB29881@ircssh-2.c.rugged-nimbus-611.internal/T/
					// if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(execve), 0) < 0)
					// 	throw std::runtime_error("seccomp_rule_add failed");
					if (seccomp_rule_add(ctx, SCMP_ACT_TRACE(1), SCMP_SYS(execveat), 0) < 0)
						throw std::runtime_error("seccomp_rule_add failed");

					if (seccomp_load(ctx) < 0)
						throw std::runtime_error("seccomp_load failed");

					seccomp_release(ctx);
				}
				catch(const std::exception& e)
				{
					// Ensure we cleanup nicely
					seccomp_release(ctx);
					throw;
				}

				ptrace(PTRACE_TRACEME, 0, NULL, NULL);

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
				auto result = execve(
					m_executable.ToString().c_str(),
					const_cast<char**>(arguments.data()),
					const_cast<char**>(environmentArray.data()));
				if (result == -1)
					throw std::runtime_error("Failed to start child");
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
			Log::Diag("WorkerThread Start");
			int status;

			// Wait for the first notification from the child
			auto currentProcessId = waitpid(m_processId, &status, 0);
			if (currentProcessId == -1)
				throw std::runtime_error("Wait failed");

			// Enable SecComp filtering
			unsigned int ptraceOptions =
				PTRACE_O_TRACESECCOMP |
				PTRACE_O_TRACECLONE |
				PTRACE_O_TRACEFORK |
				PTRACE_O_TRACEVFORK;

			ptrace(PTRACE_SETOPTIONS, m_processId, 0, ptraceOptions);
			ptrace(PTRACE_CONT, m_processId, NULL, NULL);

			while (true)
			{
				currentProcessId = wait(&status);
				if (currentProcessId == -1)
					throw std::runtime_error("Wait failed");

				if (WIFEXITED(status))
				{
					int exitCode = WEXITSTATUS(status);
					if (currentProcessId == m_processId)
					{
						m_exitCode = exitCode;
						return;
					}
					else
					{
						DebugTrace("Child exit: ", currentProcessId);
					}
				}
				else if (WIFSTOPPED(status))
				{
					auto signal = WSTOPSIG(status);
					switch (signal)
					{
						case SIGTRAP:
						{
							auto event = (unsigned int)status >> 16;
							switch (event)
							{
								case 0:
									// Process start
									break;
								case PTRACE_EVENT_SECCOMP:
									m_eventListener.ProcessSysCall(currentProcessId);
									break;
								case PTRACE_EVENT_FORK:
								case PTRACE_EVENT_VFORK:
								case PTRACE_EVENT_CLONE:
									// Entering child process
									break;
								default:
									std::cout << "UNKNOWN PTRACE EVENT: " << event << " STATUS: " << status << " PID: " << currentProcessId << std::endl;
									break;
							}

							break;
						}
						case SIGSTOP:
						{
							// Trace clone
							break;
						}
						case SIGCLD:
						{
							// Child signaled
							break;
						}
						default:
						{
							std::cout << "UNKNOWN SIGNAL: " << signal << " STATUS: " << status << " PID: " << currentProcessId << std::endl;
							break;
						}
					}
				}
				else
				{
					std::cout << "UNKNOWN STATUS: " << status << " PID: " << currentProcessId << std::endl;
				}

				ptrace(PTRACE_CONT, currentProcessId, NULL, NULL);
			}
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
