// <copyright file="LinuxMonitorProcess.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

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

		// Runtime
		pid_t m_processId;
		int m_stdOutReadHandle;

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
			const Path& workingDirectory) :
			m_executable(executable),
			m_arguments(std::move(arguments)),
			m_workingDirectory(workingDirectory),
			m_isFinished(false),
			m_exitCode(-1)
		{
		}

		/// <summary>
		/// Execute a process for the provided
		/// </summary>
		void Start() override final
		{
			std::vector<const char*> arguments;
			arguments.push_back(m_executable.ToString().c_str());
			for (auto& argument : m_arguments)
				arguments.push_back(argument.c_str());
			arguments.push_back(nullptr);

			// Set current working directory that will be inherited by the child process
			auto currentWorkingDirectory = std::filesystem::current_path();
			if (chdir(m_workingDirectory.ToString().c_str()) == -1)
			{
				throw std::runtime_error("Failed to set working directory");
			}

			// Create a pipe to send stdout to parent
			int stdOutPipe[2];
			if (pipe(stdOutPipe) < 0)
			{
				throw std::runtime_error("Failed to create stdOutPipe");
			}

			// Create a child process
			pid_t processId = fork();
			if (processId == 0)
			{
				// We are the child process

				// Close the read pipe
				close(stdOutPipe[0]);

				// Redirect stdout to the pipe write
				if (dup2(stdOutPipe[1], STDOUT_FILENO) != STDOUT_FILENO)
				{
					throw std::runtime_error("dup2 error to stdout");
				}

				// Close our handle on the write end
				close(stdOutPipe[1]);

				// Replace runtime with child program
				execve(
					m_executable.ToString().c_str(),
					const_cast<char**>(arguments.data()),
					environ);

				// Running in other program now
			}

			// Parent process still

			// Reset working directory
			if (chdir(currentWorkingDirectory.string().c_str()) == -1)
			{
				throw std::runtime_error("Failed to reset working directory");
			}

			m_processId = processId;
			
			// Close our handle on the write end
			close(stdOutPipe[1]);
			m_stdOutReadHandle = stdOutPipe[0];
		}

		/// <summary>
		/// Wait for the process to exit
		/// </summary>
		void WaitForExit() override final
		{
			// Wait until child process exits.
			int status;
			auto waitResult = waitpid(m_processId, &status, 0);
			if (!waitResult)
			{
				throw std::runtime_error("Execute waitpid Failed Unknown");
			}

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
			// while (true)
			// {
			// 	if(!ReadFile(m_stdErrReadHandle.Get(), buffer, BufferSize, &dwRead, nullptr))
			// 		break;
			// 	if (dwRead == 0)
			// 		break;

			// 	// Make the string null terminated
			// 	m_stdErr << std::string_view(buffer, dwRead);
			// }

			m_exitCode = status;
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
	};
}
