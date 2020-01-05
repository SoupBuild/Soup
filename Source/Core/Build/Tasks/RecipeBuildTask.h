// <copyright file="RecipeBuildTask.h" company="Soup">
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
		/// Get the task name
		/// </summary>
		const char* GetName() const noexcept override final
		{
			return "RecipeBuild";
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		OperationResult Execute(IBuildState& buildState) noexcept override final
		{
			try
			{
				auto state = PropertyBagWrapper(buildState.GetActiveState());

				// Load the input properties
				auto compilerName = std::string(state.GetPropertyStringValue("CompilerName"));
				auto packageRoot = Path(state.GetPropertyStringValue("PackageRoot"));
				auto forceRebuild = state.GetPropertyBooleanValue("ForceRebuild");
				auto buildFlavor = std::string(state.GetPropertyStringValue("BuildFlavor"));
				auto platformLibraries = state.CopyPropertyStringListAsPathVector("PlatformLibraries");
				auto platformIncludePaths = state.CopyPropertyStringListAsPathVector("PlatformIncludePaths");
				auto platformLibraryPaths = state.CopyPropertyStringListAsPathVector("PlatformLibraryPaths");
				auto platformPreprocessorDefinitions = state.CopyPropertyStringListAsStringVector("PlatformPreprocessorDefinitions");

				// Load the input recipe
				auto packageRecipePath = packageRoot + Path(Constants::RecipeFileName);
				Recipe recipe = {};
				if (!RecipeExtensions::TryLoadFromFile(packageRecipePath, recipe))
				{
					Log::Error("Failed to load the dependency package: " + packageRecipePath.ToString());
					return -2;
				}

				// Add the dependency static library closure to link if targeting an executable or dynamic library
				std::vector<Path> linkLibraries = std::vector<Path>();

				// Combine the include paths from the recipe and the system
				auto includePaths = std::vector<Path>();
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
					platformIncludePaths.begin(),
					platformIncludePaths.end());

				// Load the extra library paths provided to the build system
				auto libraryPaths = std::vector<Path>();

				// Add the platform library paths
				libraryPaths.insert(
					libraryPaths.end(),
					platformLibraryPaths.begin(),
					platformLibraryPaths.end());

				// Combine the defines with the default set and the platform
				auto preprocessorDefinitions = std::vector<std::string>();
				preprocessorDefinitions.insert(
					preprocessorDefinitions.end(),
					platformPreprocessorDefinitions.begin(),
					platformPreprocessorDefinitions.end());
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
				auto binaryDirectory = RecipeExtensions::GetBinaryDirectory(compilerName, buildFlavor);
				auto objectDirectory = RecipeExtensions::GetObjectDirectory(compilerName, buildFlavor);

				state.SetPropertyStringValue("TargetName", recipe.GetName());
				state.SetPropertyStringValue("WorkingDirectory", packageRoot.ToString());
				state.SetPropertyStringValue("ObjectDirectory", objectDirectory.ToString());
				state.SetPropertyStringValue("BinaryDirectory", binaryDirectory.ToString());
				state.SetPropertyStringValue("ModuleInterfaceSourceFile", "");
				state.SetPropertyStringList("SourceFiles", recipe.GetSourceAsPath());
				state.SetPropertyStringList("LinkLibraries", linkLibraries);
				state.SetPropertyBooleanValue("IsIncremental", !forceRebuild);
				state.SetPropertyBooleanValue("GenerateSourceDebugInfo", false);
				state.SetPropertyStringList("PreprocessorDefinitions", preprocessorDefinitions);
				state.SetPropertyStringList("IncludeDirectories", includePaths);
				state.SetPropertyStringList("LibraryPaths", libraryPaths);

				if (recipe.HasPublic())
				{
					auto moduleInterfaceSourceFile = recipe.GetPublicAsPath();
					
					// TODO: Clang requires annoying cppm extension
					if (compilerName == "Clang")
					{
						moduleInterfaceSourceFile.SetFileExtension("cppm");
					}

					state.SetPropertyStringValue("ModuleInterfaceSourceFile", moduleInterfaceSourceFile.ToString());
				}

				// Set the correct optimization level for the requested flavor
				if (buildFlavor == "debug")
				{
					state.SetPropertyIntegerValue(
						"OptimizationLevel",
						static_cast<int64_t>(BuildOptimizationLevel::None));
					state.SetPropertyBooleanValue("GenerateSourceDebugInfo", true);
				}
				else if (buildFlavor == "release")
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
				switch (recipe.GetType())
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
				switch (recipe.GetLanguageVersion())
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
	};
}
