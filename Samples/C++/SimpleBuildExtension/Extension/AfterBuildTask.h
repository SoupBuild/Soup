#pragma once

namespace SimpleBuildExtension
{
	/// <summary>
	/// The simple build task that will run after the main build task
	/// </summary>
	class AfterBuildTask : public Opal::Memory::ReferenceCounted<Soup::Build::IBuildTask>
	{
	public:
		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() const noexcept override final
		{
			return "AfterBuild";
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
				buildStateWrapper.LogError("Unknown Error!");
				return Soup::Build::ApiCallResult::Error;
			}
		}

	private:
		/// <summary>
		/// The Core Execute task
		/// </summary>
		void Execute(Soup::Build::Utilities::BuildStateWrapper& buildState)
		{
			buildState.LogHighPriority("Running After Build!");
		}

	private:
		static Soup::Build::Utilities::StringList _runBeforeList;
		static Soup::Build::Utilities::StringList _runAfterList;
	};

	Soup::Build::Utilities::StringList AfterBuildTask::_runBeforeList =
		Soup::Build::Utilities::StringList();
	Soup::Build::Utilities::StringList AfterBuildTask::_runAfterList =
		Soup::Build::Utilities::StringList({
			"Build",
		});
}
