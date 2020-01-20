// <copyright file="RecipeBuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace RecipeBuild
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	export class RecipeBuildTask : public Memory::ReferenceCounted<Soup::Build::IBuildTask>
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
		Soup::Build::OperationResult Execute(
			Soup::Build::IBuildState& buildState) noexcept override final
		{
			auto buildStateWrapper = Soup::Build::BuildStateWrapper(buildState);

			try
			{
				return Execute(buildStateWrapper);
			}
			catch (const std::exception& ex)
			{
				buildStateWrapper.LogError(ex.what());
				return -3;
			}
			catch(...)
			{
				// Unknown error
				return -1;
			}
		}

	private:
		/// <summary>
		/// Internal implementation that can throw
		/// </summary>
		Soup::Build::OperationResult Execute(
			Soup::Build::BuildStateWrapper& buildState)
		{
			auto state = buildState.GetActiveState();

			// Load the input properties
			auto compilerName = std::string(state.GetValue("CompilerName").AsString().GetValue());
			auto packageRoot = Path(state.GetValue("PackageRoot").AsString().GetValue());
			auto forceRebuild = state.GetValue("ForceRebuild").AsBoolean().GetValue();
			auto buildFlavor = std::string(state.GetValue("BuildFlavor").AsString().GetValue());
			auto platformLibraries = state.GetValue("PlatformLibraries").AsList().CopyAsPathVector();
			auto platformIncludePaths = state.GetValue("PlatformIncludePaths").AsList().CopyAsPathVector();
			auto platformLibraryPaths = state.GetValue("PlatformLibraryPaths").AsList().CopyAsPathVector();
			auto platformPreprocessorDefinitions = state.GetValue("PlatformPreprocessorDefinitions").AsList().CopyAsStringVector();

			// Load the input recipe
			auto packageRecipePath = packageRoot + Path(Soup::Constants::RecipeFileName);
			Soup::Recipe recipe = {};
			if (!Soup::RecipeExtensions::TryLoadFromFile(packageRecipePath, recipe))
			{
				buildState.LogError("Failed to load the dependency package: " + packageRecipePath.ToString());
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
			auto binaryDirectory = Soup::RecipeExtensions::GetBinaryDirectory(compilerName, buildFlavor);
			auto objectDirectory = Soup::RecipeExtensions::GetObjectDirectory(compilerName, buildFlavor);

			state.EnsureValue("TargetName").SetValueString(recipe.GetName());
			state.EnsureValue("WorkingDirectory").SetValueString(packageRoot.ToString());
			state.EnsureValue("ObjectDirectory").SetValueString(objectDirectory.ToString());
			state.EnsureValue("BinaryDirectory").SetValueString(binaryDirectory.ToString());
			state.EnsureValue("ModuleInterfaceSourceFile").SetValueString("");
			state.EnsureValue("SourceFiles").SetValuePathList(recipe.GetSourceAsPath());
			state.EnsureValue("LinkLibraries").SetValuePathList(linkLibraries);
			state.EnsureValue("IsIncremental").SetValueBoolean(!forceRebuild);
			state.EnsureValue("GenerateSourceDebugInfo").SetValueBoolean(false);
			state.EnsureValue("PreprocessorDefinitions").SetValueStringList(preprocessorDefinitions);
			state.EnsureValue("IncludeDirectories").SetValuePathList(includePaths);
			state.EnsureValue("LibraryPaths").SetValuePathList(libraryPaths);

			if (recipe.HasPublic())
			{
				auto moduleInterfaceSourceFile = recipe.GetPublicAsPath();
				
				// TODO: Clang requires annoying cppm extension
				if (compilerName == "Clang")
				{
					moduleInterfaceSourceFile.SetFileExtension("cppm");
				}

				state.EnsureValue("ModuleInterfaceSourceFile").SetValueString(moduleInterfaceSourceFile.ToString());
			}

			// Set the correct optimization level for the requested flavor
			if (buildFlavor == "debug")
			{
				state.EnsureValue("OptimizationLevel").SetValueInteger(
					static_cast<int64_t>(BuildOptimizationLevel::None));
				state.EnsureValue("GenerateSourceDebugInfo").SetValueBoolean(true);
			}
			else if (buildFlavor == "release")
			{
				state.EnsureValue("OptimizationLevel").SetValueInteger(
					static_cast<int64_t>(BuildOptimizationLevel::Speed));
			}
			else
			{
				buildState.LogError("Unknown build flavor type.");
				throw std::runtime_error("Unknown build flavors type.");
			}

			// Convert the recipe type to the required build type
			BuildTargetType targetType;
			switch (recipe.GetType())
			{
				case Soup::RecipeType::StaticLibrary:
					targetType = BuildTargetType::StaticLibrary;
					break;
				case Soup::RecipeType::DynamicLibrary:
					targetType = BuildTargetType::DynamicLibrary;
					break;
				case Soup::RecipeType::Executable:
					targetType = BuildTargetType::Executable;
					break;
				default:
					throw std::runtime_error("Unknown build target type.");
			}

			state.EnsureValue("TargetType").SetValueInteger(static_cast<int64_t>(targetType));

			// Convert the recipe language version to the required build language
			Soup::LanguageStandard languageStandard;
			switch (recipe.GetLanguageVersion())
			{
				case Soup::RecipeLanguageVersion::CPP11:
					languageStandard = Soup::LanguageStandard::CPP11;
					break;
				case Soup::RecipeLanguageVersion::CPP14:
					languageStandard = Soup::LanguageStandard::CPP14;
					break;
				case Soup::RecipeLanguageVersion::CPP17:
					languageStandard = Soup::LanguageStandard::CPP17;
					break;
				case Soup::RecipeLanguageVersion::CPP20:
					languageStandard = Soup::LanguageStandard::CPP20;
					break;
				default:
					throw std::runtime_error("Unknown recipe language version.");
			}

			state.EnsureValue("LanguageStandard").SetValueInteger(static_cast<int64_t>(languageStandard));

			return 0;
		}
	};
}
