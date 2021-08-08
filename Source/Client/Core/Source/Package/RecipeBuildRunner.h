// <copyright file="RecipeBuildRunner.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeBuildManager.h"
#include "RecipeBuildArguments.h"

namespace Soup::Build
{
	class RecipeBuildCacheState
	{
	public:
		RecipeBuildCacheState(
			std::string name,
			Path targetDirectory,
			Path soupTargetDirectory,
			std::set<Path> childTargetDirectorySet) :
			Name(std::move(name)),
			TargetDirectory(std::move(targetDirectory)),
			SoupTargetDirectory(std::move(soupTargetDirectory)),
			ChildTargetDirectorySet(std::move(childTargetDirectorySet))
		{
		}

		std::string Name;
		Path TargetDirectory;
		Path SoupTargetDirectory;
		std::set<Path> ChildTargetDirectorySet;
	};

	/// <summary>
	/// The recipe build runner that knows how to perform the correct build for a recipe
	/// and all of its development and runtime dependencies
	/// </summary>
	export class RecipeBuildRunner
	{
	public:
		static Path GetSoupTargetDirectory()
		{
			static const auto value = Path(".soup/");
			return value;
		}

		static Path GetOutputDirectory(
			const Path& packageRoot,
			Runtime::Recipe& recipe,
			const Runtime::ValueTable& globalParameters,
			bool isHostBuild,
			RecipeBuildManager& buildManager)
		{
			// Set the default output directory to be relative to the package
			auto rootOutput = packageRoot + Path("out/");

			// Add unique location for host builds
			if (isHostBuild)
			{
				rootOutput = rootOutput + Path("HostBuild/");
			}

			// Check for root recipe file with overrides
			Path rootRecipeFile;
			if (RootRecipeExtensions::TryFindRootRecipeFile(packageRoot, rootRecipeFile))
			{
				Log::Info("Found Root Recipe: '" + rootRecipeFile.ToString() + "'");
				RootRecipe rootRecipe;
				if (!buildManager.TryGetRootRecipe(rootRecipeFile, rootRecipe))
				{
					// Nothing we can do, exit
					Log::Error("Failed to load the root recipe file: " + rootRecipeFile.ToString());
					throw HandledException(222);
				}

				// Check if there was a root output set
				if (rootRecipe.HasOutputRoot())
				{
					// Relative to the root recipe file itself
					rootOutput = rootRecipe.GetOutputRoot();

					// Add unique location for host builds
					if (isHostBuild)
					{
						rootOutput = rootOutput + Path("HostBuild/");
					}

					// Add the language sub folder
					rootOutput = rootOutput + Path(recipe.GetLanguage() + "/");

					// Add the unique recipe name
					rootOutput = rootOutput + Path(recipe.GetName() + "/");

					// Ensure there is a root relative to the file itself
					if (!rootOutput.HasRoot())
					{
						rootOutput = rootRecipeFile.GetParent() + rootOutput;
					}

					Log::Info("Override root output: " + rootOutput.ToString());
				}
			}

			// Add unique folder name for parameters
			auto parametersStream = std::stringstream();
			Runtime::ValueTableWriter::Serialize(globalParameters, parametersStream);
			auto hashParameters = Runtime::Sha3_256::HashBase64(parametersStream.str());
			auto uniqueParametersFolder = Path(hashParameters + "/");
			rootOutput = rootOutput + uniqueParametersFolder;

			return rootOutput;
		}

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RecipeBuildRunner"/> class.
		/// </summary>
		RecipeBuildRunner(RecipeBuildArguments arguments, Runtime::LocalUserConfig localUserConfig) :
			_arguments(std::move(arguments)),
			_buildManager(),
			_buildSet(),
			_hostBuildSet(),
			_buildCache(),
			_hostBuildCache(),
			_globalReadAccess(),
			_fileSystemState(std::make_shared<Runtime::FileSystemState>())
		{
			_hostBuildGlobalParameters = Runtime::ValueTable();
			
			auto flavor = std::string("release");
			auto system = std::string("win32");
			auto architecture = std::string("x64");
			auto compiler = std::string("MSVC");

			_hostBuildGlobalParameters.SetValue("Architecture", Runtime::Value(std::string(architecture)));
			_hostBuildGlobalParameters.SetValue("Compiler", Runtime::Value(std::string(compiler)));
			_hostBuildGlobalParameters.SetValue("Flavor", Runtime::Value(std::string(flavor)));
			_hostBuildGlobalParameters.SetValue("System", Runtime::Value(std::string(system)));

			// Allow reading from system
			_globalReadAccess.push_back(
				Path("C:/Windows/"));

			// Allow read access for sdks
			if (localUserConfig.HasSDKs())
			{
				Log::Info("Checking SDKs for read access");
				auto sdks = localUserConfig.GetSDKs();
				for (auto& sdk : sdks)
				{
					Log::Info("Found SDK: " + sdk.GetName());
					if (sdk.HasSourceDirectories())
					{
						for (auto& sourceDirectory : sdk.GetSourceDirectories())
						{
							Log::Info("  Read Access: " + sourceDirectory.ToString());
							_globalReadAccess.push_back(sourceDirectory);
						}
					}
				}
			}
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(const Path& workingDirectory)
		{
			// TODO: A scoped listener cleanup would be nice
			try
			{
				// Enable log event ids to track individual builds
				int projectId = 1;
				bool isHostBuild = false;
				Log::EnsureListener().SetShowEventId(true);

				auto recipePath = workingDirectory + Runtime::BuildConstants::RecipeFileName();
				Runtime::Recipe recipe = {};
				if (!_buildManager.TryGetRecipe(recipePath, recipe))
				{
					Log::Error("The target Recipe does not exist: " + recipePath.ToString());
					Log::HighPriority("Make sure the path is correct and try again");

					// Nothing we can do, exit
					throw HandledException(1123124);
				}

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
			Runtime::Recipe& recipe,
			bool isHostBuild,
			const std::set<std::string>& parentSet)
		{
			// Add current package to the parent set when building child dependencies
			auto activeParentSet = parentSet;
			activeParentSet.insert(std::string(recipe.GetName()));

			auto knownDependecyTypes = std::array<std::string_view, 3>({
				"Runtime",
				"Test",
				"Build",
			});

			for (auto knownDependecyType : knownDependecyTypes)
			{
				if (recipe.HasNamedDependencies(knownDependecyType))
				{
					for (auto dependency : recipe.GetNamedDependencies(knownDependecyType))
					{
						// Load this package recipe
						auto packagePath = GetPackageReferencePath(workingDirectory, dependency);
						auto packageRecipePath = packagePath + Runtime::BuildConstants::RecipeFileName();
						Runtime::Recipe dependencyRecipe = {};
						if (!_buildManager.TryGetRecipe(packageRecipePath, dependencyRecipe))
						{
							if (dependency.IsLocal())
							{
								Log::Error("The dependency Recipe does not exist: " + packageRecipePath.ToString());
								Log::HighPriority("Make sure the path is correct and try again");
							}
							else
							{
								Log::Error("The dependency Recipe version has not been installed: " + dependency.ToString() + " [" + workingDirectory.ToString() + "]");
								Log::HighPriority("Run `install` and try again");
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
						bool isDependencyHostBuild = isHostBuild || knownDependecyType == "Build";
						projectId = BuildRecipeAndDependencies(
							projectId,
							packagePath,
							dependencyRecipe,
							isDependencyHostBuild,
							activeParentSet);
					}
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
			Runtime::Recipe& recipe,
			bool isHostBuild)
		{
			// TODO: RAII for active id
			try
			{
				Log::SetActiveId(projectId);
				Log::Diag("Running Build");

				// Select the correct build set to ensure that the different build properties 
				// required the same project to be build twice
				auto& buildSet = isHostBuild ? _hostBuildSet : _buildSet;
				auto findBuildState = buildSet.find(recipe.GetName());
				if (findBuildState != buildSet.end())
				{
					// Verify the project name is unique
					if (findBuildState->second != workingDirectory)
					{
						Log::Error("Recipe with this name already exists: " + recipe.GetName() + " [" + workingDirectory.ToString() + "] [" + findBuildState->second.ToString() + "]");
						throw std::runtime_error("Recipe name not unique");
					}
					else
					{
						Log::Diag("Recipe already built: " + recipe.GetName());
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
						recipe.GetName(),
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
			Runtime::Recipe& recipe,
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
			auto targetDirectory = GetOutputDirectory(
				packageRoot,
				recipe,
				globalParameters,
				isHostBuild,
				_buildManager);
			auto soupTargetDirectory = targetDirectory + GetSoupTargetDirectory();

			if (!_arguments.SkipGenerate)
			{
				RunIncrementalGenerate(
					recipe,
					packageRoot,
					targetDirectory,
					soupTargetDirectory,
					globalParameters,
					isHostBuild);
			}

			// Build up the child target directory set
			auto childTargetDirectorySet = BuildChildDependenciesTargetDirectorySet(recipe, packageRoot, isHostBuild);
			if (!_arguments.SkipEvaluate)
			{
				RunEvaluate(packageRoot, targetDirectory, soupTargetDirectory, childTargetDirectorySet);
			}

			// Cache the build state for upstream dependencies
			auto& buildCache = isHostBuild ? _hostBuildCache : _buildCache;
			buildCache.emplace(
				packageRoot,
				RecipeBuildCacheState(
					recipe.GetName(),
					std::move(targetDirectory),
					std::move(soupTargetDirectory),
					std::move(childTargetDirectorySet)));
		}

		/// <summary>
		/// Run an incremental generate phase
		/// </summary>
		void RunIncrementalGenerate(
			Runtime::Recipe& recipe,
			const Path& packageDirectory,
			const Path& targetDirectory,
			const Path& soupTargetDirectory,
			const Runtime::ValueTable& globalParameters,
			bool isHostBuild)
		{
			// Clone the global parameters
			auto parametersTable = Runtime::ValueTable(globalParameters.GetValues());

			// Set the input parameters
			parametersTable.SetValue("PackageDirectory", Runtime::Value(packageDirectory.ToString()));
			parametersTable.SetValue("TargetDirectory", Runtime::Value(targetDirectory.ToString()));
			parametersTable.SetValue("SoupTargetDirectory", Runtime::Value(soupTargetDirectory.ToString()));
			parametersTable.SetValue("Dependencies", BuildParametersDependenciesValueTable(recipe, packageDirectory, isHostBuild));

			auto parametersFile = soupTargetDirectory + Runtime::BuildConstants::GenerateParametersFileName();
			Log::Info("Check outdated parameters file: " + parametersFile.ToString());
			if (_arguments.ForceRebuild || IsOutdated(parametersTable, parametersFile))
			{
				Log::Info("Save Parameters file");
				Runtime::ValueTableManager::SaveState(parametersFile, parametersTable);
			}

			// Run the incremental generate
			auto generateGraph = Runtime::OperationGraph();

			// Add the single root operation to perform the generate
			auto moduleName = System::IProcessManager::Current().GetCurrentProcessFileName();
			auto moduleFolder = moduleName.GetParent();
			auto generateExecutable = moduleFolder + Path("Generate/Soup.Build.Generate.exe");
			Runtime::OperationId generateOperatioId = 1;
			auto generateArguments = std::stringstream();
			generateArguments << soupTargetDirectory.ToString();
			auto generateOperation = Runtime::OperationInfo(
				generateOperatioId,
				"Generate Phase",
				Runtime::CommandInfo(
					packageDirectory,
					generateExecutable,
					generateArguments.str()),
				{},
				{});
			generateOperation.DependencyCount = 1;
			generateGraph.AddOperation(std::move(generateOperation));

			// Set the Generate operation as the root
			generateGraph.SetRootOperationIds({
				generateOperatioId,
			});

			// Allow no read or write access to the generate phase
			auto allowedReadAccess = std::vector<Path>();
			auto allowedWriteAccess = std::vector<Path>();

			// Load the previous build graph if it exists and merge it with the new one
			auto generateGraphFile = soupTargetDirectory + GetGenerateGraphFileName();
			TryMergeExisting(generateGraphFile, generateGraph);

			// Set the temporary folder under the target folder
			auto temporaryDirectory = targetDirectory + GetTempraryFolderName();

			// Evaluate the Generate phase
			auto evaluateGenerateEngine = Runtime::BuildEvaluateEngine(
				_fileSystemState,
				generateGraph,
				std::move(temporaryDirectory),
				std::move(allowedReadAccess),
				std::move(allowedWriteAccess));
			evaluateGenerateEngine.Evaluate();

			// Save the operation graph for future incremental builds
			Runtime::OperationGraphManager::SaveState(generateGraphFile, generateGraph, *_fileSystemState);
		}

		bool IsOutdated(const Runtime::ValueTable& parametersTable, const Path& parametersFile)
		{
			// Load up the existing parameters file and check if our state matches the previous
			// to ensure incremental builds function correctly
			auto previousParametersState = Runtime::ValueTable();
			if (Runtime::ValueTableManager::TryLoadState(parametersFile, previousParametersState))
			{
				return previousParametersState != parametersTable;
			}
			else
			{
				return true;
			}
		}

		void RunEvaluate(
			const Path& packageRoot,
			const Path& targetDirectory,
			const Path& soupTargetDirectory,
			const std::set<Path>& childTargetDirectorySet)
		{
			// Load and run the previous stored state directly
			auto generateEvaluateGraphFile = soupTargetDirectory + Runtime::BuildConstants::GenerateEvaluateOperationGraphFileName();
			auto evaluateResultGraphFile = soupTargetDirectory + GetEvaluateResultGraphFileName();

			Log::Info("Loading generate evaluate operation graph");
			auto evaluateGraph = Runtime::OperationGraph();
			if (!Runtime::OperationGraphManager::TryLoadState(
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

			// Initialize the read access with the shared global set
			auto allowedReadAccess = std::vector<Path>(_globalReadAccess);
			auto allowedWriteAccess = std::vector<Path>();

			// Allow read access for all transitive dependencies target directories
			std::copy(childTargetDirectorySet.begin(), childTargetDirectorySet.end(), std::back_inserter(allowedReadAccess));

			// Allow reading from the package root (source input) and the target directory (intermediate output)
			allowedReadAccess.push_back(packageRoot);
			allowedReadAccess.push_back(targetDirectory);

			// Only allow writing to the target directory
			allowedWriteAccess.push_back(targetDirectory);

			// Set the temporary folder under the target folder
			auto temporaryDirectory = targetDirectory + GetTempraryFolderName();

			// Ensure the temporary directories exists
			if (!System::IFileSystem::Current().Exists(temporaryDirectory))
			{
				Log::Info("Create Directory: " + temporaryDirectory.ToString());
				System::IFileSystem::Current().CreateDirectory2(temporaryDirectory);
			}

			try
			{
				// Evaluate the build
				auto evaluateEngine = Runtime::BuildEvaluateEngine(
					_fileSystemState,
					evaluateGraph,
					std::move(temporaryDirectory),
					std::move(allowedReadAccess),
					std::move(allowedWriteAccess));
				evaluateEngine.Evaluate();
			}
			catch(const Runtime::BuildFailedException&)
			{
				Log::Info("Saving partial build state");
				Runtime::OperationGraphManager::SaveState(
					evaluateResultGraphFile,
					evaluateGraph,
					*_fileSystemState);
				throw;
			}

			Log::Info("Saving updated build state");
			Runtime::OperationGraphManager::SaveState(
				evaluateResultGraphFile,
				evaluateGraph,
				*_fileSystemState);

			Log::Info("Done");
		}

		Path GetSoupUserDataPath() const
		{
			auto result = System::IFileSystem::Current().GetUserProfileDirectory() +
				Path(".soup/");
			return result;
		}

		Path GetPackageReferencePath(const Path& workingDirectory, const Runtime::PackageReference& reference) const
		{
			// If the path is relative then combine with the working directory
			Path packagePath;
			if (reference.IsLocal())
			{
				packagePath = reference.GetPath();
				if (!packagePath.HasRoot())
				{
					packagePath = workingDirectory + packagePath;
				}
			}
			else
			{
				auto packageStore = GetSoupUserDataPath() +
					Path("packages/");
				packagePath = packageStore + Path(reference.GetName()) + Path(reference.GetVersion().ToString());
			}

			return packagePath;
		}

		/// <summary>
		/// Attempt to merge the existing operation graph if it exists
		/// </summary>
		void TryMergeExisting(
			const Path& operationGraphFile,
			Runtime::OperationGraph& operationGraph)
		{
			Log::Diag("Loading previous operation graph");
			auto previousOperationGraph = Runtime::OperationGraph();
			if (Runtime::OperationGraphManager::TryLoadState(
				operationGraphFile,
				previousOperationGraph,
				*_fileSystemState))
			{
				Log::Diag("Merge previous operation graph observed results");
				for (auto& activeOperationEntry : operationGraph.GetOperations())
				{
					auto& activeOperationInfo = activeOperationEntry.second;
					Runtime::OperationInfo* previousOperationInfo = nullptr;
					if (previousOperationGraph.TryFindOperationInfo(activeOperationInfo.Command, previousOperationInfo))
					{
						activeOperationInfo.WasSuccessfulRun = previousOperationInfo->WasSuccessfulRun;
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

		Runtime::ValueTable BuildParametersDependenciesValueTable(
			Runtime::Recipe& recipe,
			const Path& workingDirectory,
			bool isHostBuild)
		{
			auto knownDependecyTypes = std::array<std::string_view, 3>({
				"Runtime",
				"Test",
				"Build",
			});

			auto result = Runtime::ValueTable();
			for (auto knownDependecyType : knownDependecyTypes)
			{
				if (recipe.HasNamedDependencies(knownDependecyType))
				{
					auto& dependencyTypeTable = result.SetValue(knownDependecyType, Runtime::Value(Runtime::ValueTable())).AsTable();
					for (auto dependency : recipe.GetNamedDependencies(knownDependecyType))
					{
						// Load this package recipe
						auto packagePath = GetPackageReferencePath(workingDirectory, dependency);

						// Cache the build state for upstream dependencies
						bool isDependencyHostBuild = isHostBuild || knownDependecyType == "Build";
						auto& buildCache = isDependencyHostBuild ? _hostBuildCache : _buildCache;

						auto findBuildCache = buildCache.find(packagePath);
						if (findBuildCache != buildCache.end())
						{
							auto& dependencyState = findBuildCache->second;
							dependencyTypeTable.SetValue(
								dependencyState.Name,
								Runtime::Value(Runtime::ValueTable({
									{
										"TargetDirectory",
										Runtime::Value(dependencyState.TargetDirectory.ToString())
									},
									{
										"SoupTargetDirectory",
										Runtime::Value(dependencyState.SoupTargetDirectory.ToString())
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
			}

			return result;
		}

		std::set<Path> BuildChildDependenciesTargetDirectorySet(
			Runtime::Recipe& recipe,
			const Path& workingDirectory,
			bool isHostBuild)
		{
			auto knownDependecyTypes = std::array<std::string_view, 3>({
				"Runtime",
				"Test",
				"Build",
			});

			auto result = std::set<Path>();
			for (auto knownDependecyType : knownDependecyTypes)
			{
				if (recipe.HasNamedDependencies(knownDependecyType))
				{
					for (auto dependency : recipe.GetNamedDependencies(knownDependecyType))
					{
						// Load this package recipe
						auto packagePath = GetPackageReferencePath(workingDirectory, dependency);

						// Cache the build state for upstream dependencies
						bool isDependencyHostBuild = isHostBuild || knownDependecyType == "Build";
						auto& buildCache = isDependencyHostBuild ? _hostBuildCache : _buildCache;

						auto findBuildCache = buildCache.find(packagePath);
						if (findBuildCache != buildCache.end())
						{
							// Combine the child dependency target and the transitive children
							auto& dependencyState = findBuildCache->second;
							result.insert(dependencyState.TargetDirectory);
							result.insert(dependencyState.ChildTargetDirectorySet.begin(), dependencyState.ChildTargetDirectorySet.end());
						}
						else
						{
							Log::Error("Dependency does not exist in build cache: " + packagePath.ToString());
							throw std::runtime_error("Dependency does not exist in build cache");
						}
					}
				}
			}

			return result;
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

		static Path GetTempraryFolderName()
		{
			static const auto value = Path("temp/");
			return value;
		}

	private:
		RecipeBuildArguments _arguments;
		Runtime::ValueTable _hostBuildGlobalParameters;

		RecipeBuildManager _buildManager;

		// Global read access
		std::vector<Path> _globalReadAccess;

		// Mapping from name to build folder to check for duplicate names with different packages
		std::map<std::string, Path> _buildSet;
		std::map<std::string, Path> _hostBuildSet;

		// Mapping from package root path to the name and target folder to be used with dependencies parameters
		std::map<Path, RecipeBuildCacheState> _buildCache;
		std::map<Path, RecipeBuildCacheState> _hostBuildCache;

		std::shared_ptr<Runtime::FileSystemState> _fileSystemState;
	};
}
