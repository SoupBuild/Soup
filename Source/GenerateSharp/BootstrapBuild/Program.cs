// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Utilities;
using System.Diagnostics;
using ConsoleTraceListener = Opal.ConsoleTraceListener;
using EventTypeFilter = Opal.EventTypeFilter;
using Path = Opal.Path;

namespace Soup.Build.Discover;

public static class Program
{
	private static readonly Path WriteCommand = new Path("./writefile.exe");

	public static async Task<int> Main(string[] args)
	{
		try
		{
			var traceFlags =
				TraceEvents.Information |
				TraceEvents.HighPriority |
				TraceEvents.Critical |
				TraceEvents.Warning |
				TraceEvents.Error;
			Log.RegisterListener(new ConsoleTraceListener(new EventTypeFilter(traceFlags), false, false));
			LifetimeManager.RegisterSingleton<IFileSystem, RuntimeFileSystem>();

			LifetimeManager.RegisterSingleton<IProcessManager, RuntimeProcessManager>();

			if (args.Length != 1)
			{
				PrintUsage();
				return -1;
			}

			var targetPath = Path.Parse(args[0]);

			var fileSystemState = new FileSystemState();

			var evaluateGraphFile = targetPath + BuildConstants.EvaluateGraphFileName;
			if (!OperationGraphManager.TryLoadState(evaluateGraphFile, fileSystemState, out var evaluateGraph))
			{
				Log.Error($"Failed to load Operation Graph: {evaluateGraphFile}");
				throw new HandledException();
			}

			var remainingDependencyCounts = new Dictionary<OperationId, int>();
			await ExecuteOperationsAsync(
				evaluateGraph,
				evaluateGraph.RootOperationIds,
				remainingDependencyCounts);

			return 0;
		}
		catch (HandledException)
		{
			Log.Error("Error");
			return -1;
		}
	}

	private static async Task ExecuteOperationsAsync(
		OperationGraph evaluateGraph,
		IList<OperationId> operations,
		Dictionary<OperationId, int> remainingDependencyCounts)
	{
		foreach (var operationId in operations)
		{
			// Check if the operation was already a child from a different path
			// Only run the operation when all of its dependencies have completed
			var operationInfo = evaluateGraph.GetOperationInfo(operationId);
			var remainingCount = -1;
			if (remainingDependencyCounts.TryGetValue(operationId, out var currentRemainingCount))
			{
				remainingCount = --currentRemainingCount;
				remainingDependencyCounts[operationId] = remainingCount;
			}
			else
			{
				// Get the cached total count and store the active count in the lookup
				remainingCount = (int)operationInfo.DependencyCount - 1;
				remainingDependencyCounts.Add(operationId, remainingCount);
			}

			if (remainingCount == 0)
			{
				// Run the single operation
				Log.Info(operationInfo.Title);
				await ExecuteCommandAsync(
					operationInfo.Command);

				// Recursively build all of the operation children
				await ExecuteOperationsAsync(
					evaluateGraph,
					operationInfo.Children,
					remainingDependencyCounts);
			}
			else if (remainingCount < 0)
			{
				throw new InvalidOperationException("Remaining dependency count less than zero");
			}
			else
			{
				// This operation will be executed from a different path
			}
		}
	}

	public static async Task ExecuteCommandAsync(CommandInfo command)
	{
		if (command.Executable == WriteCommand)
		{
			await WriteFileAsync(command);
		}
		else
		{
			var arguments = string.Join(" ", command.Arguments);
			Log.Info($"Execute: [{command.WorkingDirectory}] {command.Executable} {arguments}");
			var processInfo = new ProcessStartInfo(command.Executable.ToString(), arguments)
			{
				WorkingDirectory = command.WorkingDirectory.ToString(),
				CreateNoWindow = true,
				UseShellExecute = false,
				RedirectStandardOutput = true,
				RedirectStandardError = true,
			};

			var process = Process.Start(processInfo) ??
				throw new InvalidOperationException("Operation failed to start");
			process.OutputDataReceived += (sender, args) => Log.Info(args.Data ?? "");
			process.BeginOutputReadLine();
			process.ErrorDataReceived += (sender, args) => Log.Error(args.Data ?? "");
			process.BeginErrorReadLine();
			await process.WaitForExitAsync();

			if (process.ExitCode != 0)
			{
				Log.Error($"Process failed: {process.ExitCode}");
				throw new HandledException();
			}
		}
	}

	private static async Task WriteFileAsync(CommandInfo command)
	{
		Log.Info("Execute InProcess WriteFile");

		// Pull out the file path argument
		if (command.Arguments.Count != 2)
		{
			Log.Error("WriteFile path argument malformed");
			throw new HandledException();
		}

		var fileName = new Path(command.Arguments[0]);
		Log.Info($"WritFile: {fileName}");

		var filePath = fileName.HasRoot ? fileName : command.WorkingDirectory + fileName;
		var content = command.Arguments[1];

		// Open the file to write to
		var file = LifetimeManager.Get<IFileSystem>().OpenWrite(filePath, true);
		using var writer = new StreamWriter(file.GetOutStream());

		await writer.WriteAsync(content);
	}

	private static void PrintUsage()
	{
		Log.Info("BootstrapBuild <SoupOutDir>");
	}
}