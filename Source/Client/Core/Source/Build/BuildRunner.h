// <copyright file="BuildRunner.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "MacroManager.h"
#include "IEvaluateEngine.h"
#include "BuildConstants.h"
#include "BuildFailedException.h"
#include "PackageProvider.h"
#include "RecipeBuildArguments.h"
#include "RecipeBuildLocationManager.h"
#include "FileSystemState.h"
#include "LocalUserConfig/LocalUserConfig.h"
#include "OperationGraph/OperationGraphManager.h"
#include "OperationGraph/OperationResultsManager.h"
#include "Utils/HandledException.h"
#include "ValueTable/ValueTableManager.h"
#include "Recipe/RecipeBuildStateConverter.h"

namespace Soup::Core
{
	/// <summary>
	/// The build runner that knows how to perform the correct build for a recipe
	/// and all of its development and runtime dependencies
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class BuildRunner
	{
	private:
		// Root arguments
		const RecipeBuildArguments& _arguments;

		// SDK Parameters
		const ValueList& _sdkParameters;
		const std::vector<Path>& _sdkReadAccess;

		// System Parameters
		const std::vector<Path>& _systemReadAccess;

		// Shared Runtime
		RecipeCache& _recipeCache;
		PackageProvider& _packageProvider;
		IEvaluateEngine& _evaluateEngine;
		FileSystemState& _fileSystemState;
		RecipeBuildLocationManager& _locationManager;

		// Mapping from package id to the required information to be used with dependencies parameters
		std::map<PackageId, RecipeBuildCacheState> _buildCache;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="BuildRunner"/> class.
		/// </summary>
		BuildRunner(
			const RecipeBuildArguments& arguments,
			const ValueList& sdkParameters,
			const std::vector<Path>& sdkReadAccess,
			const std::vector<Path>& systemReadAccess,
			RecipeCache& recipeCache,
			PackageProvider& packageProvider,
			IEvaluateEngine& evaluateEngine,
			FileSystemState& fileSystemState,
			RecipeBuildLocationManager& locationManager) :
			_arguments(arguments),
			_sdkParameters(sdkParameters),
			_sdkReadAccess(sdkReadAccess),
			_systemReadAccess(systemReadAccess),
			_recipeCache(recipeCache),
			_packageProvider(packageProvider),
			_evaluateEngine(evaluateEngine),
			_fileSystemState(fileSystemState),
			_locationManager(locationManager),
			_buildCache()
		{
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute()
		{
			// TODO: A scoped listener cleanup would be nice
			try
			{
				Log::EnsureListener().SetShowEventId(true);

				// Enable log event ids to track individual builds
				auto& packageGraph = _packageProvider.GetRootPackageGraph();
				auto& packageInfo = _packageProvider.GetPackageInfo(packageGraph.RootPackageId);
				BuildPackageAndDependencies(packageGraph, packageInfo);

				Log::EnsureListener().SetShowEventId(false);
			}
			catch(...)
			{
				Log::EnsureListener().SetShowEventId(false);
				throw;
			}
		}

	private:
		/// <summary>
		/// Build the dependencies for the provided recipe recursively
		/// </summary>
		void BuildPackageAndDependencies(const PackageGraph& packageGraph, const PackageInfo& packageInfo)
		{
			for (auto& dependencyType : packageInfo.Dependencies)
			{
				for (auto& dependency : dependencyType.second)
				{
					if (dependency.IsSubGraph)
					{
						// Load this package recipe
						auto& dependencyPackageGraph = _packageProvider.GetPackageGraph(dependency.PackageGraphId);
						auto& dependencyPackageInfo = _packageProvider.GetPackageInfo(dependencyPackageGraph.RootPackageId);

						// Build all recursive dependencies
						BuildPackageAndDependencies(dependencyPackageGraph, dependencyPackageInfo);
					}
					else
					{
						// Load this package recipe
						auto& dependencyPackageInfo = _packageProvider.GetPackageInfo(dependency.PackageId);

						// Build all recursive dependencies
						BuildPackageAndDependencies(packageGraph, dependencyPackageInfo);
					}
				}
			}

			// Build the target recipe
			CheckBuildPackage(packageGraph, packageInfo);
		}

		/// <summary>
		/// The core build that will either invoke the recipe builder directly
		/// or load a previous state
		/// </summary>
		void CheckBuildPackage(const PackageGraph& packageGraph, const PackageInfo& packageInfo)
		{
			// TODO: RAII for active id
			try
			{
				Log::SetActiveId(packageInfo.Id);
				auto languagePackageName = packageInfo.Recipe.GetLanguage().GetName() + "|" + packageInfo.Recipe.GetName();
				Log::Diag("Running Build: " + languagePackageName);

				// Check if we already built this package down a different dependency path
				if (_buildCache.contains(packageInfo.Id))
				{
					Log::Diag("Recipe already built: " + languagePackageName);
				}
				else
				{
					// Run the required builds in process
					RunBuild(packageGraph, packageInfo);
				}

				Log::SetActiveId(0);
			}
			catch(...)
			{
				Log::SetActiveId(0);
				throw;
			}
		}

		/// <summary>
		/// Setup and run the individual components of the Generate and Evaluate phases for a given package
		/// </summary>
		void RunBuild(const PackageGraph& packageGraph, const PackageInfo& packageInfo)
		{
			Log::Info("Build '" + packageInfo.Recipe.GetName() + "'");

			// Build up the expected output directory for the build to be used to cache state
			auto macroPackageDirectory = Path("/(PACKAGE_" + packageInfo.Recipe.GetName() + ")/");
			auto macroTargetDirectory = Path("/(TARGET_" + packageInfo.Recipe.GetName() + ")/");
			auto realTargetDirectory = _locationManager.GetOutputDirectory(
				packageInfo.PackageRoot,
				packageInfo.Recipe,
				packageGraph.GlobalParameters,
				_recipeCache);
			auto soupTargetDirectory = realTargetDirectory + BuildConstants::SoupTargetDirectory();

			// Build up the child target directory set
			const auto& [directChildRealTargetDirectories, recursiveChildMockTargetDirectories, recursiveChildMacros] =
				GenerateChildDependenciesTargetDirectorySet(packageInfo);

			// Use the recursive set of macros along with the current targets
			auto macros = recursiveChildMacros;
			macros.emplace(macroPackageDirectory.ToString(), packageInfo.PackageRoot.ToString());
			macros.emplace(macroTargetDirectory.ToString(), realTargetDirectory.ToString());

			//////////////////////////////////////////////
			// SETUP
			/////////////////////////////////////////////

			// Load the previous operation graph and results if they exist
			Log::Info("Checking for existing Evaluate Operation Graph");
			auto evaluateGraphFile = soupTargetDirectory + BuildConstants::EvaluateGraphFileName();
			auto evaluateGraph = OperationGraph();
			auto evaluateResults = OperationResults();
			auto hasExistingGraph = OperationGraphManager::TryLoadState(
				evaluateGraphFile,
				evaluateGraph,
				_fileSystemState);
			
			if (hasExistingGraph)
			{
				Log::Info("Previous graph found");

				Log::Diag("Resolve build macros in previous graph");
				ResolveMacros(evaluateGraph, macros);

				Log::Info("Checking for existing Evaluate Operation Results");
				auto evaluateResultsFile = soupTargetDirectory + BuildConstants::EvaluateResultsFileName();
				if (OperationResultsManager::TryLoadState(
					evaluateResultsFile,
					evaluateResults,
					_fileSystemState))
				{
					Log::Info("Previous results found");
				}
				else
				{
					Log::Info("No previous results found");
				}
			}
			else
			{
				Log::Info("No previous graph found");
			}

			//////////////////////////////////////////////
			// GENERATE
			/////////////////////////////////////////////
			if (!_arguments.SkipGenerate)
			{
				auto ranGenerate = RunIncrementalGenerate(
					packageInfo,
					macroPackageDirectory,
					macroTargetDirectory,
					realTargetDirectory,
					soupTargetDirectory,
					packageGraph.GlobalParameters,
					directChildRealTargetDirectories,
					recursiveChildMockTargetDirectories,
					macros);

				//////////////////////////////////////////////
				// SETUP
				/////////////////////////////////////////////

				// Load the new Evaluate Operation Graph and merge any results that are still relevant
				if (ranGenerate)
				{
					Log::Info("Loading new Evaluate Operation Graph");
					auto updatedEvaluateGraph = OperationGraph();
					if (!OperationGraphManager::TryLoadState(
						evaluateGraphFile,
						updatedEvaluateGraph,
						_fileSystemState))
					{
						throw std::runtime_error("Missing required evaluate operation graph after generate evaluated.");
					}

					Log::Diag("Resolve build macros in new graph");
					ResolveMacros(updatedEvaluateGraph, macros);

					Log::Diag("Map previous operation graph observed results");
					auto updatedEvaluateResults = MergeOperationResults(
						evaluateGraph,
						evaluateResults,
						updatedEvaluateGraph);

					// Replace the previous operation graph and results
					evaluateGraph = std::move(updatedEvaluateGraph);
					evaluateResults = std::move(updatedEvaluateResults);
				}
			}

			//////////////////////////////////////////////
			// EVALUATE
			/////////////////////////////////////////////
			if (!_arguments.SkipEvaluate)
			{
				RunEvaluate(
					evaluateGraph,
					evaluateResults,
					realTargetDirectory,
					soupTargetDirectory);
			}

			// Cache the build state for upstream dependencies
			_buildCache.emplace(
				packageInfo.Id,
				RecipeBuildCacheState(
					packageInfo.Recipe.GetName(),
					std::move(macroTargetDirectory),
					std::move(realTargetDirectory),
					std::move(soupTargetDirectory),
					std::move(recursiveChildMockTargetDirectories),
					std::move(recursiveChildMacros)));
		}

		/// <summary>
		/// Run an incremental generate phase
		/// </summary>
		bool RunIncrementalGenerate(
			const PackageInfo& packageInfo,
			const Path& macroPackageDirectory,
			const Path& macroTargetDirectory,
			const Path& realTargetDirectory,
			const Path& soupTargetDirectory,
			const ValueTable& globalParameters,
			const std::set<Path>& directChildRealTargetDirectories,
			const std::set<Path>& recursiveChildMockTargetDirectories,
			const std::map<std::string, std::string>& macros)
		{
			// Clone the global parameters
			auto inputTable = ValueTable();

			// Pass along internal dependency information
			inputTable.emplace("Dependencies", GenerateInputDependenciesValueTable(packageInfo));

			// Set the language extension parameters
			if (packageInfo.LanguageExtensionScripts.has_value())
			{
				auto languageExtensionTable = ValueTable();

				auto languageExtensionScripts = ValueList();
				for (auto& file : packageInfo.LanguageExtensionScripts.value())
				{
					languageExtensionScripts.push_back(file.ToString());
				}

				languageExtensionTable.emplace("Scripts", std::move(languageExtensionScripts));

				if (packageInfo.LanguageExtensionBundle.has_value())
				{
					languageExtensionTable.emplace(
						"Bundle",
						Value(std::move(packageInfo.LanguageExtensionBundle.value().ToString())));
				}

				inputTable.emplace("LanguageExtension", std::move(languageExtensionTable));
			}

			// Setup input that will be included in the global state
			auto globalState = ValueTable();

			// Setup environment context generated by build runner
			auto context = ValueTable();
			context.emplace("PackageDirectory", macroPackageDirectory.ToString());
			context.emplace("TargetDirectory", macroTargetDirectory.ToString());
			globalState.emplace("Context", std::move(context));

			// Pass along the parameters
			globalState.emplace("Parameters", globalParameters);

			// Generate the dependencies input state
			globalState.emplace("Dependencies", GenerateParametersDependenciesValueTable(packageInfo));

			// Pass along the sdks
			globalState.emplace("SDKs", _sdkParameters);

			inputTable.emplace("GlobalState", std::move(globalState));

			// Build up the input state for the generate call
			auto evaluateAllowedReadAccess = ValueList();
			auto evaluateAllowedWriteAccess = ValueList();
			auto macroTable = ValueTable();

			// Allow read access for all sdk directories
			for (auto& value : _sdkReadAccess)
				evaluateAllowedReadAccess.push_back(value.ToString());

			// Allow read access for all recursive dependencies target directories
			for (auto& value : recursiveChildMockTargetDirectories)
				evaluateAllowedReadAccess.push_back(value.ToString());

			// Allow reading from the package root (source input) and the target directory (intermediate output)
			evaluateAllowedReadAccess.push_back(macroPackageDirectory.ToString());
			evaluateAllowedReadAccess.push_back(macroTargetDirectory.ToString());

			// Only allow writing to the target directory
			evaluateAllowedWriteAccess.push_back(macroTargetDirectory.ToString());

			for (auto& [key, value] : macros)
				macroTable.emplace(key, value);

			inputTable.emplace("PackageRoot", packageInfo.PackageRoot.ToString());
			inputTable.emplace("ReadAccess", std::move(evaluateAllowedReadAccess));
			inputTable.emplace("WriteAccess", std::move(evaluateAllowedWriteAccess));
			inputTable.emplace("Macros", std::move(macroTable));

			auto inputFile = soupTargetDirectory + BuildConstants::GenerateInputFileName();
			Log::Info("Check outdated generate input file: " + inputFile.ToString());
			if (IsOutdated(inputTable, inputFile))
			{
				Log::Info("Save Generate Input file");
				ValueTableManager::SaveState(inputFile, inputTable);
			}

			// Run the incremental generate
			auto generateGraph = OperationGraph();

			// Add the single root operation to perform the generate
			auto moduleName = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto generateFolder = moduleName.GetParent();

			#if defined(_WIN32)
			auto generateExecutable = generateFolder + Path("Soup.Generate.exe");
			#elif defined(__linux__)
			auto generateExecutable = Path("/home/mwasplund/dev/repos/Soup/Source/out/msbuild/bin/Soup.Build.Generate/Debug/net6.0/linux-x64/publish/") + Path("Soup.Build.Generate");
			#else
			#error "Unknown platform"
			#endif

			OperationId generateOperationId = 1;
			auto generateArguments = std::stringstream();
			generateArguments << soupTargetDirectory.ToString();
			auto generateOperation = OperationInfo(
				generateOperationId,
				"Generate: " + packageInfo.Recipe.GetLanguage().GetName() + "|" + packageInfo.Recipe.GetName(),
				CommandInfo(
					packageInfo.PackageRoot,
					generateExecutable,
					generateArguments.str()),
				{},
				{},
				{},
				{});
			generateOperation.DependencyCount = 1;
			generateGraph.AddOperation(std::move(generateOperation));

			// Set the Generate operation as the root
			generateGraph.SetRootOperationIds({
				generateOperationId,
			});

			// Set Read and Write access fore the generate phase
			auto generateAllowedReadAccess = std::vector<Path>();
			auto generateAllowedWriteAccess = std::vector<Path>();

			// Allow read access to the generate executable folder, Windows and the DotNet install
			generateAllowedReadAccess.push_back(generateFolder);

			// Allow read from the language extension directory
			if (packageInfo.LanguageExtensionScripts.has_value() && packageInfo.LanguageExtensionScripts.value().size() > 0)
			{
				generateAllowedReadAccess.push_back(packageInfo.LanguageExtensionScripts.value().at(0).GetParent());
			}

			// TODO: Windows specific
			generateAllowedReadAccess.push_back(Path("C:/Windows/"));
			generateAllowedReadAccess.push_back(Path("C:/Program Files/dotnet/"));

			// Allow reading from the package root (source input) and the target directory (intermediate output)
			generateAllowedReadAccess.push_back(packageInfo.PackageRoot);
			generateAllowedReadAccess.push_back(realTargetDirectory);

			// Allow read access for all direct dependencies target directories and write to own targets
			// TODO: This is needed to get the shared properties, this may be better to do in process
			// and only allow read access to build dependencies.
			std::copy(
				directChildRealTargetDirectories.begin(),
				directChildRealTargetDirectories.end(),
				std::back_inserter(generateAllowedReadAccess));
			generateAllowedWriteAccess.push_back(realTargetDirectory);

			// Load the previous build results if it exists
			Log::Info("Checking for existing Generate Operation Results");
			auto generateResultsFile = soupTargetDirectory + BuildConstants::GenerateResultsFileName();
			auto generateResults = OperationResults();
			if (OperationResultsManager::TryLoadState(
				generateResultsFile,
				generateResults,
				_fileSystemState))
			{
				Log::Info("Previous results found");
			}
			else
			{
				Log::Info("No previous results found");
			}

			// Set the temporary folder under the target folder
			auto temporaryDirectory = realTargetDirectory + BuildConstants::TemporaryFolderName();

			// Evaluate the Generate phase
			bool ranEvaluate = _evaluateEngine.Evaluate(
				generateGraph,
				generateResults,
				temporaryDirectory,
				generateAllowedReadAccess,
				generateAllowedWriteAccess);

			if (ranEvaluate)
			{
				// Save the generate operation results for future incremental builds
				OperationResultsManager::SaveState(generateResultsFile, generateResults, _fileSystemState);
			}

			return ranEvaluate;
		}

		void ResolveMacros(
			OperationGraph& operationGraph,
			const std::map<std::string, std::string>& macros)
		{
			auto macroManager = MacroManager(macros);
			for (auto& [operationId, operation] : operationGraph.GetOperations())
			{
				operation.Command.Arguments = macroManager.ResolveMacros(std::move(operation.Command.Arguments));
				operation.Command.WorkingDirectory = macroManager.ResolveMacros(std::move(operation.Command.WorkingDirectory));
				operation.Command.Executable = macroManager.ResolveMacros(std::move(operation.Command.Executable));
				ResolveMacros(macroManager, operation.DeclaredInput);
				ResolveMacros(macroManager, operation.DeclaredOutput);
				ResolveMacros(macroManager, operation.ReadAccess);
				ResolveMacros(macroManager, operation.WriteAccess);
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

		OperationResults MergeOperationResults(
			const OperationGraph& previousGraph,
			OperationResults& previousResults,
			const OperationGraph& updatedGraph)
		{
			auto updatedResults = OperationResults();
			for (auto& [operationId, updatedOperation] : updatedGraph.GetOperations())
			{
				// Check if the new operation command existing in the previous set too
				OperationId previousOperationId;
				if (previousGraph.TryFindOperation(updatedOperation.Command, previousOperationId))
				{
					// Check if there is an existing result for the previous operation
					OperationResult* previousOperationResult;
					if (previousResults.TryFindResult(previousOperationId, previousOperationResult))
					{
						// Move this result into the updated results store
						updatedResults.AddOrUpdateOperationResult(updatedOperation.Id, std::move(*previousOperationResult));
					}
				}
			}

			return updatedResults;
		}

		void RunEvaluate(
			const OperationGraph& evaluateGraph,
			OperationResults& evaluateResults,
			const Path& realTargetDirectory,
			const Path& soupTargetDirectory)
		{
			// Set the temporary folder under the target folder
			auto temporaryDirectory = realTargetDirectory + BuildConstants::TemporaryFolderName();

			// Initialize the read access with the shared global set
			auto allowedReadAccess = std::vector<Path>();
			auto allowedWriteAccess = std::vector<Path>();

			// Allow read access from system runtime directories
			std::copy(
				_systemReadAccess.begin(),
				_systemReadAccess.end(),
				std::back_inserter(allowedReadAccess));

			// Allow read and write access to the temporary directory that is not explicitly declared
			allowedReadAccess.push_back(temporaryDirectory);
			allowedWriteAccess.push_back(temporaryDirectory);

			// TODO: REMOVE - Allow read access from SDKs
			std::copy(
				_sdkReadAccess.begin(),
				_sdkReadAccess.end(),
				std::back_inserter(allowedReadAccess));

			// Ensure the temporary directories exists
			if (!System::IFileSystem::Current().Exists(temporaryDirectory))
			{
				Log::Info("Create Directory: " + temporaryDirectory.ToString());
				System::IFileSystem::Current().CreateDirectory2(temporaryDirectory);
			}

			try
			{
				// Evaluate the build
				auto ranEvaluate = _evaluateEngine.Evaluate(
					evaluateGraph,
					evaluateResults,
					temporaryDirectory,
					allowedReadAccess,
					allowedWriteAccess);

				if (ranEvaluate)
				{
					Log::Info("Saving updated build state");
					auto evaluateResultsFile = soupTargetDirectory + BuildConstants::EvaluateResultsFileName();
					OperationResultsManager::SaveState(evaluateResultsFile, evaluateResults, _fileSystemState);
				}
			}
			catch(const BuildFailedException&)
			{
				Log::Info("Saving partial build state");
				auto evaluateResultsFile = soupTargetDirectory + BuildConstants::EvaluateResultsFileName();
				OperationResultsManager::SaveState(evaluateResultsFile, evaluateResults, _fileSystemState);
				throw;
			}

			Log::Info("Done");
		}

		bool IsOutdated(const ValueTable& parametersTable, const Path& parametersFile)
		{
			// Load up the existing parameters file and check if our state matches the previous
			// to ensure incremental builds function correctly
			auto previousParametersState = ValueTable();
			if (ValueTableManager::TryLoadState(parametersFile, previousParametersState))
			{
				return previousParametersState != parametersTable;
			}
			else
			{
				return true;
			}
		}

		ValueTable GenerateInputDependenciesValueTable(
			const PackageInfo& packageInfo)
		{
			auto result = ValueTable();

			for (const auto& [dependencyTypeKey, dependencyTypeValue] : packageInfo.Dependencies)
			{
				auto dependencyTypeTable = ValueTable();
				for (auto& dependency : dependencyTypeValue)
				{
					// Load this package recipe
					auto dependencyPackageId = dependency.PackageId;
					if (dependency.IsSubGraph)
					{
						auto& dependencyPackageGraph = _packageProvider.GetPackageGraph(dependency.PackageGraphId);
						dependencyPackageId = dependencyPackageGraph.RootPackageId;
					}

					auto& dependencyPackageInfo = _packageProvider.GetPackageInfo(dependencyPackageId);

					// Grab the build state for upstream dependencies
					auto findBuildCache = _buildCache.find(dependencyPackageInfo.Id);
					if (findBuildCache != _buildCache.end())
					{
						auto& dependencyState = findBuildCache->second;
						dependencyTypeTable.emplace(
							dependencyState.Name,
							Value(ValueTable({
								{
									"SoupTargetDirectory",
									Value(dependencyState.SoupTargetDirectory.ToString())
								},
							})));
					}
					else
					{
						Log::Error("Dependency does not exist in build cache: " + dependencyPackageInfo.PackageRoot.ToString());
						throw std::runtime_error("Dependency does not exist in build cache: " + dependencyPackageInfo.PackageRoot.ToString());
					}
				}

				result.emplace(dependencyTypeKey, Value(std::move(dependencyTypeTable)));
			}

			return result;
		}

		ValueTable GenerateParametersDependenciesValueTable(
			const PackageInfo& packageInfo)
		{
			auto result = ValueTable();

			for (const auto& [dependencyTypeKey, dependencyTypeValue] : packageInfo.Dependencies)
			{
				auto dependencyTypeTable = ValueTable();
				for (auto& dependency : dependencyTypeValue)
				{
					// Load this package recipe
					auto dependencyPackageId = dependency.PackageId;
					if (dependency.IsSubGraph)
					{
						auto& dependencyPackageGraph = _packageProvider.GetPackageGraph(dependency.PackageGraphId);
						dependencyPackageId = dependencyPackageGraph.RootPackageId;
					}

					auto& dependencyPackageInfo = _packageProvider.GetPackageInfo(dependencyPackageId);

					// Grab the build state for upstream dependencies
					auto findBuildCache = _buildCache.find(dependencyPackageInfo.Id);
					if (findBuildCache != _buildCache.end())
					{
						auto& dependencyState = findBuildCache->second;
						auto contextTable = ValueTable({
							{
								"Reference",
								Value(dependency.OriginalReference.ToString())
							},
							{
								"TargetDirectory",
								Value(dependencyState.MacroTargetDirectory.ToString())
							},
						});

						dependencyTypeTable.emplace(
							dependencyState.Name,
							Value(ValueTable({
								{ "Context", std::move(contextTable) },
							})));
					}
					else
					{
						Log::Error("Dependency does not exist in build cache: " + dependencyPackageInfo.PackageRoot.ToString());
						throw std::runtime_error("Dependency does not exist in build cache: " + dependencyPackageInfo.PackageRoot.ToString());
					}
				}

				result.emplace(dependencyTypeKey, Value(std::move(dependencyTypeTable)));
			}

			return result;
		}

		std::tuple<std::set<Path>, std::set<Path>, std::map<std::string, std::string>> GenerateChildDependenciesTargetDirectorySet(
			const PackageInfo& packageInfo)
		{
			auto directRealDirectories = std::set<Path>();
			auto recursiveMacroDirectories = std::set<Path>();
			auto recursiveMacros = std::map<std::string, std::string>();
			for (auto& dependencyType : packageInfo.Dependencies)
			{
				for (auto& dependency : dependencyType.second)
				{
					// Load this package recipe
					auto dependencyPackageId = dependency.PackageId;
					if (dependency.IsSubGraph)
					{
						auto& dependencyPackageGraph = _packageProvider.GetPackageGraph(dependency.PackageGraphId);
						dependencyPackageId = dependencyPackageGraph.RootPackageId;
					}

					auto& dependencyPackageInfo = _packageProvider.GetPackageInfo(dependencyPackageId);

					// Grab the build state for upstream dependencies
					auto findBuildCache = _buildCache.find(dependencyPackageInfo.Id);
					if (findBuildCache != _buildCache.end())
					{
						// Combine the child dependency target and the recursive children
						auto& dependencyState = findBuildCache->second;
						directRealDirectories.insert(dependencyState.RealTargetDirectory);

						recursiveMacroDirectories.insert(dependencyState.MacroTargetDirectory);
						recursiveMacroDirectories.insert(
							dependencyState.RecursiveChildMacroTargetDirectorySet.begin(),
							dependencyState.RecursiveChildMacroTargetDirectorySet.end());

						recursiveMacros.emplace(dependencyState.MacroTargetDirectory.ToString(), dependencyState.RealTargetDirectory.ToString());
						recursiveMacros.insert(
							dependencyState.RecursiveChildMacros.begin(),
							dependencyState.RecursiveChildMacros.end());
					}
					else
					{
						Log::Error("Dependency does not exist in build cache: " + dependencyPackageInfo.PackageRoot.ToString());
						throw std::runtime_error("Dependency does not exist in build cache: " + dependencyPackageInfo.PackageRoot.ToString());
					}
				}
			}

			return std::make_tuple(
				std::move(directRealDirectories),
				std::move(recursiveMacroDirectories),
				std::move(recursiveMacros));
		}
	};
}
