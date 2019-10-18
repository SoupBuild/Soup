// <copyright file="RecipeBuild.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"
#include "RecipeBuildArguments.h"

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

			// Add all dependency packages modules references
			auto includeModules = std::vector<Path>();
			for (auto dependecy : recipe.GetDependencies())
			{
				auto packagePath = RecipeExtensions::GetPackageReferencePath(workingDirectory, dependecy);
				auto modulePath = RecipeExtensions::GetRecipeModulePath(
					packagePath,
					RecipeExtensions::GetBinaryDirectory(*_compiler, arguments.Configuration),
					std::string(_compiler->GetModuleFileExtension()));
				includeModules.push_back(std::move(modulePath));
			}

			// Add the dependency static library closure to link if targeting an executable
			std::vector<Path> linkLibraries;
			if (recipe.GetType() == RecipeType::Executable)
			{
				RecipeExtensions::GenerateDependecyStaticLibraryClosure(
					*_compiler,
					arguments.Configuration,
					workingDirectory,
					recipe.GetDependencies(),
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
			buildArguments.PreprocessorDefinitions = std::vector<std::string>({
				"SOUP_BUILD",
			});
			buildArguments.IncludeDirectories =
				recipe.HasIncludePaths() ? recipe.GetIncludePathsAsPath() : std::vector<Path>();

			// Set the correct optimization level for the requested configuration
			if (arguments.Configuration == "debug")
			{
				buildArguments.OptimizationLevel = BuildOptimizationLevel::None;
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
					buildArguments.TargetType = BuildTargetType::Library;
					break;
				case RecipeType::Executable:
					buildArguments.TargetType = BuildTargetType::Executable;
					break;
				default:
					throw std::runtime_error("Unknown build target type.");
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
		std::shared_ptr<ICompiler> _compiler;
	};
}
