// <copyright file="RecipeBuild.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "RecipeExtensions.h"

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
			int projectId,
			const Path& workingDirectory,
			const Recipe& recipe,
			bool forceBuild)
		{
			Log::SetActiveId(projectId);
			Log::Info("Building '" + recipe.GetName() + "'");

			// Add all dependency packages modules references
			auto includeModules = std::vector<Path>();
			for (auto dependecy : recipe.GetDependencies())
			{
				auto packagePath = RecipeExtensions::GetPackageReferencePath(workingDirectory, dependecy);
				auto modulePath = RecipeExtensions::GetRecipeModulePath(
					packagePath,
					RecipeExtensions::GetBinaryDirectory(*_compiler),
					std::string(_compiler->GetModuleFileExtension()));
				includeModules.push_back(std::move(modulePath));
			}

			// Add the dependency static library closure to link if targeting an executable
			std::vector<Path> linkLibraries;
			if (recipe.GetType() == RecipeType::Executable)
			{
				RecipeExtensions::GenerateDependecyStaticLibraryClosure(
					*_compiler,
					workingDirectory,
					recipe.GetDependencies(),
					linkLibraries);
			}

			// Build up arguments to build this individual recipe
			auto arguments = BuildArguments();
			arguments.TargetName = recipe.GetName();
			arguments.WorkingDirectory = workingDirectory;
			arguments.ObjectDirectory = RecipeExtensions::GetObjectDirectory(*_compiler);
			arguments.BinaryDirectory = RecipeExtensions::GetBinaryDirectory(*_compiler);
			arguments.ModuleInterfaceSourceFile = 
				recipe.HasPublic() ? recipe.GetPublicAsPath() : Path();
			arguments.SourceFiles = recipe.GetSourceAsPath();
			arguments.IncludeModules = std::move(includeModules);
			arguments.LinkLibraries = std::move(linkLibraries);
			arguments.IsIncremental = !forceBuild;
			arguments.PreprocessorDefinitions = std::vector<std::string>({
				"SOUP_BUILD",
			});
			arguments.IncludeDirectories =
				recipe.HasIncludePaths() ? recipe.GetIncludePathsAsPath() : std::vector<Path>();

			// Convert the recipe type to the required build type
			switch (recipe.GetType())
			{
				case RecipeType::Library:
					arguments.TargetType = BuildTargetType::Library;
					break;
				case RecipeType::Executable:
					arguments.TargetType = BuildTargetType::Executable;
					break;
				default:
					throw std::runtime_error("Unknown build target type.");
			}

			// Perform the build
			auto buildEngine = BuildEngine(_compiler);
			auto wasBuilt = buildEngine.Execute(arguments);

			if (wasBuilt)
				Log::Info("Done");
			else
				Log::Info("Up to date");
		}

	private:
		std::shared_ptr<ICompiler> _compiler;
	};
}
