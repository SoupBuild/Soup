// <copyright file="MockProcessManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

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
		uniqueId = 1;
		requests = [];
		processFileName = new Path("C:/testlocation/SoupCMDTest.exe");
		executeResults = [];
	}

	/// <summary>
	/// Initializes a new instance of the <see cref='MockProcessManager'/> class.
	/// </summary>
	/// <param name="processFileName">The process file name.</param>
	public MockProcessManager(Path processFileName)
	{
		uniqueId = 1;
		requests = [];
		this.processFileName = processFileName;
		executeResults = [];
	}

	/// <summary>
	/// Create a result.
	/// </summary>
	/// <param name="command">The command.</param>
	/// <param name="output">The output.</param>
	public void RegisterExecuteResult(string command, string output)
	{
		executeResults.Add(command, output);
	}

	/// <summary>
	/// Get the load requests.
	/// </summary>
	public IReadOnlyList<string> Requests => requests;

	/// <summary>
	/// Gets the process file name.
	/// </summary>
	public Path GetCurrentProcessFileName()
	{
		requests.Add("GetCurrentProcessFileName");
		return processFileName;
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
		var id = uniqueId++;
		var message = $"CreateProcess: {id} [{workingDirectory}] {executable} {arguments}";
		requests.Add(message);

		// Check if there is a registered output
		if (executeResults.TryGetValue(message, out var output))
		{
			return new MockProcess(
				id,
				requests,
				0,
				output,
				string.Empty);
		}
		else
		{
			return new MockProcess(
				id,
				requests,
				0,
				string.Empty,
				string.Empty);
		}
	}
}