// <copyright file="RuntimeProcess.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Diagnostics;
using System.Threading.Tasks;

namespace Opal.System;

/// <summary>
/// The shared runtime process executable using system.
/// </summary>
public class RuntimeProcess : IProcess
{
	// Input
	private readonly Path executable;
	private readonly string arguments;
	private readonly Path workingDirectory;

	// Runtime
	private Process? process;

	/// <summary>
	/// Initializes a new instance of the <see cref='RuntimeProcess'/> class.
	/// </summary>
	/// <param name="executable">The executable.</param>
	/// <param name="arguments">The arguments.</param>
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
			FileName = this.executable.ToString(),
			Arguments = this.arguments,
			WorkingDirectory = this.workingDirectory.ToString(),
			RedirectStandardOutput = true,
			RedirectStandardError = true,
		};

		this.process = Process.Start(processInfo);
		if (this.process is null)
			throw new InvalidOperationException("Failed to start process");
	}

	/// <summary>
	/// Wait for the process to exit.
	/// </summary>
	public Task WaitForExitAsync()
	{
		if (this.process is null)
			throw new InvalidOperationException("Cannot wait on process that is not running");
		return this.process.WaitForExitAsync();
	}

	/// <summary>
	/// Get the exit code.
	/// </summary>
	public int GetExitCode()
	{
		if (this.process is null)
			throw new InvalidOperationException("Cannot access process that does not exist");
		return this.process.ExitCode;
	}

	/// <summary>
	/// Get the standard output.
	/// </summary>
	public string GetStandardOutput()
	{
		if (this.process is null)
			throw new InvalidOperationException("Cannot access process that does not exist");
		return this.process.StandardOutput.ReadToEnd();
	}

	/// <summary>
	/// Get the standard error output.
	/// </summary>
	public string GetStandardError()
	{
		if (this.process is null)
			throw new InvalidOperationException("Cannot access process that does not exist");
		return this.process.StandardError.ReadToEnd();
	}
}