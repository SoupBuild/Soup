// <copyright file="BuildState.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Utilities;
using System;
using System.Linq;

namespace Soup.Build.Generate
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	internal class BuildState : IBuildState
	{
		/// <summary>
		/// Initializes a new instance of the BuildState class
		/// </summary>
		public BuildState(ValueTable activeState, FileSystemState fileSystemState)
		{
			ActiveState = activeState;
			SharedState = new ValueTable();

			_graphGenerator = new OperationGraphGenerator(fileSystemState);
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
			_graphGenerator.CreateOperation(
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

		/// <summary>
		/// Pull out the shared state
		/// </summary>
		ValueTable RetrieveSharedState()
		{
			return _sharedState;
		}

		OperationGraph BuildOperationGraph()
		{
			return _graphGenerator.BuildGraph();
		}

		private ValueTable _activeState;
		private ValueTable _sharedState;
		private OperationGraphGenerator _graphGenerator;
	};
}
