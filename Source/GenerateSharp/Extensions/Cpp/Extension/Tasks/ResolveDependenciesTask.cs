// <copyright file="ResolveToolsTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Cpp
{
	/// <summary>
	/// The resolve dependencies build task that knows how to combine all previous state
	/// into the active state.
	/// </summary>
	public class ResolveDependenciesTask : IBuildTask
	{
	public:
		ResolveDependenciesTask()
		{
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public void Execute()
		{
			var activeState = buildState.GetActiveState();

			if (activeState.HasValue("Dependencies"))
			{
				var dependenciesTable = activeState.GetValue("Dependencies").AsTable();
				if (dependenciesTable.HasValue("Runtime"))
				{
					var runtimeDependenciesTable = dependenciesTable.GetValue("Runtime").AsTable();
					var buildTable = activeState.EnsureValue("Build").EnsureTable();

					for (auto& dependencyName : runtimeDependenciesTable.GetValueKeyList().CopyAsStringVector())
					{
						// Combine the core dependency build inputs for the core build task
						buildState.LogInfo("Combine Runtime Dependency: " + dependencyName);
						var dependencyTable = runtimeDependenciesTable.GetValue(dependencyName).AsTable();

						if (dependencyTable.HasValue("Build"))
						{
							var dependencyBuildTable = dependencyTable.GetValue("Build").AsTable();

							if (dependencyBuildTable.HasValue("ModuleDependencies"))
							{
								var moduleDependencies = dependencyBuildTable
									.GetValue("ModuleDependencies")
									.AsList()
									.CopyAsStringVector();
								buildTable.EnsureValue("ModuleDependencies").EnsureList().Append(moduleDependencies);
							}

							if (dependencyBuildTable.HasValue("RuntimeDependencies"))
							{
								var runtimeDependencies = dependencyBuildTable
									.GetValue("RuntimeDependencies")
									.AsList()
									.CopyAsStringVector();
								buildTable.EnsureValue("RuntimeDependencies").EnsureList().Append(runtimeDependencies);
							}

							if (dependencyBuildTable.HasValue("LinkDependencies"))
							{
								var linkDependencies = dependencyBuildTable
									.GetValue("LinkDependencies")
									.AsList()
									.CopyAsStringVector();
								buildTable.EnsureValue("LinkDependencies").EnsureList().Append(linkDependencies);
							}
						}
					}
				}
			}
		}

	private:
		Soup::Build::Utilities::ReadOnlyStringList _runBeforeList;
		Soup::Build::Utilities::ReadOnlyStringList _runAfterList;
	};
}
