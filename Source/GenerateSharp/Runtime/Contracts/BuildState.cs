// <copyright file="BuildState.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Runtime
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	public class BuildState : IBuildState
	{
		private OperationGraphGenerator graphGenerator;

		/// <summary>
		/// Initializes a new instance of the BuildState class
		/// </summary>
		public BuildState(
			ValueTable activeState,
			FileSystemState fileSystemState,
			IList<Path> readAccessList,
			IList<Path> writeAccessList)
		{
			this.ActiveState = activeState;
			this.SharedState = new ValueTable();

			this.graphGenerator = new OperationGraphGenerator(
				fileSystemState,
				readAccessList,
				writeAccessList);
		}

		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		public IValueTable ActiveState { get; init; }

		/// <summary>
		/// Get a reference to the shared state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		public IValueTable SharedState { get; init; }

		/// <summary>
		/// Create a build operation
		/// </summary>
		public void CreateOperation(
			string title,
			string executable,
			string arguments,
			string workingDirectory,
			string[] declaredInput,
			string[] declaredOutput)
		{
			this.graphGenerator.CreateOperation(
				title,
				new Path(executable),
				arguments,
				new Path(workingDirectory),
				declaredInput.Select(value => new Path(value)).ToList(),
				declaredOutput.Select(value => new Path(value)).ToList());
		}

		/// <summary>
		/// Log a message to the build system
		/// </summary>
		public void LogTrace(TraceLevel level, string message)
		{
			switch (level)
			{
			case TraceLevel.Error:
				Log.Error(message);
				break;
			case TraceLevel.Warning:
				Log.Warning(message);
				break;
			case TraceLevel.HighPriority:
				Log.HighPriority(message);
				break;
			case TraceLevel.Information:
				Log.Info(message);
				break;
			case TraceLevel.Debug:
				Log.Diag(message);
				break;
			default:
				throw new InvalidOperationException("Unknown trace level");
			}
		}

		public OperationGraph BuildOperationGraph()
		{
			return this.graphGenerator.BuildGraph();
		}
	}
}
