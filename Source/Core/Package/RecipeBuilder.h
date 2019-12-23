﻿// <copyright file="RecipeBuild.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"
#include "RecipeBuildArguments.h"
#include "Build/BuildSystem.h"

namespace Soup
{
	/// <summary>
	/// The recipe builder that knows how to build a single recipe
	/// Used either directly inside Soup builds or in the generated build executable
	/// </summary>
	export class RecipeBuilder
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="RecipeBuilder"/> class.
		/// </summary>
		RecipeBuilder(
			std::shared_ptr<ICompiler> systemCompiler,
			std::shared_ptr<ICompiler> runtimeCompiler) :
			_systemCompiler(std::move(systemCompiler)),
			_runtimeCompiler(std::move(runtimeCompiler))
		{
			if (_systemCompiler == nullptr)
				throw std::runtime_error("Argument null: systemCompiler");
			if (_runtimeCompiler == nullptr)
				throw std::runtime_error("Argument null: runtimeCompiler");
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(
			const Path& workingDirectory,
			const Recipe& recipe,
			const RecipeBuildArguments& arguments,
			bool isSystemBuild)
		{
			std::shared_ptr<ICompiler> activeCompiler = nullptr;
			if (isSystemBuild)
			{
				Log::HighPriority("System Build '" + recipe.GetName() + "'");
				activeCompiler = _systemCompiler;
			}
			else
			{
				Log::HighPriority("Build '" + recipe.GetName() + "'");
				activeCompiler = _runtimeCompiler;
			}

			// Run all build tasks
			auto buildSystem = BuildSystem();
			if (recipe.HasDevDependencies())
			{
				for (auto dependecy : recipe.GetDevDependencies())
				{
					auto packagePath = RecipeExtensions::GetPackageReferencePath(workingDirectory, dependecy);
					auto libraryPath = RecipeExtensions::GetRecipeOutputPath(
						packagePath,
						RecipeExtensions::GetBinaryDirectory(*_systemCompiler, arguments.Flavor),
						std::string(_systemCompiler->GetDynamicLibraryFileExtension()));
					
					RunBuildExtension(libraryPath, buildSystem);
				}
			}

			// Add all dependency packages modules references
			auto includeModules = std::vector<Path>();
			if (recipe.GetLanguageVersion() == RecipeLanguageVersion::CPP20)
			{
				// TODO: MSVC requires the entire closure of interfaces
				bool isRecursive = activeCompiler->GetName() == "MSVC";
				RecipeExtensions::GenerateDependecyModuleIncludeClosure(
					*activeCompiler,
					arguments.Flavor,
					workingDirectory,
					recipe,
					includeModules,
					isRecursive);
			}

			// Add the dependency static library closure to link if targeting an executable or dynamic library
			std::vector<Path> linkLibraries = buildSystem.GetStaticLibraries();
			std::vector<Path> externalLinkLibraries = std::vector<Path>();
			if (recipe.GetType() == RecipeType::Executable || recipe.GetType() == RecipeType::DynamicLibrary)
			{
				RecipeExtensions::GenerateDependecyStaticLibraryClosure(
					*activeCompiler,
					arguments.Flavor,
					workingDirectory,
					recipe,
					linkLibraries);

				// Add the platform libraries
				externalLinkLibraries.insert(
					externalLinkLibraries.end(),
					arguments.PlatformLibraries.begin(),
					arguments.PlatformLibraries.end());
			}

			// Combine the include paths from the recipe and the system
			auto includePaths = buildSystem.GetIncludePaths();
			if (recipe.HasIncludePaths())
			{
				auto recipeIncludePaths = recipe.GetIncludePathsAsPath();
				includePaths.insert(
					includePaths.end(),
					recipeIncludePaths.begin(),
					recipeIncludePaths.end());
			}

			// Add the platform include paths
			includePaths.insert(
				includePaths.end(),
				arguments.PlatformIncludePaths.begin(),
				arguments.PlatformIncludePaths.end());

			// Load the extra library paths provided to the build system
			auto libraryPaths = buildSystem.GetLibraryPaths();

			// Add the platform library paths
			libraryPaths.insert(
				libraryPaths.end(),
				arguments.PlatformLibraryPaths.begin(),
				arguments.PlatformLibraryPaths.end());

			// Combine the defines with the default set and the platform
			auto preprocessorDefinitions = buildSystem.GetPreprocessorDefinitions();
			preprocessorDefinitions.insert(
				preprocessorDefinitions.end(),
				arguments.PlatformPreprocessorDefinitions.begin(),
				arguments.PlatformPreprocessorDefinitions.end());
			preprocessorDefinitions.push_back("SOUP_BUILD");
			if (recipe.HasDefines())
			{
				auto& recipeDefines = recipe.GetDefines();
				preprocessorDefinitions.insert(
					preprocessorDefinitions.end(),
					recipeDefines.begin(),
					recipeDefines.end());
			}

			// Build up arguments to build this individual recipe
			auto buildArguments = BuildArguments();
			buildArguments.TargetName = recipe.GetName();
			buildArguments.WorkingDirectory = workingDirectory;
			buildArguments.ObjectDirectory = RecipeExtensions::GetObjectDirectory(
				*activeCompiler,
				arguments.Flavor);
			buildArguments.BinaryDirectory = RecipeExtensions::GetBinaryDirectory(
				*activeCompiler,
				arguments.Flavor);
			buildArguments.ModuleInterfaceSourceFile = Path();
			buildArguments.SourceFiles = recipe.GetSourceAsPath();
			buildArguments.IncludeModules = std::move(includeModules);
			buildArguments.LinkLibraries = std::move(linkLibraries);
			buildArguments.ExternalLinkLibraries = std::move(externalLinkLibraries);
			buildArguments.IsIncremental = !arguments.ForceRebuild;
			buildArguments.GenerateSourceDebugInfo = false;
			buildArguments.PreprocessorDefinitions = std::move(preprocessorDefinitions);
			buildArguments.IncludeDirectories = std::move(includePaths);
			buildArguments.LibraryPaths = std::move(libraryPaths);

			if (recipe.HasPublic())
			{
				buildArguments.ModuleInterfaceSourceFile = recipe.GetPublicAsPath();
				
				// TODO: Clang requires annoying cppm extension
				if (_runtimeCompiler->GetName() == "Clang")
				{
					buildArguments.ModuleInterfaceSourceFile.SetFileExtension("cppm");
				}
			}

			// Set the correct optimization level for the requested flavor
			if (arguments.Flavor == "debug")
			{
				buildArguments.OptimizationLevel = BuildOptimizationLevel::None;
				buildArguments.GenerateSourceDebugInfo = true;
			}
			else if (arguments.Flavor == "release")
			{
				buildArguments.OptimizationLevel = BuildOptimizationLevel::Speed;
			}
			else
			{
				Log::Error("Unknown build flavor type.");
				throw std::runtime_error("Unknown build flavors type.");
			}

			// Convert the recipe type to the required build type
			switch (recipe.GetType())
			{
				case RecipeType::StaticLibrary:
					buildArguments.TargetType = BuildTargetType::StaticLibrary;
					break;
				case RecipeType::DynamicLibrary:
					buildArguments.TargetType = BuildTargetType::DynamicLibrary;
					break;
				case RecipeType::Executable:
					buildArguments.TargetType = BuildTargetType::Executable;
					break;
				default:
					throw std::runtime_error("Unknown build target type.");
			}

			// Convert the recipe language version to the required build language
			switch (recipe.GetLanguageVersion())
			{
				case RecipeLanguageVersion::CPP11:
					buildArguments.LanguageStandard = LanguageStandard::CPP11;
					break;
				case RecipeLanguageVersion::CPP14:
					buildArguments.LanguageStandard = LanguageStandard::CPP14;
					break;
				case RecipeLanguageVersion::CPP17:
					buildArguments.LanguageStandard = LanguageStandard::CPP17;
					break;
				case RecipeLanguageVersion::CPP20:
					buildArguments.LanguageStandard = LanguageStandard::CPP20;
					break;
				default:
					throw std::runtime_error("Unknown recipe language version.");
			}

			// Perform the build
			auto buildEngine = BuildEngine(activeCompiler);
			auto wasBuilt = buildEngine.Execute(buildArguments);

			// Build up the runtime dependencies
			if (recipe.GetType() == RecipeType::Executable || recipe.GetType() == RecipeType::DynamicLibrary)
			{
				std::vector<Path> runtimeDependencies;
				RecipeExtensions::GenerateDependecyDynamicLibraryClosure(
					*activeCompiler,
					arguments.Flavor,
					workingDirectory,
					recipe,
					runtimeDependencies);

				for (auto source : runtimeDependencies)
				{
					auto target = buildArguments.BinaryDirectory + Path(source.GetFileName());
					Log::Info("Copy: [" + source.ToString() + "] -> [" + target.ToString() + "]");
					System::IFileSystem::Current().CopyFile2(source, target);
				}
			}

			if (wasBuilt)
				Log::HighPriority("Done");
			else
				Log::HighPriority("Up to date");
		}

	private:
		void RunBuildExtension(Path& libraryPath, BuildEx::IBuildSystem& buildSystem)
		{
			try
			{
				Log::Info("Running Build Extension: " + libraryPath.ToString());
				auto library = System::DynamicLibraryManager::LoadDynamicLibrary(
					libraryPath.ToString().c_str());
				auto function = (int(*)(BuildEx::IBuildSystem&))library.GetFunction(
					"?RegisterBuildExtension@@YAHAEAVIBuildSystem@BuildEx@Soup@@@Z");
				auto result = function(buildSystem);
				Log::Info("Build Extension Done: " + std::to_string(result));
			}
			catch (...)
			{
				Log::Error("Build Extension Failed!");
				throw;
			}
		}

	private:
		std::shared_ptr<ICompiler> _systemCompiler;
		std::shared_ptr<ICompiler> _runtimeCompiler;
	};
}
