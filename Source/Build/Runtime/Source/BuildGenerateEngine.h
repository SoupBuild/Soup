// <copyright file="BuildGenerateEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildConstants.h"
#include "Contracts/BuildState.h"
#include "Contracts/BuildSystem.h"
#include "Contracts/ValueTable.h"
#include "OperationGraph/OperationGraphGenerator.h"
#include "OperationGraph/OperationGraphManager.h"
#include "Recipe/RecipeBuildStateConverter.h"

namespace Soup::Build::Runtime
{
	/// <summary>
	/// The core build generate engine that knows how to run all of the required build Tasks to generate the
	/// Operation Graph.
	/// </summary>
	export class BuildGenerateEngine
	{
	public:
		BuildGenerateEngine() :
			_fileSystemState(std::make_shared<FileSystemState>())
		{
		}

		/// <summary>
		/// Execute the entire operation graph that is referenced by this build generate engine
		/// </summary>
		void Generate(
			const Path& soupTargetDirectory)
		{
			// Run all build operations in the correct order with incremental build checks
			Log::Diag("Build generate start");

			// Load the parameters file
			auto parametersFile = soupTargetDirectory + BuildConstants::GenerateParametersFileName();
			auto parametersState = ValueTable();
			if (!ValueTableManager::TryLoadState(parametersFile, parametersState))
			{
				Log::Error("Failed to load the parameter file: " + parametersFile.ToString());
				throw std::runtime_error("Failed to load parameter file.");
			}

			// Get the required input state from the parameters
			auto targetDirectory = Path(parametersState.GetValue("TargetDirectory").AsString().ToString());
			auto packageDirectory = Path(parametersState.GetValue("PackageDirectory").AsString().ToString());

			// Load the recipe file
			auto recipeFile = packageDirectory + BuildConstants::RecipeFileName();
			auto recipe = Recipe();
			if (!RecipeExtensions::TryLoadRecipeFromFile(recipeFile, recipe))
			{
				Log::Error("Failed to load the recipe: " + recipeFile.ToString());
				throw std::runtime_error("Failed to load recipe.");
			}

			// Combine all the dependencies shared state
			auto dependenciesSharedState = LoadDependenciesSharedState(parametersState);

			// Generate the set of build extension libraries
			auto buildExtensionLibraries = GenerateBuildExtensionSet(recipe, dependenciesSharedState);

			// Start a new active state that is initialized to the recipe itself
			auto activeState = ValueTable();

			// Initialize the Recipe Root Table
			auto recipeState = RecipeBuildStateConverter::ConvertToBuildState(recipe.GetTable());
			activeState.SetValue("Recipe", Value(std::move(recipeState)));

			// Initialize the Parameters Root Table
			activeState.SetValue("Parameters", Value(std::move(parametersState)));

			// Initialize the Dependencies Root Table
			activeState.SetValue("Dependencies", Value(std::move(dependenciesSharedState)));

			// Keep the extension libraries open while running the build system
			// to ensure their memory is kept alive
			auto activeExtensionLibraries = std::vector<std::shared_ptr<System::ILibrary>>();
			auto evaluateGraph = OperationGraph();
			auto sharedState = ValueTable();

			{
				// Create a new build system for the requested build
				auto buildSystem = BuildSystem();

				// Run all build extension register callbacks
				for (auto buildExtension : buildExtensionLibraries)
				{
					auto library = RunBuildExtension(buildExtension, buildSystem);
					activeExtensionLibraries.push_back(std::move(library));
				}

				// Run the build
				auto buildState = BuildState(activeState, _fileSystemState);
				buildSystem.Execute(buildState);

				// Grab the build results so the dependency libraries can be released asap
				evaluateGraph = buildState.BuildOperationGraph();
				sharedState = buildState.RetrieveSharedState();
			}

			// Load the previous build graph if it exists and merge it with the new one
			auto evaluateGraphFile = soupTargetDirectory + BuildConstants::EvaluateOperationGraphFileName();
			OperationGraphManager::TryMergeExisting(evaluateGraphFile, evaluateGraph, *_fileSystemState);

			// Save the operation graph so the evaluate phase can load it
			OperationGraphManager::SaveState(evaluateGraphFile, evaluateGraph, *_fileSystemState);

			// Save the shared state that is to be passed to the downstream builds
			auto sharedStateFile = soupTargetDirectory + BuildConstants::GenerateSharedStateFileName();
			ValueTableManager::SaveState(sharedStateFile, sharedState);
			Log::Diag("Build generate end");
		}

	private:
		/// <summary>
		/// Using the parameters to resolve the dependency output folders, load up the shared state table and
		/// combine them into a single value table to be used as input the this generate phase.
		/// </summary>
		ValueTable LoadDependenciesSharedState(const ValueTable& parametersTable)
		{
			auto sharedDependenciesTable = ValueTable();
			if (parametersTable.HasValue("ResolvedDependencies"))
			{
				auto& resolvedDependencies = parametersTable.GetValue("ResolvedDependencies").AsTable();
				for (auto dependencyTypeValue : resolvedDependencies.GetValues())
				{
					auto& dependencyType = dependencyTypeValue.first;
					auto& dependencies = dependencyTypeValue.second.AsTable();
					for (auto& dependencyValue : dependencies.GetValues())
					{
						auto& dependencyName = dependencyValue.first;
						auto& dependency = dependencyValue.second.AsTable();
						auto soupTargetDirectory = Path(dependency.GetValue("SoupTargetDirectory").AsString().ToString());
						auto sharedStateFile = soupTargetDirectory + BuildConstants::GenerateSharedStateFileName();

						// Load the shared state file
						auto sharedStateTable = ValueTable();
						if (!ValueTableManager::TryLoadState(sharedStateFile, sharedStateTable))
						{
							Log::Error("Failed to load the shared state file: " + sharedStateFile.ToString());
							throw std::runtime_error("Failed to load shared state file.");
						}

						// Add the shared build state from this child build into the correct
						// table depending on the build type
						auto& typedDependenciesTable = sharedDependenciesTable.EnsureValue(dependencyType).EnsureTable();
						typedDependenciesTable.SetValue(
							dependencyName,
							Value(std::move(sharedStateTable)));
					}
				}
			}

			return sharedDependenciesTable;
		}

		/// <summary>
		/// Generate the collection of build extensions
		/// </summary>
		std::vector<Path> GenerateBuildExtensionSet(
			const Recipe& recipe,
			const ValueTable& dependenciesSharedState)
		{
			auto buildExtensionLibraries = std::vector<Path>();

			// Run the RecipeBuild extension to inject core build tasks
			auto recipeBuildExtensionPath = Path();
			auto language = recipe.GetLanguage();
			if (language == "C++")
			{
				recipeBuildExtensionPath = Path("Soup.Cpp.dll");
			}
			else if (language == "C#")
			{
				recipeBuildExtensionPath = Path("Soup.CSharp.dll");
			}
			else
			{
				throw std::runtime_error("Unknown language.");
			}

			buildExtensionLibraries.push_back(std::move(recipeBuildExtensionPath));

			// Check for any dynamic libraries in the shared state
			if (dependenciesSharedState.HasValue("Build"))
			{
				for (auto dependencyValue : dependenciesSharedState.GetValue("Build").AsTable().GetValues())
				{
					auto& dependency = dependencyValue.second.AsTable();
					auto targetFile = Path(dependency.GetValue("TargetFile").AsString().ToString());

					if (System::IFileSystem::Current().Exists(targetFile))
					{
						buildExtensionLibraries.push_back(std::move(targetFile));
					}
				}
			}

			return buildExtensionLibraries;
		}

		std::shared_ptr<System::ILibrary> RunBuildExtension(
			Path& libraryPath,
			IBuildSystem& buildSystem)
		{
			Log::Diag("Running Build Extension: " + libraryPath.ToString());
			auto library = System::ILibraryManager::Current().LoadDynamicLibrary(libraryPath);
			auto function = (int(*)(IBuildSystem&))library->GetFunction(
				"RegisterBuildExtension");
			auto result = function(buildSystem);
			if (result != 0)
			{
				Log::Error("Build Extension Failed: " + std::to_string(result));
				throw std::runtime_error("Build Extension Failed.");
			}
			else
			{
				Log::Info("Build Extension Done");
			}

			// Keep the library open to ensure the registered tasks are not lost
			return library;
		}

	private:
		std::shared_ptr<FileSystemState> _fileSystemState;
	};
}
