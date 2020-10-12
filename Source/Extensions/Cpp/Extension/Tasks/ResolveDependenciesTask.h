// <copyright file="ResolveToolsTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp
{
	/// <summary>
	/// The resolve dependencies build task that knows how to combine all previous state
	/// into the active state.
	/// </summary>
	export class ResolveDependenciesTask : public Memory::ReferenceCounted<Soup::Build::IBuildTask>
	{
	public:
		ResolveDependenciesTask() :
			_runBeforeList({ "Build" }),
			_runAfterList()
		{
		}

		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() const noexcept override final
		{
			return "ResolveDependencies";
		}

		/// <summary>
		/// Get the run before list
		/// </summary>
		const Soup::Build::IReadOnlyList<const char*>& GetRunBeforeList() const noexcept override final
		{
			return _runBeforeList;
		}

		/// <summary>
		/// Get the run after list
		/// </summary>
		const Soup::Build::IReadOnlyList<const char*>& GetRunAfterList() const noexcept override final
		{
			return _runAfterList;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		Soup::Build::ApiCallResult TryExecute(
			Soup::Build::IBuildState& buildState) noexcept override final
		{
			auto buildStateWrapper = Soup::Build::Utilities::BuildStateWrapper(buildState);

			try
			{
				Execute(buildStateWrapper);
				return Soup::Build::ApiCallResult::Success;
			}
			catch (const std::exception& ex)
			{
				buildStateWrapper.LogError(ex.what());
				return Soup::Build::ApiCallResult::Error;
			}
			catch(...)
			{
				// Unknown error
				return Soup::Build::ApiCallResult::Error;
			}
		}

	private:
		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(Soup::Build::Utilities::BuildStateWrapper& buildState)
		{
			auto activeState = buildState.GetActiveState();

			if (activeState.HasValue("Dependencies"))
			{
				auto dependenciesTable = activeState.GetValue("Dependencies").AsTable();
				auto buildTable = activeState.EnsureValue("Build").EnsureTable();

				for (auto& dependencyName : dependenciesTable.GetValueKeyList().CopyAsStringVector())
				{
					// Combine the core dependency build inputs for the core build task
					buildState.LogInfo("Combine Dependency: " + dependencyName);
					auto dependencyTable = dependenciesTable.GetValue(dependencyName).AsTable();

					if (dependencyTable.HasValue("Build"))
					{
						auto dependencyBuildTable = dependencyTable.GetValue("Build").AsTable();

						if (dependencyBuildTable.HasValue("ModuleDependencies"))
						{
							auto moduleDependencies = dependencyBuildTable
								.GetValue("ModuleDependencies")
								.AsList()
								.CopyAsStringVector();
							buildTable.EnsureValue("ModuleDependencies").EnsureList().Append(moduleDependencies);
						}

						if (dependencyBuildTable.HasValue("RuntimeDependencies"))
						{
							auto runtimeDependencies = dependencyBuildTable
								.GetValue("RuntimeDependencies")
								.AsList()
								.CopyAsStringVector();
							buildTable.EnsureValue("RuntimeDependencies").EnsureList().Append(runtimeDependencies);
						}

						if (dependencyBuildTable.HasValue("LinkDependencies"))
						{
							auto linkDependencies = dependencyBuildTable
								.GetValue("LinkDependencies")
								.AsList()
								.CopyAsStringVector();
							buildTable.EnsureValue("LinkDependencies").EnsureList().Append(linkDependencies);
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
