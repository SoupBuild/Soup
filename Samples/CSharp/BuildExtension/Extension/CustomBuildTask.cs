// <copyright file="CustomBuildTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Samples.CSharp.BuildExtension.Extension
{
	using System.Collections.Generic;
	using Soup.Build;

	public class CustomBuildTask : IBuildTask
	{
		private IBuildState buildState;
		private IValueFactory factory;

		/// <summary>
		/// Get the run before list
		/// </summary>
		public static IReadOnlyList<string> RunBeforeList => new List<string>()
		{
			"BuildTask",
		};

		/// <summary>
		/// Get the run after list
		/// </summary>
		public static IReadOnlyList<string> RunAfterList => new List<string>()
		{
		};

		public CustomBuildTask(IBuildState buildState, IValueFactory factory)
		{
			this.buildState = buildState;
			this.factory = factory;
		}

		public void Execute()
		{
			this.buildState.LogTrace(TraceLevel.HighPriority, "Running Before Build!");

			// Get the build table
			var buildTable = this.buildState.ActiveState["Build"].AsTable();

			// As a sample of what a build extension can do we set a new
			// preprocessor definition to influence the build
			var preprocessorDefinitions = new List<string>()
			{
				"SPECIAL_BUILD",
			};
			buildTable.EnsureValueList(this.factory, "PreprocessorDefinitions").Append(this.factory, preprocessorDefinitions);
		}
	}
}
