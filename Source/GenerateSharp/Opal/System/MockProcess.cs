// <copyright file="MockProcess.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace Opal.System;

/// <summary>
/// A mock process executable using system.
/// </summary>
public class MockProcess : IProcess
{
	// Input
	private readonly int _id;
	private readonly IList<string> _requests;

	// Result
	private bool _isFinished;
	private readonly string _stdOut;
	private readonly string _stdErr;
	private readonly int _exitCode;

	/// <summary>
	/// Initializes a new instance of the <see cref='MockProcess'/> class.
	/// </summary>
	/// <param name="id">The id.</param>
	/// <param name="requests">The requests.</param>
	/// <param name="exitCode">The exitCode.</param>
	/// <param name="stdOut">The standard out.</param>
	/// <param name="stdErr">The standard error.</param>
	public MockProcess(
		int id,
		IList<string> requests,
		int exitCode,
		string stdOut,
		string stdErr)
	{
		_id = id;
		_requests = requests;
		_isFinished = false;
		_stdOut = stdOut;
		_stdErr = stdErr;
		_exitCode = exitCode;
	}

	/// <summary>
	/// Execute a process for the provided.
	/// </summary>
	public void Start()
	{
		_requests.Add($"ProcessStart: {_id}");
	}

	/// <summary>
	/// Wait for the process to exit.
	/// </summary>
	public Task WaitForExitAsync()
	{
		_requests.Add($"WaitForExit: {_id}");

		_isFinished = true;
		return Task.CompletedTask;
	}

	/// <summary>
	/// Get the exit code.
	/// </summary>
	public int GetExitCode()
	{
		_requests.Add($"GetExitCode: {_id}");

		if (!_isFinished)
			throw new InvalidOperationException("Process has not finished.");
		return _exitCode;
	}

	/// <summary>
	/// Get the standard output.
	/// </summary>
	public string GetStandardOutput()
	{
		_requests.Add($"GetStandardOutput: {_id}");

		if (!_isFinished)
			throw new InvalidOperationException("Process has not finished.");
		return _stdOut;
	}

	/// <summary>
	/// Get the standard error output.
	/// </summary>
	public string GetStandardError()
	{
		_requests.Add($"GetStandardError: {_id}");

		if (!_isFinished)
			throw new InvalidOperationException("Process has not finished.");
		return _stdErr;
	}
}
