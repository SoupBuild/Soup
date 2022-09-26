// <copyright file="RecipeBuildRunner.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildEvaluateEngine.h"
#include "BuildConstants.h"
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
		RecipeBuildArguments _arguments;
		ValueTable _hostBuildGlobalParameters;

		ValueList _sdkParameters;

		PackageProvider _packageProvider;

		// Global read access
		std::vector<Path> _systemReadAccess;
		std::vector<Path> _sdkReadAccess;

		// Mapping from name to build folder to check for duplicate names with different packages
		std::map<std::string, Path> _buildSet;
		std::map<std::string, Path> _hostBuildSet;

		// Mapping from package root path to the name and target folder to be used with dependencies parameters
		std::map<Path, RecipeBuildCacheState> _buildCache;
		std::map<Path, RecipeBuildCacheState> _hostBuildCache;

		std::shared_ptr<FileSystemState> _fileSystemState;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RecipeBuildRunner"/> class.
		/// </summary>
		RecipeBuildRunner(RecipeBuildArguments arguments, LocalUserConfig localUserConfig) :
			_arguments(std::move(arguments)),
			_sdkParameters(),
			_packageProvider(),
			_buildSet(),
			_hostBuildSet(),
			_buildCache(),
			_hostBuildCache(),
			_systemReadAccess(),
			_sdkReadAccess(),
			_fileSystemState(std::make_shared<FileSystemState>())
		{
			_hostBuildGlobalParameters = ValueTable();

			// TODO: Default parameters need to come from the build extension
			auto flavor = std::string("release");
			auto system = std::string("win32");
			auto architecture = std::string("x64");
			auto compiler = std::string("MSVC");

			_hostBuildGlobalParameters.SetValue("Architecture", Value(std::string(architecture)));
			_hostBuildGlobalParameters.SetValue("Compiler", Value(std::string(compiler)));
			_hostBuildGlobalParameters.SetValue("Flavor", Value(std::string(flavor)));
			_hostBuildGlobalParameters.SetValue("System", Value(std::string(system)));

			// Allow read access from system directories
			// TODO: Windows specific
			_systemReadAccess.push_back(
				Path("C:/Windows/"));

			// Process the SDKs
			if (localUserConfig.HasSDKs())
			{
				Log::Info("Checking SDKs for read access");
				auto sdks = localUserConfig.GetSDKs();
				for (auto& sdk : sdks)
				{
					auto sdkName = sdk.GetName();
					Log::Info("Found SDK: " + sdkName);
					if (sdk.HasSourceDirectories())
					{
						for (auto& sourceDirectory : sdk.GetSourceDirectories())
						{
							Log::Info("  Read Access: " + sourceDirectory.ToString());
							_sdkReadAccess.push_back(sourceDirectory);
						}
					}

					auto sdkParameters = ValueTable();
					sdkParameters.SetValue("Name", Value(sdkName));
					if (sdk.HasProperties())
					{
						sdkParameters.SetValue("Properties", RecipeBuildStateConverter::ConvertToBuildState(sdk.GetProperties()));
					}

					_sdkParameters.GetValues().push_back(std::move(sdkParameters));
				}
			}
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(const Path& workingDirectory)
		{
			// Enable log event ids to track individual builds
			int projectId = 1;
			bool isHostBuild = false;

			_packageProvider.LoadClosure(workingDirectory);

			auto recipePath = workingDirectory + BuildConstants::RecipeFileName();
			Recipe recipe = {};
			if (!_packageProvider.TryGetRecipe(recipePath, recipe))
			{
				Log::Error("The target Recipe does not exist: " + recipePath.ToString());
				Log::HighPriority("Make sure the path is correct and try again");

				// Nothing we can do, exit
				throw HandledException(1123124);
			}

			// TODO: A scoped listener cleanup would be nice
			try
			{
				Log::EnsureListener().SetShowEventId(true);

				auto rootParentSet = std::set<std::string>();
				projectId = BuildRecipeAndDependencies(
					projectId,
					workingDirectory,
					recipe,
					isHostBuild,
					rootParentSet);

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
		int BuildRecipeAndDependencies(
			int projectId,
			const Path& workingDirectory,
			Recipe& recipe,
			bool isHostBuild,
			const std::set<std::string>& parentSet)
		{
			// Add current package to the parent set when building child dependencies
			auto activeParentSet = parentSet;
			activeParentSet.insert(std::string(recipe.GetName()));

			for (auto dependecyType : recipe.GetDependencyTypes())
			{
				// Same language as parent is implied
				auto implicitLanguage = recipe.GetLanguage().GetName();
				if (dependecyType == "Build")
				{
					// Build dependencies do not inherit the parent language
					// Instead, they default to C#
					implicitLanguage = "C#";
				}

				for (auto dependency : recipe.GetNamedDependencies(dependecyType))
				{
					// Load this package recipe
					auto packagePath = _packageProvider.GetPackageReferencePath(
						workingDirectory,
						dependency,
						implicitLanguage);
					auto packageRecipePath = packagePath + BuildConstants::RecipeFileName();
					Recipe dependencyRecipe = {};
					if (!_packageProvider.TryGetRecipe(packageRecipePath, dependencyRecipe))
					{
						if (dependency.IsLocal())
						{
							Log::Error("The dependency Recipe does not exist: " + packageRecipePath.ToString());
							Log::HighPriority("Make sure the path is correct and try again");
						}
						else
						{
							Log::Error("The dependency Recipe version has not been installed: " + dependency.ToString() + " -> " + packagePath.ToString() + " [" + workingDirectory.ToString() + "]");
							Log::HighPriority("Run `restore` and try again");
						}

						// Nothing we can do, exit
						throw HandledException(1234);
					}

					// Ensure we do not have any circular dependencies
					if (activeParentSet.contains(dependencyRecipe.GetName()))
					{
						Log::Error("Found circular dependency: " + recipe.GetName() + " -> " + dependencyRecipe.GetName());
						throw std::runtime_error("BuildRecipeAndDependencies: Circular dependency.");
					}

					// Build all recursive dependencies
					bool isDependencyHostBuild = isHostBuild || dependecyType == "Build";
					projectId = BuildRecipeAndDependencies(
						projectId,
						packagePath,
						dependencyRecipe,
						isDependencyHostBuild,
						activeParentSet);
				}
			}

			// Build the root recipe
			projectId = CheckBuildRecipe(
				projectId,
				workingDirectory,
				recipe,
				isHostBuild);

			// Return the updated project id after building all dependencies
			return projectId;
		}

		/// <summary>
		/// The core build that will either invoke the recipe builder directly
		/// or load a previous state
		/// </summary>
		int CheckBuildRecipe(
			int projectId,
			const Path& workingDirectory,
			Recipe& recipe,
			bool isHostBuild)
		{
			// TODO: RAII for active id
			try
			{
				Log::SetActiveId(projectId);
				auto languagePackageName = recipe.GetLanguage().GetName() + "|" + recipe.GetName();
				Log::Diag("Running Build: " + languagePackageName);

				// Select the correct build set to ensure that the different build properties 
				// required the same project to be build twice
				auto& buildSet = isHostBuild ? _hostBuildSet : _buildSet;
				auto findBuildState = buildSet.find(languagePackageName);
				if (findBuildState != buildSet.end())
				{
					// Verify the project name is unique
					if (findBuildState->second != workingDirectory)
					{
						Log::Error("Recipe with this name already exists: " + languagePackageName + " [" + workingDirectory.ToString() + "] [" + findBuildState->second.ToString() + "]");
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
						workingDirectory,
						recipe,
						isHostBuild);

					// Keep track of the packages we have already built
					auto insertBuildState = buildSet.emplace(
						languagePackageName,
						workingDirectory);

					// Replace the find iterator so it can be used to update the shared table state
					findBuildState = insertBuildState.first;

					// Move to the next build project id
					projectId++;
				}

				Log::SetActiveId(0);
			}
			catch(...)
			{
				Log::SetActiveId(0);
				throw;
			}

			return projectId;
		}

		/// <summary>
		/// Setup and run the individual components of the Generate and Evaluate phases for a given package
		/// </summary>
		void RunBuild(
			const Path& packageRoot,
			Recipe& recipe,
			bool isHostBuild)
		{
			if (isHostBuild)
			{
				Log::Info("Host Build '" + recipe.GetName() + "'");
			}
			else
			{
				Log::Info("Build '" + recipe.GetName() + "'");
			}

			auto& globalParameters = isHostBuild ? _hostBuildGlobalParameters : _arguments.GlobalParameters;

			// Build up the expected output directory for the build to be used to cache state
			auto targetDirectory = RecipeBuildLocationManager::GetOutputDirectory(
				packageRoot,
				recipe,
				globalParameters,
				_packageProvider);
			auto soupTargetDirectory = targetDirectory + BuildConstants::GetSoupTargetDirectory();

			// Build up the child target directory set
			auto childTargetDirectorySets = BuildChildDependenciesTargetDirectorySet(recipe, packageRoot, isHostBuild);
			auto& directChildTargetDirectories = childTargetDirectorySets.first;
			auto& recursiveChildTargetDirectories = childTargetDirectorySets.second;

			if (!_arguments.SkipGenerate)
			{
				RunIncrementalGenerate(
					recipe,
					packageRoot,
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
				packageRoot,
				RecipeBuildCacheState(
					recipe.GetName(),
					std::move(targetDirectory),
					std::move(soupTargetDirectory),
					std::move(recursiveChildTargetDirectories)));
		}

		/// <summary>
		/// Run an incremental generate phase
		/// </summary>
		void RunIncrementalGenerate(
			Recipe& recipe,
			const Path& packageDirectory,
			const Path& targetDirectory,
			const Path& soupTargetDirectory,
			const ValueTable& globalParameters,
			bool isHostBuild,
			const std::set<Path>& directChildTargetDirectories,
			const std::set<Path>& recursiveChildTargetDirectories)
		{
			// Clone the global parameters
			auto parametersTable = ValueTable(globalParameters.GetValues());

			auto languageExtensionPath = _packageProvider.GetLanguageExtensionPath(recipe);

			// Set the input parameters
			parametersTable.SetValue("LanguageExtensionPath", Value(languageExtensionPath.ToString()));
			parametersTable.SetValue("PackageDirectory", Value(packageDirectory.ToString()));
			parametersTable.SetValue("TargetDirectory", Value(targetDirectory.ToString()));
			parametersTable.SetValue("SoupTargetDirectory", Value(soupTargetDirectory.ToString()));
			parametersTable.SetValue("Dependencies", BuildParametersDependenciesValueTable(recipe, packageDirectory, isHostBuild));
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
			evaluateAllowedReadAccess.push_back(packageDirectory);
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
				"Generate Phase: " + recipe.GetLanguage().GetName() + "|" + recipe.GetName(),
				CommandInfo(
					packageDirectory,
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
			generateAllowedReadAccess.push_back(packageDirectory);
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
			auto generateGraphFile = soupTargetDirectory + GetGenerateGraphFileName();
			TryMergeExisting(generateGraphFile, generateGraph);

			// Set the temporary folder under the target folder
			auto temporaryDirectory = targetDirectory + GetTemporaryFolderName();

			// Evaluate the Generate phase
			auto evaluateGenerateEngine = BuildEvaluateEngine(
				_fileSystemState,
				generateGraph,
				std::move(temporaryDirectory),
				std::move(generateAllowedReadAccess),
				std::move(generateAllowedWriteAccess));
			evaluateGenerateEngine.Evaluate();

			// Save the operation graph for future incremental builds
			OperationGraphManager::SaveState(generateGraphFile, generateGraph, *_fileSystemState);
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
			auto evaluateResultGraphFile = soupTargetDirectory + GetEvaluateResultGraphFileName();

			Log::Info("Loading generate evaluate operation graph");
			auto evaluateGraph = OperationGraph();
			if (!OperationGraphManager::TryLoadState(
				generateEvaluateGraphFile,
				evaluateGraph,
				*_fileSystemState))
			{
				throw std::runtime_error("Missing generated operation graph for evaluate phase.");
			}

			if (!_arguments.ForceRebuild)
			{
				// Load the previous build graph if it exists and merge it with the new one
				TryMergeExisting(evaluateResultGraphFile, evaluateGraph);
			}

			// Set the temporary folder under the target folder
			auto temporaryDirectory = targetDirectory + GetTemporaryFolderName();

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
				auto evaluateEngine = BuildEvaluateEngine(
					_fileSystemState,
					evaluateGraph,
					std::move(temporaryDirectory),
					std::move(allowedReadAccess),
					std::move(allowedWriteAccess));
				evaluateEngine.Evaluate();
			}
			catch(const BuildFailedException&)
			{
				Log::Info("Saving partial build state");
				OperationGraphManager::SaveState(
					evaluateResultGraphFile,
					evaluateGraph,
					*_fileSystemState);
				throw;
			}

			Log::Info("Saving updated build state");
			OperationGraphManager::SaveState(
				evaluateResultGraphFile,
				evaluateGraph,
				*_fileSystemState);

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
				*_fileSystemState))
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

		ValueTable BuildParametersDependenciesValueTable(
			Recipe& recipe,
			const Path& workingDirectory,
			bool isHostBuild)
		{
			auto result = ValueTable();
			for (auto dependecyType : recipe.GetDependencyTypes())
			{
				// Same language as parent is implied
				auto implicitLanguage = recipe.GetLanguage().GetName();
				if (dependecyType == "Build")
				{
					// Build dependencies do not inherit the parent language
					// Instead, they default to C#
					implicitLanguage = "C#";
				}

				auto& dependencyTypeTable = result.SetValue(dependecyType, Value(ValueTable())).AsTable();
				for (auto dependency : recipe.GetNamedDependencies(dependecyType))
				{
					// Load this package recipe
					auto packagePath = _packageProvider.GetPackageReferencePath(
						workingDirectory,
						dependency,
						implicitLanguage);

					// Cache the build state for upstream dependencies
					bool isDependencyHostBuild = isHostBuild || dependecyType == "Build";
					auto& buildCache = isDependencyHostBuild ? _hostBuildCache : _buildCache;

					auto findBuildCache = buildCache.find(packagePath);
					if (findBuildCache != buildCache.end())
					{
						auto& dependencyState = findBuildCache->second;
						dependencyTypeTable.SetValue(
							dependencyState.Name,
							Value(ValueTable({
								{
									"Reference",
									Value(dependency.ToString())
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
						Log::Error("Dependency does not exist in build cache: " + packagePath.ToString());
						throw std::runtime_error("Dependency does not exist in build cache");
					}
				}
			}

			return result;
		}

		std::pair<std::set<Path>, std::set<Path>> BuildChildDependenciesTargetDirectorySet(
			Recipe& recipe,
			const Path& workingDirectory,
			bool isHostBuild)
		{
			auto directDirectories = std::set<Path>();
			auto recursiveDirectories = std::set<Path>();
			for (auto dependecyType : recipe.GetDependencyTypes())
			{
				// Same language as parent is implied
				auto implicitLanguage = recipe.GetLanguage().GetName();
				if (dependecyType == "Build")
				{
					// Build dependencies do not inherit the parent language
					// Instead, they default to C#
					implicitLanguage = "C#";
				}

				for (auto dependency : recipe.GetNamedDependencies(dependecyType))
				{
					// Load this package recipe
					auto packagePath = _packageProvider.GetPackageReferencePath(
						workingDirectory,
						dependency,
						implicitLanguage);

					// Cache the build state for upstream dependencies
					bool isDependencyHostBuild = isHostBuild || dependecyType == "Build";
					auto& buildCache = isDependencyHostBuild ? _hostBuildCache : _buildCache;

					auto findBuildCache = buildCache.find(packagePath);
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
						Log::Error("Dependency does not exist in build cache: " + packagePath.ToString());
						throw std::runtime_error("Dependency does not exist in build cache");
					}
				}
			}

			return std::make_pair(std::move(directDirectories), std::move(recursiveDirectories));
		}

	private:
		static Path GetGenerateGraphFileName()
		{
			static const auto value = Path("GenerateGraph.bog");
			return value;
		}

		static Path GetEvaluateResultGraphFileName()
		{
			static const auto value = Path("EvaluateResultGraph.bog");
			return value;
		}

		static Path GetTemporaryFolderName()
		{
			static const auto value = Path("temp/");
			return value;
		}
	};
}
