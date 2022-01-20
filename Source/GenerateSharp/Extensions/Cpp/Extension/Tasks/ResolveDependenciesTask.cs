// <copyright file="ResolveToolsTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Cpp
{
	using System.Collections.Generic;

	/// <summary>
	/// The resolve dependencies build task that knows how to combine all previous state
	/// into the active state.
	/// </summary>
	public class ResolveDependenciesTask : IBuildTask
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

		public ResolveDependenciesTask(IBuildState buildState, IValueFactory factory)
		{
			this.buildState = buildState;
			this.factory = factory;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public void Execute()
		{
			var activeState = this.buildState.ActiveState;

			if (activeState.TryGetValue("Dependencies", out var dependenciesValue))
			{
				var dependenciesTable = dependenciesValue.AsTable();
				if (dependenciesTable.TryGetValue("Runtime", out var runtimeValue))
				{
					var runtimeDependenciesTable = runtimeValue.AsTable();
					var buildTable = activeState.EnsureValueTable(this.factory, "Build");

					foreach (var dependencyName in runtimeDependenciesTable.Keys)
					{
						// Combine the core dependency build inputs for the core build task
						this.buildState.LogTrace(TraceLevel.Information, "Combine Runtime Dependency: " + dependencyName);
						var dependencyTable = runtimeDependenciesTable[dependencyName].AsTable();

						if (dependencyTable.TryGetValue("Build", out var buildValue))
						{
							var dependencyBuildTable = buildValue.AsTable();

							if (dependencyBuildTable.TryGetValue("ModuleDependencies", out var moduleDependenciesValue))
							{
								var moduleDependencies = moduleDependenciesValue.AsList();
								buildTable.EnsureValueList(this.factory, "ModuleDependencies").Append(moduleDependencies);
							}

							if (dependencyBuildTable.TryGetValue("RuntimeDependencies", out var runtimeDependenciesValue))
							{
								var runtimeDependencies = runtimeDependenciesValue.AsList();
								buildTable.EnsureValueList(this.factory, "RuntimeDependencies").Append(runtimeDependencies);
							}

							if (dependencyBuildTable.TryGetValue("LinkDependencies", out var linkDependenciesValue))
							{
								var linkDependencies = linkDependenciesValue.AsList();
								buildTable.EnsureValueList(this.factory, "LinkDependencies").Append(linkDependencies);
							}
						}
					}
				}
			}
		}
	}
}
