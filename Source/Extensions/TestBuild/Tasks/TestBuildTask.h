#pragma once

namespace Soup::Test
{
	using CreateCompiler = std::function<std::shared_ptr<Soup::Compiler::ICompiler>(Soup::Build::Utilities::ValueTableWrapper&)>;
	using CompilerFactory = std::map<std::string, CreateCompiler>;

	/// <summary>
	/// The simple build task that will run after the main build task
	/// </summary>
	class TestBuildTask : public Memory::ReferenceCounted<Soup::Build::IBuildTask>
	{
	public:
		TestBuildTask(CompilerFactory compilerFactory) :
			_compilerFactory(std::move(compilerFactory))
		{
		}

		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() const noexcept override final
		{
			return "TestBuild";
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
				// We cannot throw accross the DLL boundary for a build extension
				// So all internal errors must be converted to error codes
				Execute(buildStateWrapper);
				return Soup::Build::ApiCallResult::Success;
			}
			catch(...)
			{
				buildStateWrapper.LogError("Unknown Error");
				return Soup::Build::ApiCallResult::Error;
			}
		}

	private:
		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(Soup::Build::Utilities::BuildStateWrapper& buildState)
		{
			auto activeState = buildState.GetActiveState();
			auto sharedState = buildState.GetSharedState();
			auto recipeTable = activeState.GetValue("Recipe").AsTable();

			if (!recipeTable.HasValue("Tests"))
			{
				buildState.LogError("No Tests Specified");
				throw std::runtime_error("");
			}

			auto arguments = Soup::Compiler::BuildArguments();

			// Load up the common build properties from the original Build table in the active state
			auto activeBuildTable = activeState.GetValue("Build").AsTable();
			LoadBuildProperties(activeBuildTable, arguments);

			// Load the test properties
			auto testTable = recipeTable.GetValue("Tests").AsTable();
			LoadTestBuildProperties(buildState, testTable, arguments);

			// Load up the input build parameters from the shared build state as if
			// this is a dependency build
			auto sharedBuildTable = sharedState.GetValue("Build").AsTable();
			LoadDependencyBuildInput(sharedBuildTable, arguments);

			// Load up the dev dependencies build input to add extra test runtime libraries
			LoadDevDependencyBuildInput(buildState, activeState, arguments);

			// Initialize the compiler to use
			auto compilerName = std::string(activeState.GetValue("CompilerName").AsString().GetValue());
			auto findCompilerFactory = _compilerFactory.find(compilerName);
			if (findCompilerFactory == _compilerFactory.end())
			{
				buildState.LogError("Unknown compiler: " + compilerName);
				throw new std::runtime_error("");
			}

			auto createCompiler = findCompilerFactory->second;
			auto compiler = createCompiler(activeState);

			auto buildEngine = Soup::Compiler::BuildEngine(compiler);
			auto buildResult = buildEngine.Execute(buildState, arguments);

			// Create the operation to run tests during build
			auto title = "Run Tests";
			auto program = 
				arguments.WorkingDirectory +
				arguments.BinaryDirectory +
				Path(arguments.TargetName);
			program.SetFileExtension("exe");
			auto workingDirectory = Path("");
			auto runArguments = "";
			auto inputFiles = std::vector<Path>({});
			auto outputFiles = std::vector<Path>({});
			auto runTestsOperation = Build::Utilities::BuildOperationWrapper(
				new Build::Utilities::BuildOperation(
					title,
					program,
					runArguments,
					workingDirectory,
					inputFiles,
					outputFiles));

			// Run the test harness
			Soup::Build::Utilities::BuildOperationExtensions::AddLeafChild(
				buildResult.BuildOperations,
				runTestsOperation);

			// Register the root build tasks
			buildState.GetRootOperationList().Append(buildResult.BuildOperations);
		}

		void LoadBuildProperties(
			Soup::Build::Utilities::ValueTableWrapper& buildTable,
			Soup::Compiler::BuildArguments& arguments)
		{
			arguments.LanguageStandard = static_cast<Soup::Compiler::LanguageStandard>(
				buildTable.GetValue("LanguageStandard").AsInteger().GetValue());
			arguments.WorkingDirectory = Path(buildTable.GetValue("WorkingDirectory").AsString().GetValue());
			arguments.ObjectDirectory = Path(buildTable.GetValue("ObjectDirectory").AsString().GetValue());
			arguments.BinaryDirectory = Path(buildTable.GetValue("BinaryDirectory").AsString().GetValue());

			if (buildTable.HasValue("IncludeDirectories"))
			{
				arguments.IncludeDirectories =
					buildTable.GetValue("IncludeDirectories").AsList().CopyAsPathVector();
			}

			if (buildTable.HasValue("PlatformLibraries"))
			{
				arguments.PlatformLinkDependencies =
					buildTable.GetValue("PlatformLibraries").AsList().CopyAsPathVector();
			}

			if (buildTable.HasValue("LinkLibraries"))
			{
				arguments.LinkDependencies =
					buildTable.GetValue("LinkLibraries").AsList().CopyAsPathVector();
			}

			if (buildTable.HasValue("LibraryPaths"))
			{
				arguments.LibraryPaths =
					buildTable.GetValue("LibraryPaths").AsList().CopyAsPathVector();
			}

			if (buildTable.HasValue("PreprocessorDefinitions"))
			{
				arguments.PreprocessorDefinitions =
					buildTable.GetValue("PreprocessorDefinitions").AsList().CopyAsStringVector();
			}

			if (buildTable.HasValue("OptimizationLevel"))
			{
				arguments.OptimizationLevel = static_cast<Soup::Compiler::BuildOptimizationLevel>(
					buildTable.GetValue("OptimizationLevel").AsInteger().GetValue());
			}
			else
			{
				arguments.OptimizationLevel = Soup::Compiler::BuildOptimizationLevel::None;
			}

			if (buildTable.HasValue("GenerateSourceDebugInfo"))
			{
				arguments.GenerateSourceDebugInfo =
					buildTable.GetValue("GenerateSourceDebugInfo").AsBoolean().GetValue();
			}
			else
			{
				arguments.GenerateSourceDebugInfo = false;
			}
		}

		void LoadTestBuildProperties(
			Soup::Build::Utilities::BuildStateWrapper& buildState,
			Soup::Build::Utilities::ValueTableWrapper& testTable,
			Soup::Compiler::BuildArguments& arguments)
		{
			if (!testTable.HasValue("Source"))
			{
				buildState.LogError("No Test Source Files");
				throw std::runtime_error("");
			}

			arguments.SourceFiles =
				testTable.GetValue("Source").AsList().CopyAsPathVector();

			// Combine the include paths from the recipe and the system
			if (testTable.HasValue("IncludePaths"))
			{
				arguments.IncludeDirectories = CombineUnique(
					arguments.IncludeDirectories,
					testTable.GetValue("IncludePaths").AsList().CopyAsPathVector());
			}

			arguments.TargetName = "TestHarness";
			arguments.TargetType = Soup::Compiler::BuildTargetType::Executable;
		}

		void LoadDependencyBuildInput(
			Soup::Build::Utilities::ValueTableWrapper& buildTable,
			Soup::Compiler::BuildArguments& arguments)
		{
			// Load the runtime dependencies
			if (buildTable.HasValue("RuntimeDependencies"))
			{
				arguments.RuntimeDependencies = MakeUnique(
					buildTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector());
			}

			// Load the link dependencies
			if (buildTable.HasValue("LinkDependencies"))
			{
				arguments.LinkDependencies = MakeUnique(
					buildTable.GetValue("LinkDependencies").AsList().CopyAsPathVector());
			}

			// Load the module references
			if (buildTable.HasValue("ModuleDependencies"))
			{
				arguments.ModuleDependencies = MakeUnique(
					buildTable.GetValue("ModuleDependencies").AsList().CopyAsPathVector());
			}
		}

		static void LoadDevDependencyBuildInput(
			Soup::Build::Utilities::BuildStateWrapper& buildState,
			Soup::Build::Utilities::ValueTableWrapper& activeState,
			Soup::Compiler::BuildArguments& arguments)
		{
			if (activeState.HasValue("DevDependencies"))
			{
				auto dependenciesTable = activeState.GetValue("DevDependencies").AsTable();

				for (auto& dependencyName : dependenciesTable.GetValueKeyList().CopyAsStringVector())
				{
					// Combine the core dependency build inputs for the core build task
					buildState.LogInfo("Combine DevDependency: " + dependencyName);
					auto dependencyTable = dependenciesTable.GetValue(dependencyName).AsTable();

					if (dependencyTable.HasValue("Build"))
					{
						auto dependencyBuildTable = dependencyTable.GetValue("Build").AsTable();

						if (dependencyBuildTable.HasValue("ModuleDependencies"))
						{
							auto moduleDependencies = dependencyBuildTable
								.GetValue("ModuleDependencies")
								.AsList()
								.CopyAsStringVector();

							arguments.ModuleDependencies = CombineUnique(
								arguments.ModuleDependencies,
								dependencyBuildTable.GetValue("ModuleDependencies").AsList().CopyAsPathVector());
						}

						if (dependencyBuildTable.HasValue("RuntimeDependencies"))
						{
							auto runtimeDependencies = dependencyBuildTable
								.GetValue("RuntimeDependencies")
								.AsList()
								.CopyAsStringVector();

							arguments.RuntimeDependencies = CombineUnique(
								arguments.RuntimeDependencies,
								dependencyBuildTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector());
						}

						if (dependencyBuildTable.HasValue("LinkDependencies"))
						{
							auto linkDependencies = dependencyBuildTable
								.GetValue("LinkDependencies")
								.AsList()
								.CopyAsStringVector();

							arguments.LinkDependencies = CombineUnique(
								arguments.LinkDependencies,
								dependencyBuildTable.GetValue("LinkDependencies").AsList().CopyAsPathVector());
						}
					}
				}
			}
		}

		static std::vector<Path> CombineUnique(
			const std::vector<Path>& collection1,
			const std::vector<Path>& collection2)
		{
			std::unordered_set<std::string> valueSet;
			for (auto& value : collection1)
				valueSet.insert(value.ToString());
			for (auto& value : collection2)
				valueSet.insert(value.ToString());

			std::vector<Path> result;
			for (auto& value : valueSet)
				result.push_back(Path(value));

			return result;
		}

		static std::vector<Path> MakeUnique(const std::vector<Path>& collection)
		{
			std::unordered_set<std::string> valueSet;
			for (auto& value : collection)
				valueSet.insert(value.ToString());

			std::vector<Path> result;
			for (auto& value : valueSet)
				result.push_back(Path(value));

			return result;
		}

	private:
		CompilerFactory _compilerFactory;
		static Soup::Build::Utilities::StringList _runBeforeList;
		static Soup::Build::Utilities::StringList _runAfterList;
	};

	Soup::Build::Utilities::StringList TestBuildTask::_runBeforeList =
		Soup::Build::Utilities::StringList();
	Soup::Build::Utilities::StringList TestBuildTask::_runAfterList =
		Soup::Build::Utilities::StringList({
			"Build",
		});
}
