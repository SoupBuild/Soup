// <copyright file="RecipeBuild.h" company="Soup">
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
		RecipeBuilder(std::shared_ptr<ICompiler> compiler) :
			_compiler(std::move(compiler))
		{
			if (_compiler == nullptr)
				throw std::runtime_error("Argument null: compiler");
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(
			const Path& workingDirectory,
			const Recipe& recipe,
			const RecipeBuildArguments& arguments)
		{
			Log::Info("Building '" + recipe.GetName() + "'");

			// Run all build tasks
			if (recipe.HasDevDependencies())
			{
				for (auto dependecy : recipe.GetDevDependencies())
				{
					auto packagePath = RecipeExtensions::GetPackageReferencePath(workingDirectory, dependecy);
					auto libraryPath = RecipeExtensions::GetRecipeOutputPath(
						packagePath,
						RecipeExtensions::GetBinaryDirectory(*_compiler, arguments.Configuration),
						std::string(_compiler->GetDynamicLibraryFileExtension()));
					
					RunBuildExtension(libraryPath);
				}
			}

			// Add all dependency packages modules references
			auto includeModules = std::vector<Path>();
			if (recipe.HasDependencies())
			{
				for (auto dependecy : recipe.GetDependencies())
				{
					auto packagePath = RecipeExtensions::GetPackageReferencePath(workingDirectory, dependecy);
					auto modulePath = RecipeExtensions::GetRecipeOutputPath(
						packagePath,
						RecipeExtensions::GetBinaryDirectory(*_compiler, arguments.Configuration),
						std::string(_compiler->GetModuleFileExtension()));
					includeModules.push_back(std::move(modulePath));
				}
			}

			// Add the dependency static library closure to link if targeting an executable
			std::vector<Path> linkLibraries;
			if (recipe.GetType() == RecipeType::Executable)
			{
				RecipeExtensions::GenerateDependecyStaticLibraryClosure(
					*_compiler,
					arguments.Configuration,
					workingDirectory,
					recipe,
					linkLibraries);
			}

			// Build up arguments to build this individual recipe
			auto buildArguments = BuildArguments();
			buildArguments.TargetName = recipe.GetName();
			buildArguments.WorkingDirectory = workingDirectory;
			buildArguments.ObjectDirectory = RecipeExtensions::GetObjectDirectory(*_compiler, arguments.Configuration);
			buildArguments.BinaryDirectory = RecipeExtensions::GetBinaryDirectory(*_compiler, arguments.Configuration);
			buildArguments.ModuleInterfaceSourceFile = 
				recipe.HasPublic() ? recipe.GetPublicAsPath() : Path();
			buildArguments.SourceFiles = recipe.GetSourceAsPath();
			buildArguments.IncludeModules = std::move(includeModules);
			buildArguments.LinkLibraries = std::move(linkLibraries);
			buildArguments.IsIncremental = !arguments.ForceRebuild;
			buildArguments.GenerateSourceDebugInfo = false;
			buildArguments.PreprocessorDefinitions = std::vector<std::string>({
				"SOUP_BUILD",
			});
			buildArguments.IncludeDirectories =
				recipe.HasIncludePaths() ? recipe.GetIncludePathsAsPath() : std::vector<Path>();

			// Set the correct optimization level for the requested configuration
			if (arguments.Configuration == "debug")
			{
				buildArguments.OptimizationLevel = BuildOptimizationLevel::None;
				buildArguments.GenerateSourceDebugInfo = true;
			}
			else if (arguments.Configuration == "release")
			{
				buildArguments.OptimizationLevel = BuildOptimizationLevel::Speed;
			}
			else
			{
				Log::Error("Unknown build configuration type.");
				throw std::runtime_error("Unknown build configuration type.");
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
			auto buildEngine = BuildEngine(_compiler);
			auto wasBuilt = buildEngine.Execute(buildArguments);

			if (wasBuilt)
				Log::Info("Done");
			else
				Log::Info("Up to date");
		}

	private:
		void RunBuildExtension(Path& libraryPath)
		{
			try
			{
				auto buildSystem = BuildSystem();

				Log::Info("Running Build Extension: " + libraryPath.ToString());
				auto library = Platform::DynamicLibraryManager::LoadDynamicLibrary(
					libraryPath.ToString().c_str());
				auto function = (int(*)(BuildEx::IBuildSystem&))library.GetFunction(
					"?RegisterBuildExtension@@YAHAEAVIBuildSystem@BuildEx@Soup@@@Z");
				auto result = function(buildSystem);
				Log::Info("TestDLL Done: " + std::to_string(result));
			}
			catch (const char* message)
			{
				Log::Info(message);
			}
			catch (unsigned long error)
			{
				Log::Info("Failed: " + std::to_string(error));
			}
			catch (...)
			{
				Log::Info("Unknown failure...");
			}
		}

	private:
		std::shared_ptr<ICompiler> _compiler;
	};
}
