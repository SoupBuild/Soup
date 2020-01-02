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
	export class RecipeBuildTask : public Memory::ReferenceCounted<IBuildTask>
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
		const char* GetName() const noexcept override final
		{
			return "RecipeBuild";
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		BuildSystemResult Execute(IBuildState& buildState) noexcept override final
		{
			try
			{
				auto state = BuildStateWrapper(buildState);

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

				state.SetPropertyStringValue("TargetName", _recipe.GetName());
				state.SetPropertyStringValue("WorkingDirectory", _workingDirectory.ToString());
				state.SetPropertyStringValue(
					"ObjectDirectory",
					RecipeExtensions::GetObjectDirectory(*_activeCompiler, _arguments.Flavor).ToString());
				state.SetPropertyStringValue("BinaryDirectory", binaryDirectory.ToString());
				state.SetPropertyStringValue("ModuleInterfaceSourceFile", "");
				state.SetPropertyStringList("SourceFiles", _recipe.GetSourceAsPath());
				state.SetPropertyStringList("IncludeModules", includeModules);
				state.SetPropertyStringList("LinkLibraries", linkLibraries);
				state.SetPropertyStringList("ExternalLinkLibraries", externalLinkLibraries);
				state.SetPropertyBooleanValue("IsIncremental", !_arguments.ForceRebuild);
				state.SetPropertyBooleanValue("GenerateSourceDebugInfo", false);
				state.SetPropertyStringList("PreprocessorDefinitions", preprocessorDefinitions);
				state.SetPropertyStringList("IncludeDirectories", includePaths);
				state.SetPropertyStringList("LibraryPaths", libraryPaths);

				if (_recipe.HasPublic())
				{
					auto moduleInterfaceSourceFile = _recipe.GetPublicAsPath();
					
					// TODO: Clang requires annoying cppm extension
					if (_activeCompiler->GetName() == "Clang")
					{
						moduleInterfaceSourceFile.SetFileExtension("cppm");
					}

					state.SetPropertyStringValue("ModuleInterfaceSourceFile", moduleInterfaceSourceFile.ToString());
				}

				// Set the correct optimization level for the requested flavor
				if (_arguments.Flavor == "debug")
				{
					state.SetPropertyIntegerValue(
						"OptimizationLevel",
						static_cast<int64_t>(BuildOptimizationLevel::None));
					state.SetPropertyBooleanValue("GenerateSourceDebugInfo", true);
				}
				else if (_arguments.Flavor == "release")
				{
					state.SetPropertyIntegerValue(
						"OptimizationLevel",
						static_cast<int64_t>(BuildOptimizationLevel::Speed));
				}
				else
				{
					Log::Error("Unknown build flavor type.");
					throw std::runtime_error("Unknown build flavors type.");
				}

				// Convert the recipe type to the required build type
				BuildTargetType targetType;
				switch (_recipe.GetType())
				{
					case RecipeType::StaticLibrary:
						targetType = BuildTargetType::StaticLibrary;
						break;
					case RecipeType::DynamicLibrary:
						targetType = BuildTargetType::DynamicLibrary;
						break;
					case RecipeType::Executable:
						targetType = BuildTargetType::Executable;
						break;
					default:
						throw std::runtime_error("Unknown build target type.");
				}

				state.SetPropertyIntegerValue("TargetType", static_cast<int64_t>(targetType));

				// Convert the recipe language version to the required build language
				LanguageStandard languageStandard;
				switch (_recipe.GetLanguageVersion())
				{
					case RecipeLanguageVersion::CPP11:
						languageStandard = LanguageStandard::CPP11;
						break;
					case RecipeLanguageVersion::CPP14:
						languageStandard = LanguageStandard::CPP14;
						break;
					case RecipeLanguageVersion::CPP17:
						languageStandard = LanguageStandard::CPP17;
						break;
					case RecipeLanguageVersion::CPP20:
						languageStandard = LanguageStandard::CPP20;
						break;
					default:
						throw std::runtime_error("Unknown recipe language version.");
				}

				state.SetPropertyIntegerValue("LanguageStandard", static_cast<int64_t>(languageStandard));

				return 0;
			}
			catch(...)
			{
				// Unknown error
				return -1;
			}
		}

	private:
		std::shared_ptr<ICompiler> _systemCompiler;
		std::shared_ptr<ICompiler> _activeCompiler;
		Path _workingDirectory;
		Recipe _recipe;
		RecipeBuildArguments _arguments;
	};
}
