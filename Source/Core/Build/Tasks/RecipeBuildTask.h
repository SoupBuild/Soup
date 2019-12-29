// <copyright file="RecipeBuild.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"
#include "RecipeBuildArguments.h"

namespace Soup::Build
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	export class RecipeBuildTask : public IBuildTask
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RecipeBuildTask"/> class.
		/// </summary>
		RecipeBuildTask(
			std::shared_ptr<ICompiler> systemCompiler,
			std::shared_ptr<ICompiler> activeCompiler,
			const Path& workingDirectory,
			const Recipe& recipe,
			const RecipeBuildArguments& arguments) :
			_systemCompiler(std::move(systemCompiler)),
			_activeCompiler(std::move(activeCompiler)),
			_workingDirectory(workingDirectory),
			_recipe(recipe),
			_arguments(arguments)
		{
			if (_systemCompiler == nullptr)
				throw std::runtime_error("Argument null: systemCompiler");
			if (_activeCompiler == nullptr)
				throw std::runtime_error("Argument null: activeCompiler");
		}

		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() override final
		{
			return "RecipeBuild";
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(IBuildState& state) override final
		{
			// Run all build tasks
			auto buildSystem = BuildSystem();
			if (_recipe.HasDevDependencies())
			{
				for (auto dependecy : _recipe.GetDevDependencies())
				{
					auto packagePath = RecipeExtensions::GetPackageReferencePath(_workingDirectory, dependecy);
					auto libraryPath = RecipeExtensions::GetRecipeOutputPath(
						packagePath,
						RecipeExtensions::GetBinaryDirectory(*_systemCompiler, _arguments.Flavor),
						std::string(_systemCompiler->GetDynamicLibraryFileExtension()));
					
					RunBuildExtension(libraryPath, buildSystem);
				}
			}

			// Add all dependency packages modules references
			auto includeModules = std::vector<Path>();
			if (_recipe.GetLanguageVersion() == RecipeLanguageVersion::CPP20)
			{
				// TODO: MSVC requires the entire closure of interfaces
				bool isRecursive = _activeCompiler->GetName() == "MSVC";
				RecipeExtensions::GenerateDependecyModuleIncludeClosure(
					*_activeCompiler,
					_arguments.Flavor,
					_workingDirectory,
					_recipe,
					includeModules,
					isRecursive);
			}

			// Add the dependency static library closure to link if targeting an executable or dynamic library
			std::vector<Path> linkLibraries = std::vector<Path>();
			std::vector<Path> externalLinkLibraries = std::vector<Path>();
			if (_recipe.GetType() == RecipeType::Executable || _recipe.GetType() == RecipeType::DynamicLibrary)
			{
				RecipeExtensions::GenerateDependecyStaticLibraryClosure(
					*_activeCompiler,
					_arguments.Flavor,
					_workingDirectory,
					_recipe,
					linkLibraries);

				// Add the platform libraries
				externalLinkLibraries.insert(
					externalLinkLibraries.end(),
					_arguments.PlatformLibraries.begin(),
					_arguments.PlatformLibraries.end());
			}

			// Combine the include paths from the recipe and the system
			auto includePaths = std::vector<Path>();
			if (_recipe.HasIncludePaths())
			{
				auto recipeIncludePaths = _recipe.GetIncludePathsAsPath();
				includePaths.insert(
					includePaths.end(),
					recipeIncludePaths.begin(),
					recipeIncludePaths.end());
			}

			// Add the platform include paths
			includePaths.insert(
				includePaths.end(),
				_arguments.PlatformIncludePaths.begin(),
				_arguments.PlatformIncludePaths.end());

			// Load the extra library paths provided to the build system
			auto libraryPaths = std::vector<Path>();

			// Add the platform library paths
			libraryPaths.insert(
				libraryPaths.end(),
				_arguments.PlatformLibraryPaths.begin(),
				_arguments.PlatformLibraryPaths.end());

			// Combine the defines with the default set and the platform
			auto preprocessorDefinitions = std::vector<std::string>();
			preprocessorDefinitions.insert(
				preprocessorDefinitions.end(),
				_arguments.PlatformPreprocessorDefinitions.begin(),
				_arguments.PlatformPreprocessorDefinitions.end());
			preprocessorDefinitions.push_back("SOUP_BUILD");
			if (_recipe.HasDefines())
			{
				auto& recipeDefines = _recipe.GetDefines();
				preprocessorDefinitions.insert(
					preprocessorDefinitions.end(),
					recipeDefines.begin(),
					recipeDefines.end());
			}

			// Build up arguments to build this individual recipe
			auto binaryDirectory = RecipeExtensions::GetBinaryDirectory(*_activeCompiler, _arguments.Flavor);

			state.SetProperty("TargetName", _recipe.GetName());
			state.SetProperty("WorkingDirectory", _workingDirectory);
			state.SetProperty(
				"ObjectDirectory",
				RecipeExtensions::GetObjectDirectory(*_activeCompiler, _arguments.Flavor));
			state.SetProperty("BinaryDirectory", binaryDirectory);
			state.SetProperty("ModuleInterfaceSourceFile", Path());
			state.SetProperty("SourceFiles", _recipe.GetSourceAsPath());
			state.SetProperty("IncludeModules", std::move(includeModules));
			state.SetProperty("LinkLibraries", std::move(linkLibraries));
			state.SetProperty("ExternalLinkLibraries", std::move(externalLinkLibraries));
			state.SetProperty("IsIncremental", !_arguments.ForceRebuild);
			state.SetProperty("GenerateSourceDebugInfo", false);
			state.SetProperty("PreprocessorDefinitions", std::move(preprocessorDefinitions));
			state.SetProperty("IncludeDirectories", std::move(includePaths));
			state.SetProperty("LibraryPaths", std::move(libraryPaths));

			if (_recipe.HasPublic())
			{
				auto moduleInterfaceSourceFile = _recipe.GetPublicAsPath();
				
				// TODO: Clang requires annoying cppm extension
				if (_activeCompiler->GetName() == "Clang")
				{
					moduleInterfaceSourceFile.SetFileExtension("cppm");
				}

				state.SetProperty("ModuleInterfaceSourceFile", moduleInterfaceSourceFile);
			}

			// Set the correct optimization level for the requested flavor
			if (_arguments.Flavor == "debug")
			{
				state.SetProperty("OptimizationLevel", BuildOptimizationLevel::None);
				state.SetProperty("GenerateSourceDebugInfo", true);
			}
			else if (_arguments.Flavor == "release")
			{
				state.SetProperty("OptimizationLevel", BuildOptimizationLevel::Speed);
			}
			else
			{
				Log::Error("Unknown build flavor type.");
				throw std::runtime_error("Unknown build flavors type.");
			}

			// Convert the recipe type to the required build type
			switch (_recipe.GetType())
			{
				case RecipeType::StaticLibrary:
					state.SetProperty("TargetType", BuildTargetType::StaticLibrary);
					break;
				case RecipeType::DynamicLibrary:
					state.SetProperty("TargetType", BuildTargetType::DynamicLibrary);
					break;
				case RecipeType::Executable:
					state.SetProperty("TargetType", BuildTargetType::Executable);
					break;
				default:
					throw std::runtime_error("Unknown build target type.");
			}

			// Convert the recipe language version to the required build language
			switch (_recipe.GetLanguageVersion())
			{
				case RecipeLanguageVersion::CPP11:
					state.SetProperty("LanguageStandard", LanguageStandard::CPP11);
					break;
				case RecipeLanguageVersion::CPP14:
					state.SetProperty("LanguageStandard", LanguageStandard::CPP14);
					break;
				case RecipeLanguageVersion::CPP17:
					state.SetProperty("LanguageStandard", LanguageStandard::CPP17);
					break;
				case RecipeLanguageVersion::CPP20:
					state.SetProperty("LanguageStandard", LanguageStandard::CPP20);
					break;
				default:
					throw std::runtime_error("Unknown recipe language version.");
			}
		}

	private:
		void RunBuildExtension(Path& libraryPath, IBuildSystem& buildSystem)
		{
			// try
			// {
			// 	Log::Info("Running Build Extension: " + libraryPath.ToString());
			// 	auto library = System::DynamicLibraryManager::LoadDynamicLibrary(
			// 		libraryPath.ToString().c_str());
			// 	auto function = (int(*)(Build::IBuildSystem&))library.GetFunction(
			// 		"?RegisterBuildExtension@@YAHAEAVIBuildSystem@BuildEx@Soup@@@Z");
			// 	auto result = function(buildSystem);
			// 	Log::Info("Build Extension Done: " + std::to_string(result));
			// }
			// catch (...)
			// {
			// 	Log::Error("Build Extension Failed!");
			// 	throw;
			// }
		}

	private:
		std::shared_ptr<ICompiler> _systemCompiler;
		std::shared_ptr<ICompiler> _activeCompiler;
		Path _workingDirectory;
		Recipe _recipe;
		RecipeBuildArguments _arguments;
	};
}
