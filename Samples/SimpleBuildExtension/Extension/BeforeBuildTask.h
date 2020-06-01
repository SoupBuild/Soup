#pragma once

namespace SimpleBuildExtension
{
	/// <summary>
	/// The simple build task that will run before the main build task
	/// </summary>
	class BeforeBuildTask : public Opal::Memory::ReferenceCounted<Soup::Build::IBuildTask>
	{
	public:
		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() const noexcept override final
		{
			return "BeforeBuild";
		}

		/// <summary>
		/// Get the run before list
		/// </summary>
		Soup::Build::IList<const char*>& GetRunBeforeList() noexcept override final
		{
			return _runBeforeList;
		}

		/// <summary>
		/// Get the run after list
		/// </summary>
		Soup::Build::IList<const char*>& GetRunAfterList() noexcept override final
		{
			return _runAfterList;
		}

		/// <summary>
		/// The Core Execute task
		/// </summary>
		Soup::Build::ApiCallResult Execute(
			Soup::Build::IBuildState& buildState) noexcept override final
		{
			auto buildStateWrapper = Soup::Build::Extensions::BuildStateWrapper(buildState);

			try
			{
				// We cannot throw accross the DLL boundary for a build extension
				// So all internal errors must be converted to error codes
				Execute(buildStateWrapper);
				return 0;
			}
			catch(...)
			{
				buildStateWrapper.LogError("Unknown Error!");
				return -1;
			}
		}

	private:
		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(Soup::Build::Extensions::BuildStateWrapper& buildState)
		{
			buildState.LogHighPriority("Running Before Build!");
			auto rootTable = buildState.GetActiveState();
			auto buildTable = rootTable.EnsureValue("Build").EnsureTable();

			// As a sample of what a build extension can do we set a new
			// preprocessor definition to influence the build
			auto preprocessorDefinitions = std::vector<std::string>({
				"SPECIAL_BUILD",
			});
			buildTable.EnsureValue("PreprocessorDefinitions").EnsureList().Append(preprocessorDefinitions);
		}

	private:
		static Soup::Build::Extensions::StringList _runBeforeList;
		static Soup::Build::Extensions::StringList _runAfterList;
	};

	Soup::Build::Extensions::StringList BeforeBuildTask::_runBeforeList =
		Soup::Build::Extensions::StringList({
			"Build",
		});
	Soup::Build::Extensions::StringList BeforeBuildTask::_runAfterList =
		Soup::Build::Extensions::StringList();
}
