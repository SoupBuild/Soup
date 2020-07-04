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
		RecipeBuildTask() :
			_runBeforeList({ "Build" }),
			_runAfterList({ "ResolveTools" })
		{
		}

		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() const noexcept override final
		{
			return "RecipeBuild";
		}

		/// <summary>
		/// Get the run before list
		/// </summary>
		const Soup::Build::IReadOnlyList<const char*>& GetRunBeforeList() const noexcept override final
		{
			return _runBeforeList;
		}

		/// <summary>
		/// Get the run after list
		/// </summary>
		const Soup::Build::IReadOnlyList<const char*>& GetRunAfterList() const noexcept override final
		{
			return _runAfterList;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		Soup::Build::ApiCallResult TryExecute(
			Soup::Build::IBuildState& buildState) noexcept override final
		{
			auto buildStateWrapper = Soup::Build::Utilities::BuildStateWrapper(buildState);

			try
			{
				Execute(buildStateWrapper);
				return Soup::Build::ApiCallResult::Success;
			}
			catch (const std::exception& ex)
			{
				buildStateWrapper.LogError(ex.what());
				return Soup::Build::ApiCallResult::Error;
			}
			catch(...)
			{
				// Unknown error
				return Soup::Build::ApiCallResult::Error;
			}
		}

	private:
		/// <summary>
		/// Internal implementation that can throw
		/// </summary>
		void Execute(Soup::Build::Utilities::BuildStateWrapper& buildState)
		{
			auto rootTable = buildState.GetActiveState();
			auto recipeTable = rootTable.GetValue("Recipe").AsTable();
			auto buildTable = rootTable.EnsureValue("Build").EnsureTable();

			// Load the input properties
			auto compilerName = std::string(rootTable.GetValue("CompilerName").AsString().GetValue());
			auto packageRoot = Path(rootTable.GetValue("PackageRoot").AsString().GetValue());
			auto buildFlavor = std::string(rootTable.GetValue("BuildFlavor").AsString().GetValue());
			auto platformLibraries = rootTable.GetValue("PlatformLibraries").AsList().CopyAsPathVector();
			auto platformIncludePaths = rootTable.GetValue("PlatformIncludePaths").AsList().CopyAsPathVector();
			auto platformLibraryPaths = rootTable.GetValue("PlatformLibraryPaths").AsList().CopyAsPathVector();
			auto platformPreprocessorDefinitions = rootTable.GetValue("PlatformPreprocessorDefinitions").AsList().CopyAsStringVector();

			// Load Recipe properties
			auto name = std::string(recipeTable.GetValue("Name").AsString().GetValue());

			// Add the dependency static library closure to link if targeting an executable or dynamic library
			std::vector<Path> linkLibraries = std::vector<Path>();
			if (recipeTable.HasValue("LinkLibraries"))
			{
				linkLibraries = recipeTable.GetValue("LinkLibraries").AsList().CopyAsPathVector();
			}

			// Add the dependency runtime dependencies closure if present
			if (recipeTable.HasValue("RuntimeDependencies"))
			{
				std::vector<Path> runtimeDependencies = std::vector<Path>();
				if (buildTable.HasValue("RuntimeDependencies"))
				{
					runtimeDependencies = buildTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector();
				}

				auto recipeRuntimeDependencies = recipeTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector();
				std::copy(
					recipeRuntimeDependencies.begin(),
					recipeRuntimeDependencies.end(),
					std::back_inserter(runtimeDependencies)); 

				buildTable.EnsureValue("RuntimeDependencies").SetValuePathList(runtimeDependencies);
			}

			// Combine the include paths from the recipe and the system
			auto includePaths = std::vector<Path>();
			if (recipeTable.HasValue("IncludePaths"))
			{
				includePaths = recipeTable.GetValue("IncludePaths").AsList().CopyAsPathVector();
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
			if (recipeTable.HasValue("Defines"))
			{
				preprocessorDefinitions = recipeTable.GetValue("Defines").AsList().CopyAsStringVector();
			}

			preprocessorDefinitions.insert(
				preprocessorDefinitions.end(),
				platformPreprocessorDefinitions.begin(),
				platformPreprocessorDefinitions.end());
			preprocessorDefinitions.push_back("SOUP_BUILD");

			// Build up arguments to build this individual recipe
			auto binaryDirectory = rootTable.GetValue("BinaryDirectory").AsString().GetValue();
			auto objectDirectory = rootTable.GetValue("ObjectDirectory").AsString().GetValue();

			// Load the module interface file if present
			auto moduleInterfaceSourceFile = std::string();
			if (recipeTable.HasValue("Public"))
			{
				auto moduleInterfaceSourceFilePath = Path(recipeTable.GetValue("Public").AsString().GetValue());
				
				// TODO: Clang requires annoying cppm extension
				if (compilerName == "Clang")
				{
					moduleInterfaceSourceFilePath.SetFileExtension("cppm");
				}

				moduleInterfaceSourceFile = moduleInterfaceSourceFilePath.ToString();
			}

			// Load the source files if present
			auto sourceFiles = std::vector<std::string>();
			if (recipeTable.HasValue("Source"))
			{
				sourceFiles = recipeTable.GetValue("Source").AsList().CopyAsStringVector();
			}
			// Set the correct optimization level for the requested flavor
			auto optimizationLevel = Soup::Compiler::BuildOptimizationLevel::None;
			bool generateSourceDebugInfo = false;
			if (buildFlavor == "debug")
			{
				// preprocessorDefinitions.push_back("DEBUG");
				generateSourceDebugInfo = true;
			}
			else if (buildFlavor == "release")
			{
				preprocessorDefinitions.push_back("RELEASE");
				optimizationLevel = Soup::Compiler::BuildOptimizationLevel::Speed;
			}
			else
			{
				buildState.LogError("Unknown build flavor type.");
				throw std::runtime_error("Unknown build flavors type.");
			}

			buildTable.EnsureValue("TargetName").SetValueString(name);
			buildTable.EnsureValue("WorkingDirectory").SetValueString(packageRoot.ToString());
			buildTable.EnsureValue("ObjectDirectory").SetValueString(objectDirectory);
			buildTable.EnsureValue("BinaryDirectory").SetValueString(binaryDirectory);
			buildTable.EnsureValue("ModuleInterfaceSourceFile").SetValueString(moduleInterfaceSourceFile);
			buildTable.EnsureValue("OptimizationLevel").SetValueInteger(static_cast<int64_t>(optimizationLevel));
			buildTable.EnsureValue("GenerateSourceDebugInfo").SetValueBoolean(generateSourceDebugInfo);

			buildTable.EnsureValue("PlatformLibraries").EnsureList().Append(platformLibraries);
			buildTable.EnsureValue("LinkLibraries").EnsureList().Append(linkLibraries);
			buildTable.EnsureValue("PreprocessorDefinitions").EnsureList().Append(preprocessorDefinitions);
			buildTable.EnsureValue("IncludeDirectories").EnsureList().Append(includePaths);
			buildTable.EnsureValue("LibraryPaths").EnsureList().Append(libraryPaths);
			buildTable.EnsureValue("Source").EnsureList().Append(sourceFiles);

			// Convert the recipe type to the required build type
			Soup::Compiler::BuildTargetType targetType;
			auto recipeType = Soup::Build::Utilities::RecipeType::StaticLibrary;
			if (recipeTable.HasValue("Type"))
			{
				recipeType = Soup::Build::Utilities::ParseRecipeType(recipeTable.GetValue("Type").AsString().GetValue());
			}

			switch (recipeType)
			{
				case Soup::Build::Utilities::RecipeType::StaticLibrary:
					targetType = Soup::Compiler::BuildTargetType::StaticLibrary;
					break;
				case Soup::Build::Utilities::RecipeType::DynamicLibrary:
					targetType = Soup::Compiler::BuildTargetType::DynamicLibrary;
					break;
				case Soup::Build::Utilities::RecipeType::Executable:
					targetType = Soup::Compiler::BuildTargetType::Executable;
					break;
				default:
					throw std::runtime_error("Unknown build target type.");
			}

			buildTable.EnsureValue("TargetType").SetValueInteger(static_cast<int64_t>(targetType));

			// Convert the recipe language version to the required build language
			auto recipeLanguageVersion = Soup::Build::Utilities::RecipeLanguageVersion::CPP20;
			if (recipeTable.HasValue("Language"))
			{
				recipeLanguageVersion = Soup::Build::Utilities::ParseRecipeLanguageVersion(
					recipeTable.GetValue("Language").AsString().GetValue());
			}

			Soup::Compiler::LanguageStandard languageStandard;
			switch (recipeLanguageVersion)
			{
				case Soup::Build::Utilities::RecipeLanguageVersion::CPP11:
					languageStandard = Soup::Compiler::LanguageStandard::CPP11;
					break;
				case Soup::Build::Utilities::RecipeLanguageVersion::CPP14:
					languageStandard = Soup::Compiler::LanguageStandard::CPP14;
					break;
				case Soup::Build::Utilities::RecipeLanguageVersion::CPP17:
					languageStandard = Soup::Compiler::LanguageStandard::CPP17;
					break;
				case Soup::Build::Utilities::RecipeLanguageVersion::CPP20:
					languageStandard = Soup::Compiler::LanguageStandard::CPP20;
					break;
				default:
					throw std::runtime_error("Unknown recipe language version.");
			}

			buildTable.EnsureValue("LanguageStandard").SetValueInteger(static_cast<int64_t>(languageStandard));
		}

	private:
		Soup::Build::Utilities::StringList _runBeforeList;
		Soup::Build::Utilities::StringList _runAfterList;
	};
}
