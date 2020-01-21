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

			// Add the dependency static library closure to link if targeting an executable or dynamic library
			std::vector<Path> linkLibraries = std::vector<Path>();

			// Combine the include paths from the recipe and the system
			auto includePaths = std::vector<Path>();
			if (state.HasValue("IncludePaths"))
			{
				includePaths = state.GetValue("IncludePaths").AsList().CopyAsPathVector();
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
			if (state.HasValue("Defines"))
			{
				preprocessorDefinitions = state.GetValue("Defines").AsList().CopyAsStringVector();
			}

			preprocessorDefinitions.insert(
				preprocessorDefinitions.end(),
				platformPreprocessorDefinitions.begin(),
				platformPreprocessorDefinitions.end());
			preprocessorDefinitions.push_back("SOUP_BUILD");

			// Build up arguments to build this individual recipe
			auto binaryDirectory = Soup::Build::RecipeExtensions::GetBinaryDirectory(compilerName, buildFlavor);
			auto objectDirectory = Soup::Build::RecipeExtensions::GetObjectDirectory(compilerName, buildFlavor);

			state.EnsureValue("WorkingDirectory").SetValueString(packageRoot.ToString());
			state.EnsureValue("ObjectDirectory").SetValueString(objectDirectory.ToString());
			state.EnsureValue("BinaryDirectory").SetValueString(binaryDirectory.ToString());
			state.EnsureValue("ModuleInterfaceSourceFile").SetValueString("");
			state.EnsureValue("LinkLibraries").SetValuePathList(linkLibraries);
			state.EnsureValue("IsIncremental").SetValueBoolean(!forceRebuild);
			state.EnsureValue("GenerateSourceDebugInfo").SetValueBoolean(false);
			state.EnsureValue("PreprocessorDefinitions").SetValueStringList(preprocessorDefinitions);
			state.EnsureValue("IncludeDirectories").SetValuePathList(includePaths);
			state.EnsureValue("LibraryPaths").SetValuePathList(libraryPaths);

			if (state.HasValue("Public"))
			{
				auto moduleInterfaceSourceFile = Path(state.GetValue("Public").AsString().GetValue());
				
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
			auto recipeType = Soup::RecipeType::StaticLibrary;
			if (state.HasValue("Type"))
			{
				recipeType = Soup::ParseRecipeType(state.GetValue("Type").AsString().GetValue());
			}

			switch (recipeType)
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
			auto recipeLanguageVersion = Soup::RecipeLanguageVersion::CPP20;
			if (state.HasValue("Language"))
			{
				recipeLanguageVersion =
					Soup::ParseRecipeLanguageVersion(state.GetValue("Language").AsString().GetValue());
			}

			Soup::LanguageStandard languageStandard;
			switch (recipeLanguageVersion)
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
