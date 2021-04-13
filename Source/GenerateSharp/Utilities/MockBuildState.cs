// <copyright file="MockBuildState.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	public class MockBuildState : IBuildState
	{
		/// <summary>
		/// Initializes a new instance of the <see cref="MockBuildState"/> class
		/// </summary>
		public MockBuildState()
		{
			ActiveState = new ValueTable();
			SharedState = new ValueTable();

			_buildOperations = new List<BuildOperation>();
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
			_buildOperations.Add(new BuildOperation(
				title,
				new Path(workingDirectory),
				new Path(executable),
				arguments,
				declaredInput.Select(value => new Path(value)).ToList(),
				declaredOutput.Select(value => new Path(value)).ToList()));
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

		public List<BuildOperation> GetBuildOperations()
		{
			return _buildOperations;
		}

		private List<BuildOperation> _buildOperations;
	}
}
