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
			posix_spawn_file_actions_t* fileActions = nullptr;
			posix_spawnattr_t* attributes = nullptr;

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

			// Start the process
			pid_t processId;
			auto status = posix_spawn(
				&processId,
				m_executable.ToString().c_str(),
				fileActions,
				attributes,
				const_cast<char**>(arguments.data()),
				environ);

			// Reset working directory
			if (chdir(currentWorkingDirectory.string().c_str()) == -1)
			{
				throw std::runtime_error("Failed to reset working directory");
			}

			if (status != 0)
			{
				throw std::runtime_error("Execute posix_spawn Failed: " + std::to_string(status));
			}

			m_processId = processId;
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
