#pragma once

#include "WrenHost.h"

namespace Soup::Core::Generate
{
	class GenerateEngine
	{
	public:
		static void Run(const Path& soupTargetDirectory)
		{
			// Run all build operations in the correct order with incremental build checks
			Log::Diag("Build generate start: " + soupTargetDirectory.ToString());

			auto host = std::make_unique<WrenHost>();
			host->Run();

			// Load the parameters file
			auto parametersFile = soupTargetDirectory + BuildConstants::GenerateParametersFileName();
			auto parametersState = ValueTable();
			if (!ValueTableManager::TryLoadState(parametersFile, parametersState))
			{
				Log::Error("Failed to load the parameter file: " + parametersFile.ToString());
				throw std::runtime_error("Failed to load parameter file.");
			}

			// Load the read access file
			auto readAccessFile = soupTargetDirectory + BuildConstants::GenerateReadAccessFileName();
			auto readAccessList = std::vector<Path>();
			if (!PathListManager::TryLoad(readAccessFile, readAccessList))
			{
				Log::Error("Failed to load the read access file: " + readAccessFile.ToString());
				throw std::runtime_error("Failed to load read access file.");
			}

			// Load the write access file
			auto writeAccessFile = soupTargetDirectory + BuildConstants::GenerateWriteAccessFileName();
			auto writeAccessList = std::vector<Path>();
			if (!PathListManager::TryLoad(writeAccessFile, writeAccessList))
			{
				Log::Error("Failed to load the write access file: " + writeAccessFile.ToString());
				throw std::runtime_error("Failed to load write access file.");
			}

			// Get the required input state from the parameters
			auto languageExtensionResult = parametersState.find("LanguageExtensionPath");
			std::optional<Path> languageExtensionPath = std::nullopt;
			if (languageExtensionResult != parametersState.end())
			{
				languageExtensionPath = Path(languageExtensionResult->second.AsString());
			}

			auto packageDirectory = Path(parametersState.at("PackageDirectory").AsString());

			// Load the recipe file
			auto recipeFile = packageDirectory + BuildConstants::RecipeFileName();
			Recipe recipe;
			if (!RecipeExtensions::TryLoadRecipeFromFile(recipeFile, recipe))
			{
				Log::Error("Failed to load the recipe: " + recipeFile.ToString());
				throw std::runtime_error("Failed to load recipe.");
			}

			// Combine all the dependencies shared state
			auto dependenciesSharedState = LoadDependenciesSharedState(parametersState);

			// Generate the set of build extension libraries
			auto buildExtensionLibraries = GenerateBuildExtensionSet(
				languageExtensionPath,
				dependenciesSharedState);

			// Start a new active state that is initialized to the recipe itself
			auto activeState = ValueTable();

			// // Initialize the Recipe Root Table
			// auto recipeState = recipe.Document.ToBuildValue();
			// activeState.Add("Recipe", new Value(recipeState));

			// // Initialize the Parameters Root Table
			// activeState.Add("Parameters", new Value(parametersState));

			// // Initialize the Dependencies Root Table
			// activeState.Add("Dependencies", new Value(dependenciesSharedState));

			// // Keep the extension libraries open while running the build system
			// // to ensure their memory is kept alive
			// OperationGraph evaluateGraph;
			// IValueTable sharedState;

			// Create a new build system for the requested build
			// auto buildTaskManager = BuildTaskManager();

			// // Run all build extension register callbacks
			// for (auto buildExtension : buildExtensionLibraries)
			// {
			// 	auto library = LoadPlugin(buildExtension);
			// 	FindAllCommands(library, buildTaskManager);
			// }

			// // Run the build
			// auto buildState = BuildState(
			// 	activeState,
			// 	_fileSystemState,
			// 	readAccessList,
			// 	writeAccessList);
			// buildTaskManager.Execute(buildState, soupTargetDirectory);

			// // Grab the build results so the dependency libraries can be released asap
			// evaluateGraph = buildState.BuildOperationGraph();
			// sharedState = buildState.SharedState;

			// Save the operation graph so the evaluate phase can load it
			// auto evaluateGraphFile = soupTargetDirectory + BuildConstants.EvaluateGraphFileName;
			// OperationGraphManager.SaveState(evaluateGraphFile, evaluateGraph, _fileSystemState);

			// Save the shared state that is to be passed to the downstream builds
			// auto sharedStateFile = soupTargetDirectory + BuildConstants.GenerateSharedStateFileName;
			// ValueTableManager.SaveState(sharedStateFile, sharedState);
			Log::Diag("Build generate end");
		}

	private:
		/// <summary>
		/// Using the parameters to resolve the dependency output folders, load up the shared state table and
		/// combine them into a single value table to be used as input the this generate phase.
		/// </summary>
		static ValueTable LoadDependenciesSharedState(const ValueTable& parametersTable)
		{
			auto sharedDependenciesTable = ValueTable();
			auto dependencyTableValue = parametersTable.find("Dependencies");
			if (dependencyTableValue != parametersTable.end())
			{
				auto& dependenciesTable = dependencyTableValue->second.AsTable();
				for (auto dependencyTypeValue : dependenciesTable)
				{
					auto dependencyType = dependencyTypeValue.first;
					auto& dependencies = dependencyTypeValue.second.AsTable();
					for (auto dependencyValue : dependencies)
					{
						auto dependencyName = dependencyValue.first;
						auto& dependency = dependencyValue.second.AsTable();
						auto soupTargetDirectory = Path(dependency.at("SoupTargetDirectory").AsString());
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
						auto& typedDependenciesTable = EnsureValueTable(sharedDependenciesTable, dependencyType);
						typedDependenciesTable.emplace(
							dependencyName,
							Value(sharedStateTable));
					}
				}
			}

			return sharedDependenciesTable;
		}

		static ValueTable& EnsureValueTable(ValueTable& table, const std::string& key)
		{
			auto findResult = table.find(key);
			if (findResult != table.end())
			{
				return findResult->second.AsTable();
			}
			else
			{
				auto insertResult = table.emplace(key, Value(ValueTable()));
				return insertResult.first->second.AsTable();
			}
		}
		
		}

		/// <summary>
		/// Generate the collection of build extensions
		/// </summary>
		static std::vector<Path> GenerateBuildExtensionSet(
			const std::optional<Path>& languageExtensionPath,
			const ValueTable& dependenciesSharedState)
		{
			auto buildExtensionLibraries = std::vector<Path>();

			// Run the RecipeBuild extension to inject core build tasks
			if (languageExtensionPath.has_value())
			{
				buildExtensionLibraries.Add(languageExtensionPath.value());
			}

			// Check for any dynamic libraries in the shared state
			auto buildDependenciesValue = dependenciesSharedState.find("Build");
			if (buildDependenciesValue != dependenciesSharedState.end())
			{
				for (auto dependencyValue : buildDependenciesValue->second.AsTable())
				{
					auto& dependency = dependencyValue.Value.AsTable();
					auto buildTableValue = dependency.find("Build");
					if (buildTableValue != dependency.end())
					{
						auto& buildTable = buildTableValue->second.AsTable();
						auto targetFileValue = buildTable.find("TargetFile");
						if (targetFileValue != buildTable.end())
						{
							auto targetFile = Path(targetFileValue->second.AsString());
							buildExtensionLibraries.push_back(targetFile);
						}
						else
						{
							Log::Warning("Found build dependency with no target file.");
						}
					}
					else
					{
						Log::Warning("Found build dependency with no build table.");
					}
				}
			}

			return buildExtensionLibraries;
		}
	};
}
