// <copyright file="MockProcessManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Opal.System;

/// <summary>
/// The mock process manager
/// TODO: Move into test project.
/// </summary>
public class MockProcessManager : IProcessManager
{
	private int _uniqueId;
	private readonly List<string> _requests;
	private readonly Path _processFileName;
	private readonly Dictionary<string, string> _executeResults;

	/// <summary>
	/// Initializes a new instance of the <see cref='MockProcessManager'/> class.
	/// </summary>
	public MockProcessManager()
	{
		_uniqueId = 1;
		_requests = [];
		_processFileName = new Path("C:/testlocation/SoupCMDTest.exe");
		_executeResults = [];
	}

	/// <summary>
	/// Initializes a new instance of the <see cref='MockProcessManager'/> class.
	/// </summary>
	/// <param name="processFileName">The process file name.</param>
	public MockProcessManager(Path processFileName)
	{
		_uniqueId = 1;
		_requests = [];
		_processFileName = processFileName;
		_executeResults = [];
	}

	/// <summary>
	/// Create a result.
	/// </summary>
	/// <param name="command">The command.</param>
	/// <param name="output">The output.</param>
	public void RegisterExecuteResult(string command, string output)
	{
		_executeResults.Add(command, output);
	}

	/// <summary>
	/// Get the load requests.
	/// </summary>
	public IReadOnlyList<string> Requests => _requests;

	/// <summary>
	/// Gets the process file name.
	/// </summary>
	public Path GetCurrentProcessFileName()
	{
		_requests.Add("GetCurrentProcessFileName");
		return _processFileName;
	}

	/// <summary>
	/// Creates a process for the provided executable path.
	/// </summary>
	/// <param name="executable">The executable.</param>
	/// <param name="arguments">The arguments.</param>
	/// <param name="workingDirectory">The working directory.</param>
	public IProcess CreateProcess(
		Path executable,
		string arguments,
		Path workingDirectory)
	{
		var id = _uniqueId++;
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
}