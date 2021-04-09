// <copyright file="RuntimeProcess.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Diagnostics;

namespace Opal.System
{
	/// <summary>
	/// The shared runtime process executable using system
	/// </summary>
	public class RuntimeProcess : IProcess
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='RuntimeProcess'/> class.
		/// </summary>
		public RuntimeProcess(
			Path executable,
			string arguments,
			Path workingDirectory)
		{
			m_executable = executable;
			m_arguments = arguments;
			m_workingDirectory = workingDirectory;
		}

		/// <summary>
		/// Execute a process for the provided
		/// </summary>
		public void Start()
		{
			var processInfo = new ProcessStartInfo()
			{
				FileName = m_executable.ToString(),
				Arguments = m_arguments,
				WorkingDirectory = m_workingDirectory.ToString(),
				RedirectStandardOutput = true,
				RedirectStandardError = true,
			};

			m_process = Process.Start(processInfo);
			if (m_process is null)
				throw new InvalidOperationException("Failed to start process");
		}

		/// <summary>
		/// Wait for the process to exit
		/// </summary>
		public void WaitForExit()
		{
			if (m_process is null)
				throw new InvalidOperationException("Cannot wait on process that is not running");
			m_process.WaitForExit();
		}

		/// <summary>
		/// Get the exit code
		/// </summary>
		public int GetExitCode()
		{
			if (m_process is null)
				throw new InvalidOperationException("Cannot access process that does not exist");
			return m_process.ExitCode;
		}

		/// <summary>
		/// Get the standard output
		/// </summary>
		public string GetStandardOutput()
		{
			if (m_process is null)
				throw new InvalidOperationException("Cannot access process that does not exist");
			return m_process.StandardOutput.ReadToEnd();
		}

		/// <summary>
		/// Get the standard error output
		/// </summary>
		public string GetStandardError()
		{
			if (m_process is null)
				throw new InvalidOperationException("Cannot access process that does not exist");
			return m_process.StandardError.ReadToEnd();
		}

		// Input
		private Path m_executable;
		private string m_arguments;
		private Path m_workingDirectory;

		// Runtime
		Process? m_process;
	}
}
