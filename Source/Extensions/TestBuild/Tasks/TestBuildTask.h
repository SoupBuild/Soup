#pragma once

namespace Soup::Test
{
	/// <summary>
	/// The simple build task that will run after the main build task
	/// </summary>
	class TestBuildTask : public Memory::ReferenceCounted<Soup::Build::IBuildTask>
	{
	public:
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
		const Soup::Build::IList<const char*>& GetRunBeforeList() const noexcept override final
		{
			return _runBeforeList;
		}

		/// <summary>
		/// Get the run after list
		/// </summary>
		const Soup::Build::IList<const char*>& GetRunAfterList() const noexcept override final
		{
			return _runAfterList;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		Soup::Build::ApiCallResult TryExecute(
			Soup::Build::IBuildState& buildState) noexcept override final
		{
			auto buildStateWrapper = Soup::Build::Extensions::BuildStateWrapper(buildState);

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
		void Execute(Soup::Build::Extensions::BuildStateWrapper& buildState)
		{
			buildState.LogHighPriority("Building Tests");

			auto rootTable = buildState.GetActiveState();
			auto recipeTable = rootTable.GetValue("Recipe").AsTable();

			if (!recipeTable.HasValue("Tests"))
			{
				buildState.LogError("No Tests Specified");
				throw std::runtime_error("");
			}

			auto testTable = recipeTable.GetValue("Tests").AsTable();
			if (!testTable.HasValue("Source"))
			{
				buildState.LogError("No Test Source Files");
				throw std::runtime_error("");
			}

			auto arguments = Soup::Compiler::BuildArguments();
			arguments.SourceFiles =
				testTable.GetValue("Source").AsList().CopyAsPathVector();

			arguments.TargetName = "TestHarness";
			arguments.TargetType = Soup::Compiler::BuildTargetType::Executable;

			// Load up the common properties used from the generic build
			auto buildTable = rootTable.GetValue("Build").AsTable();
			LoadBuildProperties(buildTable, arguments);
		}

		void LoadBuildProperties(
			Soup::Build::Extensions::ValueTableWrapper& buildTable,
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
		static Soup::Build::Extensions::StringList _runBeforeList;
		static Soup::Build::Extensions::StringList _runAfterList;
	};

	Soup::Build::Extensions::StringList TestBuildTask::_runBeforeList =
		Soup::Build::Extensions::StringList();
	Soup::Build::Extensions::StringList TestBuildTask::_runAfterList =
		Soup::Build::Extensions::StringList({
			"Build",
		});
}
