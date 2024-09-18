// <copyright file="MockProcess.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

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
		this.id = id;
		this.requests = requests;
		isFinished = false;
		this.stdOut = stdOut;
		this.stdErr = stdErr;
		this.exitCode = exitCode;
	}

	/// <summary>
	/// Execute a process for the provided.
	/// </summary>
	public void Start()
	{
		requests.Add($"ProcessStart: {id}");
	}

	/// <summary>
	/// Wait for the process to exit.
	/// </summary>
	public Task WaitForExitAsync()
	{
		requests.Add($"WaitForExit: {id}");

		isFinished = true;
		return Task.CompletedTask;
	}

	/// <summary>
	/// Get the exit code.
	/// </summary>
	public int GetExitCode()
	{
		requests.Add($"GetExitCode: {id}");

		if (!isFinished)
			throw new InvalidOperationException("Process has not finished.");
		return exitCode;
	}

	/// <summary>
	/// Get the standard output.
	/// </summary>
	public string GetStandardOutput()
	{
		requests.Add($"GetStandardOutput: {id}");

		if (!isFinished)
			throw new InvalidOperationException("Process has not finished.");
		return stdOut;
	}

	/// <summary>
	/// Get the standard error output.
	/// </summary>
	public string GetStandardError()
	{
		requests.Add($"GetStandardError: {id}");

		if (!isFinished)
			throw new InvalidOperationException("Process has not finished.");
		return stdErr;
	}
}
