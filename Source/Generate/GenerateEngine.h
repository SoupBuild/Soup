// <copyright file="GenerateEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ExtensionManager.h"
#include "GenerateState.h"

namespace Soup::Core::Generate
{
	class GenerateEngine
	{
	public:
		static void Run(const Path& soupTargetDirectory)
		{
			// Initialize a shared File System State to cache file system access
			auto fileSystemState = FileSystemState();

			// Run all build operations in the correct order with incremental build checks
			Log::Diag("Build generate start: " + soupTargetDirectory.ToString());

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

			// Load the macros file
			auto macrosFile = soupTargetDirectory + BuildConstants::GenerateMacrosFileName();
			auto macros = std::map<std::string, std::string>();
			if (!StringMapManager::TryLoad(macrosFile, macros))
			{
				Log::Error("Failed to load the macros file: " + writeAccessFile.ToString());
				throw std::runtime_error("Failed to load macros file.");
			}

			// Setup a macro manager to resolve macros
			auto macroManager = MacroManager(fileSystemState, macros);

			// Get the required input state from the parameters
			std::optional<std::vector<Path>> languageExtensionScripts = std::nullopt;
			std::optional<Path> languageExtensionBundle = std::nullopt;
			auto languageExtensionResult = parametersState.find("LanguageExtension");
			if (languageExtensionResult != parametersState.end())
			{
				auto& languageExtensionTable = languageExtensionResult->second.AsTable();
				
				auto languageExtensionScriptsResult = languageExtensionTable.find("Scripts");
				if (languageExtensionScriptsResult != languageExtensionTable.end())
				{
					auto files = std::vector<Path>();
					for (auto& file : languageExtensionScriptsResult->second.AsList())
					{
						files.push_back(Path(file.AsString()));
					}

					languageExtensionScripts = std::move(files);
				}

				auto languageExtensionBundleResult = languageExtensionTable.find("Bundle");
				if (languageExtensionBundleResult != languageExtensionTable.end())
				{
					languageExtensionBundle = Path(languageExtensionBundleResult->second.AsString());
				}
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
				macroManager,
				languageExtensionScripts,
				languageExtensionBundle,
				dependenciesSharedState);

			// Start a new global state that is initialized to the recipe itself
			auto globalState = ValueTable();

			// Initialize the Recipe Root Table
			auto recipeState = RecipeBuildStateConverter::ConvertToBuildState(recipe.GetTable());
			globalState.emplace("Recipe", Value(std::move(recipeState)));

			// Initialize the Parameters Root Table
			globalState.emplace("Parameters", Value(std::move(parametersState)));

			// Initialize the Dependencies Root Table
			globalState.emplace("Dependencies", Value(std::move(dependenciesSharedState)));

			// Create a new build system for the requested build
			auto extensionManager = ExtensionManager();

			// Run all build extension register callbacks
			for (auto buildExtension : buildExtensionLibraries)
			{
				Log::Info("Loading Extension Script: " + buildExtension.first.ToString());
				if (buildExtension.second.has_value())
				{
					Log::Info("Bundles: " + buildExtension.second.value().ToString());
				}

				// Create a temporary Wren Host to discover all build extensions
				auto host = std::make_unique<GenerateHost>(buildExtension.first, buildExtension.second);
				host->InterpretMain();
				auto extensions = host->DiscoverExtensions();

				for (auto& extension : extensions)
				{
					extensionManager.RegisterExtensionTask(std::move(extension));
				}
			}

			// Evaluate the build extensions
			auto buildState = GenerateState(
				globalState,
				fileSystemState,
				readAccessList,
				writeAccessList);
			extensionManager.Execute(buildState);

			// Grab the build results
			auto generateInfoTable = buildState.GetGenerateInfo();
			auto evaluateGraph = buildState.BuildOperationGraph();
			auto sharedState = buildState.GetSharedState();

			// Save the runtime information so Soup View can easily visualize runtime
			auto generateInfoStateFile = soupTargetDirectory + BuildConstants::GenerateInfoFileName();
			Log::Info("Save Generate Info State: " + generateInfoStateFile.ToString());
			ValueTableManager::SaveState(generateInfoStateFile, generateInfoTable);

			// Save the operation graph so the evaluate phase can load it
			auto evaluateGraphFile = soupTargetDirectory + BuildConstants::EvaluateGraphFileName();
			OperationGraphManager::SaveState(evaluateGraphFile, evaluateGraph, fileSystemState);

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

		/// <summary>
		/// Generate the collection of build extensions
		/// </summary>
		static std::vector<std::pair<Path, std::optional<Path>>> GenerateBuildExtensionSet(
			MacroManager& macroManager,
			const std::optional<std::vector<Path>>& languageExtensionScripts,
			const std::optional<Path>& languageExtensionBundle,
			const ValueTable& dependenciesSharedState)
		{
			auto buildExtensionLibraries = std::vector<std::pair<Path, std::optional<Path>>>();

			// Run the RecipeBuild extension to inject core build tasks
			if (languageExtensionScripts.has_value())
			{
				for (auto& script : languageExtensionScripts.value())
				{
					buildExtensionLibraries.push_back(std::make_pair(script, languageExtensionBundle));
				}
			}

			// Check for any dynamic libraries in the shared state
			Log::Info("Check Extensions");
			auto buildDependenciesValue = dependenciesSharedState.find("Build");
			if (buildDependenciesValue != dependenciesSharedState.end())
			{
				for (auto& [dependencyKey, dependencyValue] : buildDependenciesValue->second.AsTable())
				{
					Log::Info("Check Build Dependency: " + dependencyKey);
					if (dependencyValue.IsTable())
					{
						auto& dependency = dependencyValue.AsTable();
						auto buildTableValue = dependency.find("Build");
						if (buildTableValue != dependency.end())
						{
							auto& buildTable = buildTableValue->second.AsTable();

							Path targetDirectory;
							auto targetDirectoryValue = buildTable.find("TargetDirectory");
							if (targetDirectoryValue != buildTable.end())
							{
								if (targetDirectoryValue->second.IsString())
								{
									auto macroTargetDirectory = targetDirectoryValue->second.AsString();
									targetDirectory = Path(macroManager.ResolveMacros(macroTargetDirectory));
								}
								else
								{
									Log::Warning("Build dependency TargetDirectory property was not a string.");
								}
							}

							std::optional<Path> moduleBundle = std::nullopt;
							auto moduleBundleValue = buildTable.find("ModuleBundle");
							if (moduleBundleValue != buildTable.end())
							{
								if (moduleBundleValue->second.IsString())
								{
									moduleBundle = Path(moduleBundleValue->second.AsString());
								}
								else
								{
									Log::Warning("Build dependency ModuleBundle property was not a string.");
								}
							}

							auto sourceValue = buildTable.find("Source");
							if (sourceValue != buildTable.end())
							{
								if (sourceValue->second.IsList())
								{
									auto files = std::vector<Path>();
									for (auto& file : sourceValue->second.AsList())
									{
										auto scriptFile = targetDirectory + Path(file.AsString());
										buildExtensionLibraries.push_back(
											std::make_pair(std::move(scriptFile), moduleBundle));
									}
								}
								else
								{
									Log::Warning("Build dependency Source property was not a list.");
								}
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
					else
					{
						Log::Warning("Dependency shared state was not a table.");
					}
				}
			}

			return buildExtensionLibraries;
		}
	};
}
