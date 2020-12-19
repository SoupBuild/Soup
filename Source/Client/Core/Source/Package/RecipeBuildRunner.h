// <copyright file="RecipeBuildRunner.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeBuildManager.h"
#include "RecipeBuildArguments.h"
#include "RecipeExtensions.h"
#include "RecipeBuildStateConverter.h"

namespace Soup::Build
{
	/// <summary>
	/// The recipe build runner that knows how to perform the correct build for a recipe
	/// and all of its development and runtime dependencies
	/// </summary>
	export class RecipeBuildRunner
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RecipeBuildRunner"/> class.
		/// </summary>
		RecipeBuildRunner(
			std::string hostCompiler,
			std::string runtimeCompiler,
			RecipeBuildArguments arguments) :
			_hostCompiler(std::move(hostCompiler)),
			_runtimeCompiler(std::move(runtimeCompiler)),
			_arguments(std::move(arguments)),
			_buildManager(),
			_buildSet(),
			_hostBuildSet(),
			_hostBuildPaths(),
			_fileSystemState(std::make_shared<Runtime::FileSystemState>())
		{
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(const Path& workingDirectory)
		{
			// Enable log event ids to track individual builds
			int projectId = 1;
			bool isHostBuild = false;
			std::string_view dependencyType = "Runtime";
			Log::EnsureListener().SetShowEventId(true);

			// TODO: A scoped listener cleanup would be nice
			try
			{
				auto recipePath = workingDirectory + Path(Constants::RecipeFileName);
				Recipe recipe = {};
				if (!_buildManager.TryGetRecipe(recipePath, recipe))
				{
					Log::Error("The target Recipe does not exist: " + recipePath.ToString());
					Log::HighPriority("Make sure the path is correct and try again");

					// Nothing we can do, exit
					throw HandledException(1123124);
				}

				auto rootParentSet = std::set<std::string>();
				auto rootState = RecipeBuildStateConverter::ConvertToBuildState(recipe.GetTable());

				projectId = BuildRecipeAndDependencies(
					projectId,
					workingDirectory,
					recipe,
					isHostBuild,
					dependencyType,
					rootParentSet,
					rootState);

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
			std::string_view dependencyType,
			const std::set<std::string>& parentSet,
			Runtime::ValueTable& sharedState)
		{
			// Add current package to the parent set when building child dependencies
			auto activeParentSet = parentSet;
			activeParentSet.insert(std::string(recipe.GetName()));

			// Start a new active state that is initialized to the recipe itself
			auto activeState = RecipeBuildStateConverter::ConvertToRootBuildState(recipe.GetTable());

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
						auto packageRecipePath = packagePath + Path(Constants::RecipeFileName);
						Recipe dependencyRecipe = {};
						if (!_buildManager.TryGetRecipe(packageRecipePath, dependencyRecipe))
						{
							if (dependency.IsLocal())
							{
								Log::Error("The dependency Recipe does not exist: " + packageRecipePath.ToString());
								Log::HighPriority("Make sure the path is correct and try again");
							}
							else
							{
								Log::Error("The dependency Recipe version has not been installed: " + dependency.ToString());
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
							knownDependecyType,
							activeParentSet,
							activeState);
					}
				}
			}

			// Build the root recipe
			projectId = CheckBuildRecipe(
				projectId,
				workingDirectory,
				recipe,
				isHostBuild,
				dependencyType,
				activeState,
				sharedState);

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
			bool isHostBuild,
			std::string_view dependencyType,
			Runtime::ValueTable& activeState,
			Runtime::ValueTable& sharedState)
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
					Log::Diag("Recipe already built: " + recipe.GetName());
				}
				else
				{
					// Run the required builds in process
					// This will break the circular requirements for the core build libraries
					auto resultSharedState = RunInProcessBuild(
						workingDirectory,
						recipe,
						isHostBuild,
						activeState);

					// Keep track of the packages we have already built
					// TODO: Verify unique names
					auto insertBuildState = buildSet.emplace(
						recipe.GetName(),
						std::move(resultSharedState));

					// Replace the find iterator so it can be used to update the shared table state
					findBuildState = insertBuildState.first;

					// Move to the next build project id
					projectId++;
				}

				// Add the shared build state Runtimed from this child build into the correct
				// Table depending on the build type
				auto& dependenciesTable = sharedState.EnsureValue("Dependencies").EnsureTable();
				auto& typedDependenciesTable = dependenciesTable.EnsureValue(dependencyType).EnsureTable();
				typedDependenciesTable.SetValue(
					recipe.GetName(),
					Runtime::Value(findBuildState->second));

				Log::SetActiveId(0);
			}
			catch(...)
			{
				Log::SetActiveId(0);
				throw;
			}

			return projectId;
		}

		Runtime::ValueTable RunInProcessBuild(
			const Path& packageRoot,
			Recipe& recipe,
			bool isHostBuild,
			Runtime::ValueTable& activeState)
		{
			// Select the correct compiler to use
			std::string activeCompiler = "";
			std::string activeFlavor = "";
			if (isHostBuild)
			{
				Log::HighPriority("Host Build '" + recipe.GetName() + "'");
				activeCompiler = _hostCompiler;
				activeFlavor = "release";
			}
			else
			{
				Log::HighPriority("Build '" + recipe.GetName() + "'");
				activeCompiler = _runtimeCompiler;
				activeFlavor = _arguments.Flavor;
			}

			// Set the default output directory to be relative to the package
			auto rootOutput = packageRoot + Path("out/");

			// Add unique location for host builds
			if (isHostBuild)
			{
				rootOutput = rootOutput + Path("HostBuild/");
			}

			// Check for root recipe file with overrides
			Path rootRecipeFile;
			if (RecipeExtensions::TryFindRootRecipeFile(packageRoot, rootRecipeFile))
			{
				Log::Info("Found Root Recipe: '" + rootRecipeFile.ToString() + "'");
				RootRecipe rootRecipe;
				if (!_buildManager.TryGetRootRecipe(rootRecipeFile, rootRecipe))
				{
					// Nothing we can do, exit
					Log::Error("Failed to load the root recipe file: " + rootRecipeFile.ToString());
					throw HandledException(222);
				}

				// Today the only unique thing it can do is set the shared output directory
				if (rootRecipe.HasOutputRoot())
				{
					// Relative to the root recipe file itself
					rootOutput = rootRecipe.GetOutputRoot();

					// Add unique location for host builds
					if (isHostBuild)
					{
						rootOutput = rootOutput + Path("HostBuild/");
					}

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

			// Build up the expected output directory for the build to be used to cache state
			auto targetDirectory = rootOutput + Runtime::BuildGenerateEngine::GetConfigurationDirectory(
				_hostCompiler,
				activeFlavor,
				_arguments.System,
				_arguments.Architecture);

			// Cache if is host build to load build tasks
			if (isHostBuild)
			{
				_hostBuildPaths.emplace(recipe.GetName(), targetDirectory);
			}

			auto activeBuildGraph = Runtime::OperationGraph();
			auto sharedState = Runtime::ValueTable();
			if (!_arguments.SkipGenerate)
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

				if (recipe.HasBuildDependencies())
				{
					for (auto dependency : recipe.GetBuildDependencies())
					{
						auto packagePath = GetPackageReferencePath(packageRoot, dependency);
						auto packageRecipePath = packagePath + Path(Constants::RecipeFileName);
						Recipe dependecyRecipe = {};
						if (!RecipeExtensions::TryLoadRecipeFromFile(packageRecipePath, dependecyRecipe))
						{
							Log::Error("Failed to load the package: " + packageRecipePath.ToString());
							throw std::runtime_error("RunInProcessBuild: Failed to load dependency.");
						}

						// Get the recipe output directory
						auto findOutputDirectory = _hostBuildPaths.find(dependecyRecipe.GetName());
						if (findOutputDirectory == _hostBuildPaths.end())
						{
							Log::Error("Failed to find the host build path to check for a build extension: " + dependecyRecipe.GetName());
							throw std::runtime_error("RunInProcessBuild: Failed to get dev dependency output directory.");
						}

						auto binaryPath = findOutputDirectory->second + Runtime::BuildGenerateEngine::GetBinaryDirectory();
						auto moduleFilename = Path(dependecyRecipe.GetName() + ".dll");
						auto libraryPath = binaryPath + moduleFilename;

						if (System::IFileSystem::Current().Exists(libraryPath))
						{
							buildExtensionLibraries.push_back(std::move(libraryPath));
						}
					}
				}

				auto buildGenerateEngine = Runtime::BuildGenerateEngine(
					_fileSystemState);
				auto generateResult = buildGenerateEngine.Generate(
					targetDirectory,
					activeCompiler,
					activeFlavor,
					_arguments.System,
					_arguments.Architecture,
					packageRoot,
					activeState,
					buildExtensionLibraries);

				activeBuildGraph = std::move(generateResult.Graph);
				sharedState = std::move(generateResult.SharedState);
			}
			else
			{
				// Load and run the previous stored state directly
				Log::Info("Loading previous operation graph as the active graph");
				if (!Runtime::OperationGraphManager::TryLoadState(
					targetDirectory,
					activeBuildGraph,
					*_fileSystemState))
				{
					throw std::runtime_error("Missing cached operation graph when skipping Runtime phase.");
				}
			}

			if (!_arguments.SkipEvaluate)
			{
				if (_arguments.ForceRebuild)
				{
					Log::Diag("Purge operation graph to force build");
					for (auto& activeOperationEntry : activeBuildGraph.GetOperations())
					{
						auto& activeOperationInfo = activeOperationEntry.second;
						activeOperationInfo.WasSuccessfulRun = false;
						activeOperationInfo.ObservedInput = {};
						activeOperationInfo.ObservedOutput = {};
					}
				}

				try
				{
					// Evaluate the build
					auto runner = Runtime::BuildEvaluateEngine(
						_fileSystemState,
						activeBuildGraph);
					runner.Evaluate();
				}
				catch(const Runtime::BuildFailedException&)
				{
					Log::Info("Saving partial build state");
					Runtime::OperationGraphManager::SaveState(
						targetDirectory,
						activeBuildGraph,
						*_fileSystemState);
					throw;
				}

				Log::Info("Saving updated build state");
				Runtime::OperationGraphManager::SaveState(
					targetDirectory,
					activeBuildGraph,
					*_fileSystemState);

				Log::HighPriority("Done");
			}

			return sharedState;
		}

		Path GetSoupUserDataPath() const
		{
			auto result = System::IFileSystem::Current().GetUserProfileDirectory() +
				Path(".soup/");
			return result;
		}

		Path GetPackageReferencePath(const Path& workingDirectory, const PackageReference& reference) const
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

	private:
		std::string _hostCompiler;
		std::string _runtimeCompiler;
		RecipeBuildArguments _arguments;

		RecipeBuildManager _buildManager;

		std::map<std::string, Runtime::ValueTable> _buildSet;
		std::map<std::string, Runtime::ValueTable> _hostBuildSet;

		std::map<std::string, Path> _hostBuildPaths;

		std::shared_ptr<Runtime::FileSystemState> _fileSystemState;
	};
}
