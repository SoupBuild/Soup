#pragma once

namespace Soup::Test
{
	using CreateCompiler = std::function<std::shared_ptr<Cpp::Compiler::ICompiler>(Build::Utilities::ValueTableWrapper&)>;
	using CompilerFactory = std::map<std::string, CreateCompiler>;

	/// <summary>
	/// The simple build task that will run after the main build task
	/// </summary>
	class TestBuildTask : public Memory::ReferenceCounted<Build::IBuildTask>
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
		const Build::IReadOnlyList<const char*>& GetRunBeforeList() const noexcept override final
		{
			return _runBeforeList;
		}

		/// <summary>
		/// Get the run after list
		/// </summary>
		const Build::IReadOnlyList<const char*>& GetRunAfterList() const noexcept override final
		{
			return _runAfterList;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		Build::ApiCallResult TryExecute(
			Build::IBuildState& buildState) noexcept override final
		{
			auto buildStateWrapper = Build::Utilities::BuildStateWrapper(buildState);

			try
			{
				// We cannot throw accross the DLL boundary for a build extension
				// So all internal errors must be converted to error codes
				Execute(buildStateWrapper);
				return Build::ApiCallResult::Success;
			}
			catch(const std::exception& ex)
			{
				buildStateWrapper.LogError(ex.what());
				return Build::ApiCallResult::Error;
			}
			catch(...)
			{
				buildStateWrapper.LogError("Unknown Error");
				return Build::ApiCallResult::Error;
			}
		}

	private:
		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(Build::Utilities::BuildStateWrapper& buildState)
		{
			auto activeState = buildState.GetActiveState();
			auto sharedState = buildState.GetSharedState();
			auto recipeTable = activeState.GetValue("Recipe").AsTable();

			if (!recipeTable.HasValue("Tests"))
			{
				buildState.LogError("No Tests Specified");
				throw std::runtime_error("");
			}

			auto arguments = Cpp::Compiler::BuildArguments();
			arguments.TargetArchitecture = activeState.GetValue("BuildArchitecture").AsString().GetValue();

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

			// Update to place the output in a sub folder
			arguments.ObjectDirectory = arguments.ObjectDirectory + Path("Test/");
			arguments.BinaryDirectory = arguments.BinaryDirectory + Path("Test/");

			// Initialize the compiler to use
			auto compilerName = std::string(activeState.GetValue("CompilerName").AsString().GetValue());
			auto findCompilerFactory = _compilerFactory.find(compilerName);
			if (findCompilerFactory == _compilerFactory.end())
			{
				buildState.LogError("Unknown compiler: " + compilerName);
				throw std::runtime_error("");
			}

			auto createCompiler = findCompilerFactory->second;
			auto compiler = createCompiler(activeState);

			auto buildEngine = Cpp::Compiler::BuildEngine(compiler);
			auto buildResult = buildEngine.Execute(buildState, arguments);

			// Create the operation to run tests during build
			auto title = std::string("Run Tests");
			auto program = arguments.BinaryDirectory +
				Path(arguments.TargetName);
			program.SetFileExtension("exe");
			if (!program.HasRoot())
			{
				program = arguments.WorkingDirectory + program;
			}

			auto workingDirectory = arguments.WorkingDirectory;
			auto runArguments = std::string("");
			auto inputFiles = std::vector<Path>({
				program,
			});
			auto outputFiles = std::vector<Path>({});
			auto runTestsOperation =
				Build::Utilities::BuildOperation(
					std::move(title),
					std::move(workingDirectory),
					std::move(program),
					std::move(runArguments),
					std::move(inputFiles),
					std::move(outputFiles));

			// Run the test harness
			buildResult.BuildOperations.push_back(std::move(runTestsOperation));

			// Register the build operations
			for (auto& operation : buildResult.BuildOperations)
			{
				buildState.CreateOperation(operation);
			}
		}

		void LoadBuildProperties(
			Build::Utilities::ValueTableWrapper& buildTable,
			Cpp::Compiler::BuildArguments& arguments)
		{
			arguments.LanguageStandard = static_cast<Cpp::Compiler::LanguageStandard>(
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
				arguments.OptimizationLevel = static_cast<Cpp::Compiler::BuildOptimizationLevel>(
					buildTable.GetValue("OptimizationLevel").AsInteger().GetValue());
			}
			else
			{
				arguments.OptimizationLevel = Cpp::Compiler::BuildOptimizationLevel::None;
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

			// Load the internal link dependencies
			if (buildTable.HasValue("InternalLinkDependencies"))
			{
				arguments.LinkDependencies = MakeUnique(
					buildTable.GetValue("InternalLinkDependencies").AsList().CopyAsPathVector());
			}
		}

		void LoadTestBuildProperties(
			Build::Utilities::BuildStateWrapper& buildState,
			Build::Utilities::ValueTableWrapper& testTable,
			Cpp::Compiler::BuildArguments& arguments)
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

			if (testTable.HasValue("PlatformLibraries"))
			{
				arguments.PlatformLinkDependencies = CombineUnique(
					arguments.PlatformLinkDependencies,
					testTable.GetValue("PlatformLibraries").AsList().CopyAsPathVector());
			}

			arguments.TargetName = "TestHarness";
			arguments.TargetType = Cpp::Compiler::BuildTargetType::Executable;
		}

		void LoadDependencyBuildInput(
			Build::Utilities::ValueTableWrapper& buildTable,
			Cpp::Compiler::BuildArguments& arguments)
		{
			// Load the runtime dependencies
			if (buildTable.HasValue("RuntimeDependencies"))
			{
				arguments.RuntimeDependencies = MakeUnique(
					buildTable.GetValue("RuntimeDependencies").AsList().CopyAsPathVector());
			}

			// Load the module references
			if (buildTable.HasValue("ModuleDependencies"))
			{
				arguments.ModuleDependencies = MakeUnique(
					buildTable.GetValue("ModuleDependencies").AsList().CopyAsPathVector());
			}
		}

		static void LoadDevDependencyBuildInput(
			Build::Utilities::BuildStateWrapper& buildState,
			Build::Utilities::ValueTableWrapper& activeState,
			Cpp::Compiler::BuildArguments& arguments)
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
		static Build::Utilities::ReadOnlyStringList _runBeforeList;
		static Build::Utilities::ReadOnlyStringList _runAfterList;
	};

	Build::Utilities::ReadOnlyStringList TestBuildTask::_runBeforeList =
		Build::Utilities::ReadOnlyStringList();
	Build::Utilities::ReadOnlyStringList TestBuildTask::_runAfterList =
		Build::Utilities::ReadOnlyStringList({
			"Build",
		});
}
