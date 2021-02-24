// <copyright file="RecipeBuildTask.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Cpp
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	public class RecipeBuildTask : IBuildTask
	{
		public RecipeBuildTask()
		{
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		public void Execute()
		{
			auto rootTable = buildState.GetActiveState();
			auto parametersTable = rootTable.GetValue("Parameters").AsTable();
			auto recipeTable = rootTable.GetValue("Recipe").AsTable();
			auto buildTable = rootTable.EnsureValue("Build").EnsureTable();

			// Load the input properties
			auto compilerName = string(parametersTable.GetValue("Compiler").AsString().GetValue());
			auto packageRoot = Path(parametersTable.GetValue("PackageDirectory").AsString().GetValue());
			auto buildFlavor = string(parametersTable.GetValue("Flavor").AsString().GetValue());
			auto platformLibraries = rootTable.GetValue("PlatformLibraries").AsList().CopyAsPathVector();
			auto platformIncludePaths = rootTable.GetValue("PlatformIncludePaths").AsList().CopyAsPathVector();
			auto platformLibraryPaths = rootTable.GetValue("PlatformLibraryPaths").AsList().CopyAsPathVector();
			auto platformPreprocessorDefinitions = rootTable.GetValue("PlatformPreprocessorDefinitions").AsList().CopyAsStringVector();

			// Load Recipe properties
			auto name = string(recipeTable.GetValue("Name").AsString().GetValue());

			// Add any explicit platform dependencies that were added in the recipe
			if (recipeTable.HasValue("PlatformLibraries"))
			{
				for (auto value : recipeTable.GetValue("PlatformLibraries").AsList().CopyAsPathVector())
				{
					platformLibraries.push_back(std::move(value));
				}
			}

			// Add the dependency static library closure to link if targeting an executable or dynamic library
			std::vector<Path> linkLibraries = std::vector<Path>();
			if (recipeTable.HasValue("LinkLibraries"))
			{
				for (auto value : recipeTable.GetValue("LinkLibraries").AsList().CopyAsPathVector())
				{
					// If relative then resolve to working directory
					if (value.HasRoot())
					{
						linkLibraries.push_back(std::move(value));
					}
					else
					{
						linkLibraries.push_back(packageRoot + value);
					}
				}
			}

			// Add the dependency runtime dependencies closure if present
			if (recipeTable.HasValue("RuntimeDependencies"))
			{
				std::vector<Path> runtimeDependencies = std::vector<Path>();
				if (buildTable.HasValue("RuntimeDependencies"))
				{
					runtimeDependencies = buildTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector();
				}

				for (auto value : recipeTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector())
				{
					// If relative then resolve to working directory
					if (value.HasRoot())
					{
						runtimeDependencies.push_back(std::move(value));
					}
					else
					{
						runtimeDependencies.push_back(packageRoot + value);
					}
				}

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
			auto preprocessorDefinitions = std::vector<string>();
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
			auto targetDirectory = Path(parametersTable.GetValue("TargetDirectory").AsString().GetValue());
			auto binaryDirectory = targetDirectory + Path("bin/");
			auto objectDirectory = targetDirectory + Path("obj/");

			// Load the module interface file if present
			auto moduleInterfaceSourceFile = string();
			if (recipeTable.HasValue("Interface"))
			{
				auto moduleInterfaceSourceFilePath = Path(recipeTable.GetValue("Interface").AsString().GetValue());
				
				// TODO: Clang requires annoying cppm extension
				if (compilerName == "Clang")
				{
					moduleInterfaceSourceFilePath.SetFileExtension("cppm");
				}

				moduleInterfaceSourceFile = moduleInterfaceSourceFilePath.ToString();
			}

			// Load the source files if present
			auto sourceFiles = std::vector<string>();
			if (recipeTable.HasValue("Source"))
			{
				sourceFiles = recipeTable.GetValue("Source").AsList().CopyAsStringVector();
			}

			// Check for warning settings
			bool enableWarningsAsErrors = true;
			if (recipeTable.HasValue("EnableWarningsAsErrors"))
			{
				enableWarningsAsErrors = recipeTable.GetValue("EnableWarningsAsErrors").AsBoolean().GetValue();
			}

			// Set the correct optimization level for the requested flavor
			auto optimizationLevel = Soup::Cpp::Compiler::BuildOptimizationLevel::None;
			bool generateSourceDebugInfo = false;
			if (buildFlavor == "debug")
			{
				// preprocessorDefinitions.push_back("DEBUG");
				generateSourceDebugInfo = true;
			}
			else if (buildFlavor == "debugrelease")
			{
				preprocessorDefinitions.push_back("RELEASE");
				generateSourceDebugInfo = true;
				optimizationLevel = Soup::Cpp::Compiler::BuildOptimizationLevel::Speed;
			}
			else if (buildFlavor == "release")
			{
				preprocessorDefinitions.push_back("RELEASE");
				optimizationLevel = Soup::Cpp::Compiler::BuildOptimizationLevel::Speed;
			}
			else
			{
				buildState.LogError("Unknown build flavor type.");
				throw std::runtime_error("Unknown build flavors type.");
			}

			buildTable.EnsureValue("TargetName").SetValueString(name);
			buildTable.EnsureValue("WorkingDirectory").SetValueString(packageRoot.ToString());
			buildTable.EnsureValue("ObjectDirectory").SetValueString(objectDirectory.ToString());
			buildTable.EnsureValue("BinaryDirectory").SetValueString(binaryDirectory.ToString());
			buildTable.EnsureValue("ModuleInterfaceSourceFile").SetValueString(moduleInterfaceSourceFile);
			buildTable.EnsureValue("OptimizationLevel").SetValueInteger(static_cast<int64_t>(optimizationLevel));
			buildTable.EnsureValue("GenerateSourceDebugInfo").SetValueBoolean(generateSourceDebugInfo);

			buildTable.EnsureValue("PlatformLibraries").EnsureList().Append(platformLibraries);
			buildTable.EnsureValue("LinkLibraries").EnsureList().Append(linkLibraries);
			buildTable.EnsureValue("PreprocessorDefinitions").EnsureList().Append(preprocessorDefinitions);
			buildTable.EnsureValue("IncludeDirectories").EnsureList().Append(includePaths);
			buildTable.EnsureValue("LibraryPaths").EnsureList().Append(libraryPaths);
			buildTable.EnsureValue("Source").EnsureList().Append(sourceFiles);

			buildTable.EnsureValue("EnableWarningsAsErrors").SetValueBoolean(enableWarningsAsErrors);

			// Convert the recipe type to the required build type
			auto targetType = Soup::Cpp::Compiler::BuildTargetType::StaticLibrary;
			if (recipeTable.HasValue("Type"))
			{
				targetType = ParseType(recipeTable.GetValue("Type").AsString().GetValue());
			}

			buildTable.EnsureValue("TargetType").SetValueInteger(static_cast<int64_t>(targetType));

			// Convert the recipe language version to the required build language
			auto languageStandard = Soup::Cpp::Compiler::LanguageStandard::CPP20;
			if (recipeTable.HasValue("LanguageVersion"))
			{
				languageStandard = ParseLanguageStandard(
					recipeTable.GetValue("LanguageVersion").AsString().GetValue());
			}

			buildTable.EnsureValue("LanguageStandard").SetValueInteger(static_cast<int64_t>(languageStandard));
		}

		static Soup::Cpp::Compiler::BuildTargetType ParseType(string_view value)
		{
			if (value == "Executable")
				return Soup::Cpp::Compiler::BuildTargetType::Executable;
			else if (value == "StaticLibrary")
				return Soup::Cpp::Compiler::BuildTargetType::StaticLibrary;
			else if (value == "DynamicLibrary")
				return Soup::Cpp::Compiler::BuildTargetType::DynamicLibrary;
			else
				throw std::runtime_error("Unknown target type value.");
		}

		private static Soup::Cpp::Compiler::LanguageStandard ParseLanguageStandard(const string& value)
		{
			if (value == "C++11")
				return Soup::Cpp::Compiler::LanguageStandard::CPP11;
			else if (value == "C++14")
				return Soup::Cpp::Compiler::LanguageStandard::CPP14;
			else if (value == "C++17")
				return Soup::Cpp::Compiler::LanguageStandard::CPP17;
			else if (value == "C++20")
				return Soup::Cpp::Compiler::LanguageStandard::CPP20;
			else
				throw std::runtime_error("Unknown recipe language standard value.");
		}
	}
}
