// <copyright file="RecipeBuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	export class RecipeBuildTask : public Memory::ReferenceCounted<Soup::Build::IBuildTask>
	{
	public:
		RecipeBuildTask() :
			_runBeforeList({ "Build" }),
			_runAfterList({ "ResolveTools" })
		{
		}

		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() const noexcept override final
		{
			return "RecipeBuild";
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
		/// Internal implementation that can throw
		/// </summary>
		void Execute(Soup::Build::Utilities::BuildStateWrapper& buildState)
		{
			auto rootTable = buildState.GetActiveState();
			auto recipeTable = rootTable.GetValue("Recipe").AsTable();
			auto buildTable = rootTable.EnsureValue("Build").EnsureTable();

			// Load the input properties
			auto compilerName = std::string(rootTable.GetValue("CompilerName").AsString().GetValue());
			auto packageRoot = Path(rootTable.GetValue("PackageRoot").AsString().GetValue());
			auto buildFlavor = std::string(rootTable.GetValue("BuildFlavor").AsString().GetValue());
			auto platformLibraries = rootTable.GetValue("PlatformLibraries").AsList().CopyAsPathVector();
			auto platformPreprocessorDefinitions = rootTable.GetValue("PlatformPreprocessorDefinitions").AsList().CopyAsStringVector();

			// Load Recipe properties
			auto name = std::string(recipeTable.GetValue("Name").AsString().GetValue());

			// Add any explicit platform dependencies that were added in the recipe
			if (recipeTable.HasValue("PlatformLibraries"))
			{
				for (auto value : recipeTable.GetValue("PlatformLibraries").AsList().CopyAsPathVector())
				{
					platformLibraries.push_back(std::move(value));
				}
			}

			// Add the dependency static library closure to link if targeting an executable or dynamic library
			std::vector<Path> linkLibraries = std::vector<Path>();
			if (recipeTable.HasValue("LinkLibraries"))
			{
				for (auto value : recipeTable.GetValue("LinkLibraries").AsList().CopyAsPathVector())
				{
					// If relative then resolve to working directory
					if (value.HasRoot())
					{
						linkLibraries.push_back(std::move(value));
					}
					else
					{
						linkLibraries.push_back(packageRoot + value);
					}
				}
			}

			// Add the dependency runtime dependencies closure if present
			if (recipeTable.HasValue("RuntimeDependencies"))
			{
				std::vector<Path> runtimeDependencies = std::vector<Path>();
				if (buildTable.HasValue("RuntimeDependencies"))
				{
					runtimeDependencies = buildTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector();
				}

				for (auto value : recipeTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector())
				{
					// If relative then resolve to working directory
					if (value.HasRoot())
					{
						runtimeDependencies.push_back(std::move(value));
					}
					else
					{
						runtimeDependencies.push_back(packageRoot + value);
					}
				}

				buildTable.EnsureValue("RuntimeDependencies").SetValuePathList(runtimeDependencies);
			}

			// Load the extra library paths provided to the build system
			auto libraryPaths = std::vector<Path>();

			// Combine the defines with the default set and the platform
			auto preprocessorDefinitions = std::vector<std::string>();
			if (recipeTable.HasValue("Defines"))
			{
				preprocessorDefinitions = recipeTable.GetValue("Defines").AsList().CopyAsStringVector();
			}

			preprocessorDefinitions.insert(
				preprocessorDefinitions.end(),
				platformPreprocessorDefinitions.begin(),
				platformPreprocessorDefinitions.end());
			preprocessorDefinitions.push_back("SOUP_BUILD");

			// Build up arguments to build this individual recipe
			auto binaryDirectory = rootTable.GetValue("BinaryDirectory").AsString().GetValue();
			auto objectDirectory = rootTable.GetValue("ObjectDirectory").AsString().GetValue();

			// Load the source files if present
			auto sourceFiles = std::vector<std::string>();
			if (recipeTable.HasValue("Source"))
			{
				sourceFiles = recipeTable.GetValue("Source").AsList().CopyAsStringVector();
			}

			// Set the correct optimization level for the requested flavor
			auto enableOptimization = false;
			bool generateSourceDebugInfo = false;
			if (buildFlavor == "debug")
			{
				// preprocessorDefinitions.push_back("DEBUG");
				generateSourceDebugInfo = true;
			}
			else if (buildFlavor == "release")
			{
				preprocessorDefinitions.push_back("RELEASE");
				enableOptimization = true;
			}
			else
			{
				buildState.LogError("Unknown build flavor type.");
				throw std::runtime_error("Unknown build flavors type.");
			}

			buildTable.EnsureValue("TargetName").SetValueString(name);
			buildTable.EnsureValue("WorkingDirectory").SetValueString(packageRoot.ToString());
			buildTable.EnsureValue("ObjectDirectory").SetValueString(objectDirectory);
			buildTable.EnsureValue("BinaryDirectory").SetValueString(binaryDirectory);
			buildTable.EnsureValue("EnableOptimization").SetValueBoolean(enableOptimization);
			buildTable.EnsureValue("GenerateSourceDebugInfo").SetValueBoolean(generateSourceDebugInfo);

			buildTable.EnsureValue("PreprocessorDefinitions").EnsureList().Append(preprocessorDefinitions);
			buildTable.EnsureValue("LibraryPaths").EnsureList().Append(libraryPaths);
			buildTable.EnsureValue("Source").EnsureList().Append(sourceFiles);

			// Convert the recipe type to the required build type
			auto targetType = Soup::CSharp::Compiler::BuildTargetType::Library;
			if (recipeTable.HasValue("Type"))
			{
				targetType = ParseType(recipeTable.GetValue("Type").AsString().GetValue());
			}

			buildTable.EnsureValue("TargetType").SetValueInteger(static_cast<int64_t>(targetType));
		}

		static Soup::CSharp::Compiler::BuildTargetType ParseType(std::string_view value)
		{
			if (value == "Executable")
				return Soup::CSharp::Compiler::BuildTargetType::Executable;
			else if (value == "Library")
				return Soup::CSharp::Compiler::BuildTargetType::Library;
			else
				throw std::runtime_error("Unknown target type value.");
		}

	private:
		Soup::Build::Utilities::ReadOnlyStringList _runBeforeList;
		Soup::Build::Utilities::ReadOnlyStringList _runAfterList;
	};
}
