// <copyright file="MockProcess.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Opal.System
{
	/// <summary>
	/// A mock process executable using system
	/// </summary>
	public class MockProcess : IProcess
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='MockProcess'/> class.
		/// </summary>
		public MockProcess(
			int id,
			List<string> requests,
			int exitCode,
			string stdOut,
			string stdErr)
		{
			m_id = id;
			m_requests = requests;
			m_isFinished = false;
			m_stdOut = stdOut;
			m_stdErr = stdErr;
			m_exitCode = exitCode;
		}

		/// <summary>
		/// Execute a process for the provided
		/// </summary>
		public void Start()
		{
			m_requests.Add($"ProcessStart: {m_id}");
		}

		/// <summary>
		/// Wait for the process to exit
		/// </summary>
		public void WaitForExit()
		{
			m_requests.Add($"WaitForExit: {m_id}");

			m_isFinished = true;
		}

		/// <summary>
		/// Get the exit code
		/// </summary>
		public int GetExitCode()
		{
			m_requests.Add($"GetExitCode: {m_id}");

			if (!m_isFinished)
				throw new InvalidOperationException("Process has not finished.");
			return m_exitCode;
		}

		/// <summary>
		/// Get the standard output
		/// </summary>
		public string GetStandardOutput()
		{
			m_requests.Add($"GetStandardOutput: {m_id}");

			if (!m_isFinished)
				throw new InvalidOperationException("Process has not finished.");
			return m_stdOut;
		}

		/// <summary>
		/// Get the standard error output
		/// </summary>
		public string GetStandardError()
		{
			m_requests.Add($"GetStandardError: {m_id}");

			if (!m_isFinished)
				throw new InvalidOperationException("Process has not finished.");
			return m_stdErr;
		}

		// Input
		private int m_id;
		private List<string> m_requests;

		// Result
		private bool m_isFinished;
		private string m_stdOut;
		private string m_stdErr;
		private int m_exitCode;
	}
}
