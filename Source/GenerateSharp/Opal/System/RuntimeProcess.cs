// <copyright file="RuntimeProcess.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Diagnostics;

namespace Opal.System;

/// <summary>
/// The shared runtime process executable using system.
/// </summary>
public class RuntimeProcess : IProcess
{
	// Input
	private readonly Path _executable;
	private readonly string _arguments;
	private readonly Path _workingDirectory;

	// Runtime
	private Process? _process;

	/// <summary>
	/// Initializes a new instance of the <see cref='RuntimeProcess'/> class.
	/// </summary>
	/// <param name="executable">The executable.</param>
	/// <param name="arguments">The arguemnts.</param>
	/// <param name="workingDirectory">The workingDirectory.</param>
	public RuntimeProcess(
		Path executable,
		string arguments,
		Path workingDirectory)
	{
		this.executable = executable;
		this.arguments = arguments;
		this.workingDirectory = workingDirectory;
	}

	/// <summary>
	/// Execute a process for the provided.
	/// </summary>
	public void Start()
	{
		var processInfo = new ProcessStartInfo()
		{
			FileName = executable.ToString(),
			Arguments = arguments,
			WorkingDirectory = workingDirectory.ToString(),
			RedirectStandardOutput = true,
			RedirectStandardError = true,
		};

		process = Process.Start(processInfo);
		if (process is null)
			throw new InvalidOperationException("Failed to start process");
	}

	/// <summary>
	/// Wait for the process to exit.
	/// </summary>
	public Task WaitForExitAsync()
	{
		if (process is null)
			throw new InvalidOperationException("Cannot wait on process that is not running");
		return process.WaitForExitAsync();
	}

	/// <summary>
	/// Get the exit code.
	/// </summary>
	public int GetExitCode()
	{
		if (process is null)
			throw new InvalidOperationException("Cannot access process that does not exist");
		return process.ExitCode;
	}

	/// <summary>
	/// Get the standard output.
	/// </summary>
	public string GetStandardOutput()
	{
		if (process is null)
			throw new InvalidOperationException("Cannot access process that does not exist");
		return process.StandardOutput.ReadToEnd();
	}

	/// <summary>
	/// Get the standard error output.
	/// </summary>
	public string GetStandardError()
	{
		if (process is null)
			throw new InvalidOperationException("Cannot access process that does not exist");
		return process.StandardError.ReadToEnd();
	}
}