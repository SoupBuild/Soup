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
	private:
		FileSystemState _fileSystemState;
	public:
		GenerateEngine() :
			_fileSystemState()
		{
		}

		void Run(const Path& soupTargetDirectory)
		{
			// Run all build operations in the correct order with incremental build checks
			Log::Diag("Build generate start: {}", soupTargetDirectory.ToString());

			// Load the input file
			auto inputFile = soupTargetDirectory + BuildConstants::GenerateInputFileName();
			auto inputTable = ValueTable();
			if (!ValueTableManager::TryLoadState(inputFile, inputTable))
			{
				Log::Error("Failed to load the input file: {}", inputFile.ToString());
				throw std::runtime_error("Failed to load input file.");
			}

			auto packageRoot = Path(inputTable.at("PackageRoot").AsString());
			auto userDataPath = Path(inputTable.at("UserDataPath").AsString());

			// Load the local user config and any sdk content
			auto sdkParameters = ValueList();
			auto sdkReadAccess = std::vector<Path>();
			LoadLocalUserConfig(userDataPath, sdkParameters, sdkReadAccess);

			// Load the recipe file
			auto recipeFile = packageRoot + BuildConstants::RecipeFileName();
			Recipe recipe;
			if (!RecipeExtensions::TryLoadRecipeFromFile(recipeFile, recipe))
			{
				Log::Error("Failed to load the recipe: {}", recipeFile.ToString());
				throw std::runtime_error("Failed to load recipe.");
			}

			// Load the input macro definition
			auto generateMacros = std::map<std::string, std::string>();
			for (auto& [key, value] : inputTable.at("GenerateMacros").AsTable())
				generateMacros.emplace(key, value.AsString());

			// Load the input macro definition
			auto generateSubGraphMacros = std::map<std::string, std::string>();
			for (auto& [key, value] : inputTable.at("GenerateSubGraphMacros").AsTable())
				generateSubGraphMacros.emplace(key, value.AsString());

			// Load the input read access list
			auto evaluateAllowedReadAccess = std::vector<Path>();
			for (auto& value : inputTable.at("EvaluateReadAccess").AsList())
				evaluateAllowedReadAccess.push_back(Path(value.AsString()));

			// Load the input write access list
			auto evaluateAllowedWriteAccess = std::vector<Path>();
			for (auto& value : inputTable.at("EvaluateWriteAccess").AsList())
				evaluateAllowedWriteAccess.push_back(Path(value.AsString()));

			// Load the input macro definition
			auto evaluateMacros = std::map<std::string, std::string>();
			for (auto& [key, value] : inputTable.at("EvaluateMacros").AsTable())
				evaluateMacros.emplace(key, value.AsString());

			// Allow read access for all sdk directories
			for (auto& value : sdkReadAccess)
				evaluateAllowedReadAccess.push_back(std::move(value));

			// Setup a macro manager to resolve macros
			auto generateMacroManager = MacroManager(generateMacros);
			auto generateSubGraphMacroManager = MacroManager(generateSubGraphMacros);
			auto evaluateMacroManager = MacroManager(evaluateMacros);

			// Combine all the dependencies shared state
			auto dependenciesSharedState = LoadDependenciesSharedState(
				generateSubGraphMacroManager,
				inputTable);

			// Generate the set of build extension libraries
			auto buildExtensionLibraries = GenerateBuildExtensionSet(
				generateMacroManager,
				dependenciesSharedState);

			// Start a new global state
			auto globalState = ValueTable();

			// Pass along the sdks
			globalState.emplace("SDKs", std::move(sdkParameters));

			// Initialize the Recipe Root Table
			auto recipeState = RecipeBuildStateConverter::ConvertToBuildState(recipe.GetTable());
			globalState.emplace("Recipe", Value(std::move(recipeState)));

			// Initialize input global state
			for (auto& [key, value] : inputTable.at("GlobalState").AsTable())
			{
				globalState.emplace(key, std::move(value));
			}

			// Merge the dependencies state
			auto& globalDependenciesTable = globalState.at("Dependencies").AsTable();
			for (auto& [dependencyType, dependencyTypeValue] : dependenciesSharedState)
			{
				auto& globalDependenciesType = globalDependenciesTable.at(dependencyType).AsTable();
				auto& sharedStateDependenciesType = dependencyTypeValue.AsTable();
				for (auto& [dependencyName, dependencySharedState] : sharedStateDependenciesType)
				{
					auto& globalDependency = globalDependenciesType.at(dependencyName).AsTable();
					globalDependency.emplace("SharedState", std::move(dependencySharedState));
				}
			}

			// Create a new build system for the requested build
			auto extensionManager = ExtensionManager();

			// Run all build extension register callbacks
			for (auto buildExtension : buildExtensionLibraries)
			{
				Log::Info("Loading Extension Script: {}", buildExtension.first.ToString());
				if (buildExtension.second.has_value())
				{
					Log::Info("Bundles: {}", buildExtension.second.value().ToString());
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
				_fileSystemState,
				evaluateAllowedReadAccess,
				evaluateAllowedWriteAccess);
			extensionManager.Execute(buildState);

			// Grab the build results
			auto generateInfoTable = buildState.GetGenerateInfo();
			auto evaluateGraph = buildState.BuildOperationGraph();
			auto sharedState = buildState.GetSharedState();

			// Save the runtime information so Soup View can easily visualize runtime
			auto generateInfoStateFile = soupTargetDirectory + BuildConstants::GenerateInfoFileName();
			Log::Info("Save Generate Info State: {}", generateInfoStateFile.ToString());
			ValueTableManager::SaveState(generateInfoStateFile, generateInfoTable);

			// Resolve macros before saving evaluate graph
			Log::Diag("Resolve build macros in evaluate graph");
			ResolveMacros(evaluateMacroManager, evaluateGraph);

			// Save the operation graph so the evaluate phase can load it
			auto evaluateGraphFile = soupTargetDirectory + BuildConstants::EvaluateGraphFileName();
			OperationGraphManager::SaveState(evaluateGraphFile, evaluateGraph, _fileSystemState);

			// Save the shared state that is to be passed to the downstream builds
			auto sharedStateFile = soupTargetDirectory + BuildConstants::GenerateSharedStateFileName();
			ValueTableManager::SaveState(sharedStateFile, sharedState);

			Log::Diag("Build generate end");
		}

	private:
		/// <summary>
		/// Load Local User Config and process any known state
		/// </summary>
		static void LoadLocalUserConfig(
			const Path& userDataPath,
			ValueList& sdkParameters,
			std::vector<Path>& sdkReadAccess)
		{
			// Load the local user config
			auto localUserConfigPath = userDataPath + BuildConstants::LocalUserConfigFileName();
			LocalUserConfig localUserConfig = {};
			if (!LocalUserConfigExtensions::TryLoadLocalUserConfigFromFile(localUserConfigPath, localUserConfig))
			{
				Log::Warning("Local User Config invalid");
			}

			// Process the SDKs
			if (localUserConfig.HasSDKs())
			{
				Log::Info("Checking SDKs for read access");
				auto sdks = localUserConfig.GetSDKs();
				for (auto& sdk : sdks)
				{
					auto sdkName = sdk.GetName();
					Log::Info("Found SDK: {}", sdkName);
					if (sdk.HasSourceDirectories())
					{
						for (auto& sourceDirectory : sdk.GetSourceDirectories())
						{
							Log::Info("  Read Access: {}", sourceDirectory.ToString());
							sdkReadAccess.push_back(sourceDirectory);
						}
					}

					auto sdkParameter = ValueTable();
					sdkParameter.emplace("Name", Value(sdkName));
					if (sdk.HasProperties())
					{
						sdkParameter.emplace(
							"Properties",
							RecipeBuildStateConverter::ConvertToBuildState(sdk.GetProperties()));
					}

					sdkParameters.push_back(std::move(sdkParameter));
				}
			}
		}

		/// <summary>
		/// Using the parameters to resolve the dependency output folders, load up the shared state table and
		/// combine them into a single value table to be used as input the this generate phase.
		/// </summary>
		static ValueTable LoadDependenciesSharedState(
			MacroManager& generateSubGraphMacroManager,
			const ValueTable& inputTable)
		{
			auto hackMacros = std::map<std::string, std::string>({
				{ "/(TARGET_Soup.Wren)/", "/(TARGET_mwasplund|Soup.Wren)/" },
				{ "/(TARGET_mkdir)/", "/(TARGET_mwasplund|mkdir)/" },
				{ "/(TARGET_copy)/", "/(TARGET_mwasplund|copy)/" },
			});
			auto hackMacroManager = MacroManager(hackMacros);

			auto sharedDependenciesTable = ValueTable();
			auto dependencyTableValue = inputTable.find("Dependencies");
			if (dependencyTableValue != inputTable.end())
			{
				auto& dependenciesTable = dependencyTableValue->second.AsTable();
				for (auto& [dependencyType, dependencyTypeValue] : dependenciesTable)
				{
					bool isSubGraphType = dependencyType == "Build" || dependencyType == "Tool";
					auto& dependencies = dependencyTypeValue.AsTable();
					for (auto& [dependencyName, dependencyValue] : dependencies)
					{
						auto& dependency = dependencyValue.AsTable();
						auto soupTargetDirectory = Path(dependency.at("SoupTargetDirectory").AsString());
						auto sharedStateFile = soupTargetDirectory + BuildConstants::GenerateSharedStateFileName();

						// Load the shared state file
						auto sharedStateTable = ValueTable();
						if (!ValueTableManager::TryLoadState(sharedStateFile, sharedStateTable))
						{
							Log::Error("Failed to load the shared state file: {}", sharedStateFile.ToString());
							throw std::runtime_error("Failed to load shared state file.");
						}

						// Hack
						sharedStateTable = ResolveMacros(hackMacroManager, sharedStateTable);

						// Ensure SubGraph macros are unique
						if (isSubGraphType)
						{
							sharedStateTable = ResolveMacros(generateSubGraphMacroManager, sharedStateTable);
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
			const ValueTable& dependenciesSharedState)
		{
			auto buildExtensionLibraries = std::vector<std::pair<Path, std::optional<Path>>>();

			// Check for any dynamic libraries in the shared state
			Log::Info("Check Extensions");
			auto buildDependenciesValue = dependenciesSharedState.find("Build");
			if (buildDependenciesValue != dependenciesSharedState.end())
			{
				for (auto& [dependencyKey, dependencyValue] : buildDependenciesValue->second.AsTable())
				{
					Log::Info("Check Build Dependency: {}", dependencyKey);
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
									auto macroModuleBundle = moduleBundleValue->second.AsString();
									moduleBundle = Path(macroManager.ResolveMacros(macroModuleBundle));
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

		void ResolveMacros(
			MacroManager& macroManager,
			OperationGraph& operationGraph)
		{
			for (auto& [operationId, operation] : operationGraph.GetOperations())
			{
				ResolveMacros(macroManager, operation.Command.Arguments);
				operation.Command.WorkingDirectory = macroManager.ResolveMacros(std::move(operation.Command.WorkingDirectory));
				operation.Command.Executable = macroManager.ResolveMacros(std::move(operation.Command.Executable));
				ResolveMacros(macroManager, operation.DeclaredInput);
				ResolveMacros(macroManager, operation.DeclaredOutput);
				ResolveMacros(macroManager, operation.ReadAccess);
				ResolveMacros(macroManager, operation.WriteAccess);
			}
		}

		void ResolveMacros(MacroManager& macroManager, std::vector<std::string>& value)
		{
			for(size_t i = 0; i < value.size(); i++)
			{
				auto resolvedValue = macroManager.ResolveMacros(value[i]);
				value[i] = std::move(resolvedValue);
			}
		}

		void ResolveMacros(MacroManager& macroManager, std::vector<FileId>& value)
		{
			for(size_t i = 0; i < value.size(); i++)
			{
				Path file = _fileSystemState.GetFilePath(value[i]);
				auto resolvedFile = macroManager.ResolveMacros(file);
				value[i] = _fileSystemState.ToFileId(resolvedFile);
			}
		}

		static ValueTable ResolveMacros(MacroManager& macroManager, const ValueTable& table)
		{
			auto result = ValueTable();
			for (auto& [key, value] : table)
			{
				// Resolve the key
				auto resolvedKey = macroManager.ResolveMacros(key);

				// Resolve the value
				auto resolvedValue = ResolveMacros(macroManager, value);

				result.emplace(resolvedKey, resolvedValue);
			}

			return result;
		}

		static ValueList ResolveMacros(MacroManager& macroManager, const ValueList& list)
		{
			auto result = ValueList();
			for (auto& value : list)
			{
				result.push_back(ResolveMacros(macroManager, value));
			}

			return result;
		}

		static Value ResolveMacros(MacroManager& macroManager, const Value& value)
		{
			switch (value.GetType())
			{
				case ValueType::Table:
					return Value(ResolveMacros(macroManager, value.AsTable()));
				case ValueType::List:
					return Value(ResolveMacros(macroManager, value.AsList()));
				case ValueType::String:
					return Value(macroManager.ResolveMacros(value.AsString()));
				case ValueType::Integer:
				case ValueType::Float:
				case ValueType::Boolean:
					// Nothing to resolve
					return value;
				default:
					throw std::runtime_error("Unknown ValueType");
			}
		}
	};
}
