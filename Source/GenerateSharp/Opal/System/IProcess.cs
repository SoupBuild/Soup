// <copyright file="IProcess.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal.System;

/// <summary>
/// The process interface
/// Interface mainly used to allow for unit testing client code.
/// </summary>
public interface IProcess
{
	/// <summary>
	/// Execute a process for the provided.
	/// </summary>
	void Start();

	/// <summary>
	/// Wait for the process to exit.
	/// </summary>
	Task WaitForExitAsync();

	/// <summary>
	/// Get the exit code.
	/// </summary>
	int GetExitCode();

	/// <summary>
	/// Get the standard output.
	/// </summary>
	string GetStandardOutput();

	/// <summary>
	/// Get the standard error output.
	/// </summary>
	string GetStandardError();
}