// <copyright file="RecipeBuildRunner.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IEvaluateEngine.h"
#include "BuildConstants.h"
#include "BuildFailedException.h"
#include "PackageProvider.h"
#include "RecipeBuildArguments.h"
#include "FileSystemState.h"
#include "LocalUserConfig/LocalUserConfig.h"
#include "OperationGraph/OperationGraphManager.h"
#include "Utils/HandledException.h"
#include "ValueTable/ValueTableManager.h"
#include "Recipe/RecipeBuildStateConverter.h"
#include "PathList/PathListManager.h"

namespace Soup::Core
{
	/// <summary>
	/// The recipe build runner that knows how to perform the correct build for a recipe
	/// and all of its development and runtime dependencies
	/// </summary>
	export class RecipeBuildRunner
	{
	private:
		// Root arguments
		RecipeBuildArguments _arguments;

		// SDK Parameters
		ValueList _sdkParameters;
		std::vector<Path> _sdkReadAccess;

		// System Parameters
		ValueTable _hostBuildGlobalParameters;
		std::vector<Path> _systemReadAccess;

		// Shared Runtime
		RecipeCache& _recipeCache;
		PackageProvider& _packageProvider;
		IEvaluateEngine& _evaluateEngine;
		FileSystemState& _fileSystemState;

		// Mapping from name to build folder to check for duplicate names with different packages
		std::map<std::string, Path> _buildSet;
		std::map<std::string, Path> _hostBuildSet;

		// Mapping from package root path to the name and target folder to be used with dependencies parameters
		std::map<Path, RecipeBuildCacheState> _buildCache;
		std::map<Path, RecipeBuildCacheState> _hostBuildCache;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RecipeBuildRunner"/> class.
		/// </summary>
		RecipeBuildRunner(
			RecipeBuildArguments arguments,
			ValueList sdkParameters,
			std::vector<Path> sdkReadAccess,
			ValueTable hostBuildGlobalParameters,
			std::vector<Path> systemReadAccess,
			RecipeCache& recipeCache,
			PackageProvider& packageProvider,
			IEvaluateEngine& evaluateEngine,
			FileSystemState& fileSystemState) :
			_arguments(std::move(arguments)),
			_sdkParameters(std::move(sdkParameters)),
			_sdkReadAccess(std::move(sdkReadAccess)),
			_hostBuildGlobalParameters(std::move(hostBuildGlobalParameters)),
			_systemReadAccess(std::move(systemReadAccess)),
			_recipeCache(recipeCache),
			_packageProvider(packageProvider),
			_evaluateEngine(evaluateEngine),
			_fileSystemState(fileSystemState),
			_buildSet(),
			_hostBuildSet(),
			_buildCache(),
			_hostBuildCache()
		{
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(const PackageInfo& packageInfo)
		{
			// TODO: A scoped listener cleanup would be nice
			try
			{
				Log::EnsureListener().SetShowEventId(true);

				// Enable log event ids to track individual builds
				bool isHostBuild = false;
				BuildPackageAndDependencies(
					packageInfo,
					isHostBuild);

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
		void BuildPackageAndDependencies(
			const PackageInfo& packageInfo,
			bool isHostBuild)
		{
			for (auto dependencyType : packageInfo.Dependencies)
			{
				for (auto dependency : dependencyType.second)
				{
					auto dependencyId = dependency.second;

					// Load this package recipe
					auto dependencyPackageInfo = _packageProvider.GetPackageInfo(dependencyId);

					// Build all recursive dependencies
					bool isDependencyHostBuild = isHostBuild || dependencyType.first == "Build";
					BuildPackageAndDependencies(
						dependencyPackageInfo,
						isDependencyHostBuild);
				}
			}

			// Build the target recipe
			CheckBuildPackage(
				packageInfo,
				isHostBuild);
		}

		/// <summary>
		/// The core build that will either invoke the recipe builder directly
		/// or load a previous state
		/// </summary>
		void CheckBuildPackage(
			const PackageInfo& packageInfo,
			bool isHostBuild)
		{
			// TODO: RAII for active id
			try
			{
				Log::SetActiveId(packageInfo.ProjectId);
				auto languagePackageName = packageInfo.Recipe.GetLanguage().GetName() + "|" + packageInfo.Recipe.GetName();
				Log::Diag("Running Build: " + languagePackageName);

				// Select the correct build set to ensure that the different build properties 
				// required the same project to be build twice
				auto& buildSet = isHostBuild ? _hostBuildSet : _buildSet;
				auto findBuildState = buildSet.find(languagePackageName);
				if (findBuildState != buildSet.end())
				{
					// Verify the project name is unique
					if (findBuildState->second != packageInfo.PackageRoot)
					{
						Log::Error("Recipe with this name already exists: " + languagePackageName + " [" + packageInfo.PackageRoot.ToString() + "] [" + findBuildState->second.ToString() + "]");
						throw std::runtime_error("Recipe name not unique");
					}
					else
					{
						Log::Diag("Recipe already built: " + languagePackageName);
					}
				}
				else
				{
					// Run the required builds in process
					// This will break the circular requirements for the core build libraries
					RunBuild(
						packageInfo,
						isHostBuild);

					// Keep track of the packages we have already built
					auto insertBuildState = buildSet.emplace(
						languagePackageName,
						packageInfo.PackageRoot);

					// Replace the find iterator so it can be used to update the shared table state
					findBuildState = insertBuildState.first;
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
		void RunBuild(
			const PackageInfo& packageInfo,
			bool isHostBuild)
		{
			if (isHostBuild)
			{
				Log::Info("Host Build '" + packageInfo.Recipe.GetName() + "'");
			}
			else
			{
				Log::Info("Build '" + packageInfo.Recipe.GetName() + "'");
			}

			auto& globalParameters = isHostBuild ? _hostBuildGlobalParameters : _arguments.GlobalParameters;

			// Build up the expected output directory for the build to be used to cache state
			auto targetDirectory = RecipeBuildLocationManager::GetOutputDirectory(
				packageInfo.PackageRoot,
				packageInfo.Recipe,
				globalParameters,
				_recipeCache);
			auto soupTargetDirectory = targetDirectory + BuildConstants::GetSoupTargetDirectory();

			// Build up the child target directory set
			auto childTargetDirectorySets = GenerateChildDependenciesTargetDirectorySet(packageInfo, isHostBuild);
			auto& directChildTargetDirectories = childTargetDirectorySets.first;
			auto& recursiveChildTargetDirectories = childTargetDirectorySets.second;

			if (!_arguments.SkipGenerate)
			{
				RunIncrementalGenerate(
					packageInfo,
					targetDirectory,
					soupTargetDirectory,
					globalParameters,
					isHostBuild,
					directChildTargetDirectories,
					recursiveChildTargetDirectories);
			}

			if (!_arguments.SkipEvaluate)
			{
				RunEvaluate(targetDirectory, soupTargetDirectory);
			}

			// Cache the build state for upstream dependencies
			auto& buildCache = isHostBuild ? _hostBuildCache : _buildCache;
			buildCache.emplace(
				packageInfo.PackageRoot,
				RecipeBuildCacheState(
					packageInfo.Recipe.GetName(),
					std::move(targetDirectory),
					std::move(soupTargetDirectory),
					std::move(recursiveChildTargetDirectories)));
		}

		/// <summary>
		/// Run an incremental generate phase
		/// </summary>
		void RunIncrementalGenerate(
			const PackageInfo& packageInfo,
			const Path& targetDirectory,
			const Path& soupTargetDirectory,
			const ValueTable& globalParameters,
			bool isHostBuild,
			const std::set<Path>& directChildTargetDirectories,
			const std::set<Path>& recursiveChildTargetDirectories)
		{
			// Clone the global parameters
			auto parametersTable = ValueTable(globalParameters.GetValues());

			auto languageExtensionPath = _packageProvider.GetLanguageExtensionPath(packageInfo.Recipe);

			// Set the input parameters
			parametersTable.SetValue("LanguageExtensionPath", Value(languageExtensionPath.ToString()));
			parametersTable.SetValue("PackageDirectory", Value(packageInfo.PackageRoot.ToString()));
			parametersTable.SetValue("TargetDirectory", Value(targetDirectory.ToString()));
			parametersTable.SetValue("SoupTargetDirectory", Value(soupTargetDirectory.ToString()));
			parametersTable.SetValue("Dependencies", GenerateParametersDependenciesValueTable(packageInfo, isHostBuild));
			parametersTable.SetValue("SDKs", _sdkParameters);

			auto parametersFile = soupTargetDirectory + BuildConstants::GenerateParametersFileName();
			Log::Info("Check outdated parameters file: " + parametersFile.ToString());
			if (_arguments.ForceRebuild || IsOutdated(parametersTable, parametersFile))
			{
				Log::Info("Save Parameters file");
				ValueTableManager::SaveState(parametersFile, parametersTable);
			}

			// Initialize the read access with the shared global set
			auto evaluateAllowedReadAccess = std::vector<Path>();
			auto evaluateAllowedWriteAccess = std::vector<Path>();

			// Allow read access for all sdk directories
			std::copy(
				_sdkReadAccess.begin(),
				_sdkReadAccess.end(),
				std::back_inserter(evaluateAllowedReadAccess));

			// Allow read access for all recursive dependencies target directories
			std::copy(
				recursiveChildTargetDirectories.begin(),
				recursiveChildTargetDirectories.end(),
				std::back_inserter(evaluateAllowedReadAccess));

			// Allow reading from the package root (source input) and the target directory (intermediate output)
			evaluateAllowedReadAccess.push_back(packageInfo.PackageRoot);
			evaluateAllowedReadAccess.push_back(targetDirectory);

			// Only allow writing to the target directory
			evaluateAllowedWriteAccess.push_back(targetDirectory);

			auto readAccessFile = soupTargetDirectory + BuildConstants::GenerateReadAccessFileName();
			Log::Info("Check outdated read access file: " + readAccessFile.ToString());
			if (_arguments.ForceRebuild || IsOutdated(evaluateAllowedReadAccess, readAccessFile))
			{
				Log::Info("Save Read Access file");
				PathListManager::Save(readAccessFile, evaluateAllowedReadAccess);
			}

			auto writeAccessFile = soupTargetDirectory + BuildConstants::GenerateWriteAccessFileName();
			Log::Info("Check outdated write access file: " + writeAccessFile.ToString());
			if (_arguments.ForceRebuild || IsOutdated(evaluateAllowedWriteAccess, writeAccessFile))
			{
				Log::Info("Save Write Access file");
				PathListManager::Save(writeAccessFile, evaluateAllowedWriteAccess);
			}

			// Run the incremental generate
			auto generateGraph = OperationGraph();

			// Add the single root operation to perform the generate
			auto moduleName = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto moduleFolder = moduleName.GetParent();
			auto generateFolder = moduleFolder + Path("Generate/");
			auto generateExecutable = generateFolder + Path("Soup.Build.Generate.exe");
			OperationId generateOperatioId = 1;
			auto generateArguments = std::stringstream();
			generateArguments << soupTargetDirectory.ToString();
			auto generateOperation = OperationInfo(
				generateOperatioId,
				"Generate Phase: " + packageInfo.Recipe.GetLanguage().GetName() + "|" + packageInfo.Recipe.GetName(),
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
				generateOperatioId,
			});

			// Set Read and Write access fore the generate phase
			auto generateAllowedReadAccess = std::vector<Path>();
			auto generateAllowedWriteAccess = std::vector<Path>();

			// Allow read access to the generate executable folder, Windows and the DotNet install
			generateAllowedReadAccess.push_back(generateFolder);

			// Allow read from the language extension directory
			generateAllowedReadAccess.push_back(languageExtensionPath.GetParent());

			// TODO: Windows specific
			generateAllowedReadAccess.push_back(Path("C:/Windows/"));
			generateAllowedReadAccess.push_back(Path("C:/Program Files/dotnet/"));

			// Allow reading from the package root (source input) and the target directory (intermediate output)
			generateAllowedReadAccess.push_back(packageInfo.PackageRoot);
			generateAllowedReadAccess.push_back(targetDirectory);

			// Allow read access for all direct dependencies target directories and write to own targets
			// TODO: This is needed to get the shared properties, this may be better to do in process
			// and only allow read access to build dependencies.
			std::copy(
				directChildTargetDirectories.begin(),
				directChildTargetDirectories.end(),
				std::back_inserter(generateAllowedReadAccess));
			generateAllowedWriteAccess.push_back(targetDirectory);

			// Load the previous build graph if it exists and merge it with the new one
			auto generateGraphFile = soupTargetDirectory + BuildConstants::GetGenerateGraphFileName();
			TryMergeExisting(generateGraphFile, generateGraph);

			// Set the temporary folder under the target folder
			auto temporaryDirectory = targetDirectory + BuildConstants::GetTemporaryFolderName();

			// Evaluate the Generate phase
			_evaluateEngine.Evaluate(
				generateGraph,
				temporaryDirectory,
				generateAllowedReadAccess,
				generateAllowedWriteAccess);

			// Save the operation graph for future incremental builds
			OperationGraphManager::SaveState(generateGraphFile, generateGraph, _fileSystemState);
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

		bool IsOutdated(const std::vector<Path>& fileList, const Path& pathListFile)
		{
			// Load up the existing path list file and check if our state matches the previous
			// to ensure incremental builds function correctly
			auto previousFileList = std::vector<Path>();
			if (PathListManager::TryLoad(pathListFile, previousFileList))
			{
				return previousFileList != fileList;
			}
			else
			{
				return true;
			}
		}

		void RunEvaluate(
			const Path& targetDirectory,
			const Path& soupTargetDirectory)
		{
			// Load and run the previous stored state directly
			auto generateEvaluateGraphFile = soupTargetDirectory + BuildConstants::GenerateEvaluateOperationGraphFileName();
			auto evaluateResultGraphFile = soupTargetDirectory + BuildConstants::GetEvaluateResultGraphFileName();

			Log::Info("Loading generate evaluate operation graph");
			auto evaluateGraph = OperationGraph();
			if (!OperationGraphManager::TryLoadState(
				generateEvaluateGraphFile,
				evaluateGraph,
				_fileSystemState))
			{
				throw std::runtime_error("Missing generated operation graph for evaluate phase.");
			}

			if (!_arguments.ForceRebuild)
			{
				// Load the previous build graph if it exists and merge it with the new one
				TryMergeExisting(evaluateResultGraphFile, evaluateGraph);
			}

			// Set the temporary folder under the target folder
			auto temporaryDirectory = targetDirectory + BuildConstants::GetTemporaryFolderName();

			// Initialize the read access with the shared global set
			auto allowedReadAccess = std::vector<Path>();
			auto allowedWriteAccess = std::vector<Path>();

			// Allow read access from system runtime directories
			std::copy(
				_systemReadAccess.begin(),
				_systemReadAccess.end(),
				std::back_inserter(allowedReadAccess));

			// Allow read and write access to the temparary directory that is not explicitly declared
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
				_evaluateEngine.Evaluate(
					evaluateGraph,
					temporaryDirectory,
					allowedReadAccess,
					allowedWriteAccess);
			}
			catch(const BuildFailedException&)
			{
				Log::Info("Saving partial build state");
				OperationGraphManager::SaveState(
					evaluateResultGraphFile,
					evaluateGraph,
					_fileSystemState);
				throw;
			}

			Log::Info("Saving updated build state");
			OperationGraphManager::SaveState(
				evaluateResultGraphFile,
				evaluateGraph,
				_fileSystemState);

			Log::Info("Done");
		}

		/// <summary>
		/// Attempt to merge the existing operation graph if it exists
		/// </summary>
		void TryMergeExisting(
			const Path& operationGraphFile,
			OperationGraph& operationGraph)
		{
			Log::Diag("Loading previous operation graph");
			auto previousOperationGraph = OperationGraph();
			if (OperationGraphManager::TryLoadState(
				operationGraphFile,
				previousOperationGraph,
				_fileSystemState))
			{
				Log::Diag("Merge previous operation graph observed results");
				for (auto& activeOperationEntry : operationGraph.GetOperations())
				{
					auto& activeOperationInfo = activeOperationEntry.second;
					OperationInfo* previousOperationInfo = nullptr;
					if (previousOperationGraph.TryFindOperationInfo(activeOperationInfo.Command, previousOperationInfo))
					{
						activeOperationInfo.WasSuccessfulRun = previousOperationInfo->WasSuccessfulRun;
						activeOperationInfo.EvaluateTime = previousOperationInfo->EvaluateTime;
						activeOperationInfo.ObservedInput = previousOperationInfo->ObservedInput;
						activeOperationInfo.ObservedOutput = previousOperationInfo->ObservedOutput;
					}
				}
			}
			else
			{
				Log::Info("No valid previous build graph found");
			}
		}

		ValueTable GenerateParametersDependenciesValueTable(
			const PackageInfo& packageInfo,
			bool isHostBuild)
		{
			auto result = ValueTable();

			for (auto dependencyType : packageInfo.Dependencies)
			{
				auto& dependencyTypeTable = result.SetValue(dependencyType.first, Value(ValueTable())).AsTable();
				for (auto dependency : dependencyType.second)
				{
					auto dependencyId = dependency.second;
					auto& dependencyReference = dependency.first;

					// Load this package recipe
					auto dependencyPackageInfo = _packageProvider.GetPackageInfo(dependencyId);

					// Cache the build state for upstream dependencies
					bool isDependencyHostBuild = isHostBuild || dependencyType.first == "Build";
					auto& buildCache = isDependencyHostBuild ? _hostBuildCache : _buildCache;

					auto findBuildCache = buildCache.find(dependencyPackageInfo.PackageRoot);
					if (findBuildCache != buildCache.end())
					{
						auto& dependencyState = findBuildCache->second;
						dependencyTypeTable.SetValue(
							dependencyState.Name,
							Value(ValueTable({
								{
									"Reference",
									Value(dependencyReference.ToString())
								},
								{
									"TargetDirectory",
									Value(dependencyState.TargetDirectory.ToString())
								},
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
			}

			return result;
		}

		std::pair<std::set<Path>, std::set<Path>> GenerateChildDependenciesTargetDirectorySet(
			const PackageInfo& packageInfo,
			bool isHostBuild)
		{
			auto directDirectories = std::set<Path>();
			auto recursiveDirectories = std::set<Path>();
			for (auto dependencyType : packageInfo.Dependencies)
			{
				for (auto dependency : dependencyType.second)
				{
					auto dependencyId = dependency.second;

					// Load this package recipe
					auto dependencyPackageInfo = _packageProvider.GetPackageInfo(dependencyId);

					// Cache the build state for upstream dependencies
					bool isDependencyHostBuild = isHostBuild || dependencyType.first == "Build";
					auto& buildCache = isDependencyHostBuild ? _hostBuildCache : _buildCache;

					auto findBuildCache = buildCache.find(dependencyPackageInfo.PackageRoot);
					if (findBuildCache != buildCache.end())
					{
						// Combine the child dependency target and the recursive children
						auto& dependencyState = findBuildCache->second;
						directDirectories.insert(dependencyState.TargetDirectory);
						recursiveDirectories.insert(dependencyState.TargetDirectory);
						recursiveDirectories.insert(dependencyState.RecursiveChildTargetDirectorySet.begin(), dependencyState.RecursiveChildTargetDirectorySet.end());
					}
					else
					{
						Log::Error("Dependency does not exist in build cache: " + dependencyPackageInfo.PackageRoot.ToString());
						throw std::runtime_error("Dependency does not exist in build cache: " + dependencyPackageInfo.PackageRoot.ToString());
					}
				}
			}

			return std::make_pair(std::move(directDirectories), std::move(recursiveDirectories));
		}
	};
}
