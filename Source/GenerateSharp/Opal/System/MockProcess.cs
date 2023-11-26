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
	private int id;
	private IList<string> requests;

	// Result
	private bool isFinished;
	private string stdOut;
	private string stdErr;
	private int exitCode;

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
		this.isFinished = false;
		this.stdOut = stdOut;
		this.stdErr = stdErr;
		this.exitCode = exitCode;
	}

	/// <summary>
	/// Execute a process for the provided.
	/// </summary>
	public void Start()
	{
		this.requests.Add($"ProcessStart: {this.id}");
	}

	/// <summary>
	/// Wait for the process to exit.
	/// </summary>
	public Task WaitForExitAsync()
	{
		this.requests.Add($"WaitForExit: {this.id}");

		this.isFinished = true;
		return Task.CompletedTask;
	}

	/// <summary>
	/// Get the exit code.
	/// </summary>
	public int GetExitCode()
	{
		this.requests.Add($"GetExitCode: {this.id}");

		if (!this.isFinished)
			throw new InvalidOperationException("Process has not finished.");
		return this.exitCode;
	}

	/// <summary>
	/// Get the standard output.
	/// </summary>
	public string GetStandardOutput()
	{
		this.requests.Add($"GetStandardOutput: {this.id}");

		if (!this.isFinished)
			throw new InvalidOperationException("Process has not finished.");
		return this.stdOut;
	}

	/// <summary>
	/// Get the standard error output.
	/// </summary>
	public string GetStandardError()
	{
		this.requests.Add($"GetStandardError: {this.id}");

		if (!this.isFinished)
			throw new InvalidOperationException("Process has not finished.");
		return this.stdErr;
	}
}
