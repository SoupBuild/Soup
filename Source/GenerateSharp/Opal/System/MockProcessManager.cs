// <copyright file="MockProcessManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Opal.System
{
	/// <summary>
	/// The mock process manager
	/// TODO: Move into test project
	/// </summary>
	public class MockProcessManager : IProcessManager
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='MockProcessManager'/> class.
		/// </summary>
		public MockProcessManager()
		{
			m_uniqueId = 1;
			_requests = new List<string>();
			_processFileName = new Path("C:/testlocation/SoupCMDTest.exe");
			_executeResults = new Dictionary<string, string>();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref='MockProcessManager'/> class.
		/// </summary>
		public MockProcessManager(Path processFileName)
		{
			m_uniqueId = 1;
			_requests = new List<string>();
			_processFileName = processFileName;
			_executeResults = new Dictionary<string, string>();
		}

		/// <summary>
		/// Create a result 
		/// </summary>
		public void RegisterExecuteResult(string command, string output)
		{
			_executeResults.Add(command, output);
		}

		/// <summary>
		/// Get the load requests
		/// </summary>
		public IReadOnlyList<string> GetRequests()
		{
			return _requests;
		}

		/// <summary>
		/// Gets the process file name
		/// </summary>
		public override Path GetCurrentProcessFileName()
		{
			_requests.Add("GetCurrentProcessFileName");
			return _processFileName;
		}

		/// <summary>
		/// Creates a process for the provided executable path
		/// </summary>
		public override IProcess CreateProcess(
			Path executable,
			string arguments,
			Path workingDirectory)
		{
			var id = m_uniqueId++;
			var message = $"CreateProcess: {id} [{workingDirectory}] {executable} {arguments}";
			_requests.Add(message);

			// Check if there is a registered output
			if (_executeResults.TryGetValue(message, out var output))
			{
				return new MockProcess(
					id,
					_requests,
					0,
					output,
					string.Empty);
			}
			else
			{
				return new MockProcess(
					id,
					_requests,
					0,
					string.Empty,
					string.Empty);
			}
		}

		private int m_uniqueId;
		private List<string> _requests;
		private Path _processFileName;
		private Dictionary<string, string> _executeResults;
	}
}
