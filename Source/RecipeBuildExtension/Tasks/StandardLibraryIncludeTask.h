// <copyright file="StandardLibraryIncludeTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace RecipeBuild
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	export class StandardLibraryIncludeTask : public Memory::ReferenceCounted<Soup::Build::IBuildTask>
	{
	public:
		/// <summary>
		/// Get the task name
		/// </summary>
		const char* GetName() const noexcept override final
		{
			return "StandardLibraryInclude";
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
		Soup::Build::OperationResult Execute(
			Soup::Build::IBuildState& buildState) noexcept override final
		{
			try
			{
				auto state = Soup::Build::Extensions::BuildStateWrapper(buildState);

				return 0;
			}
			catch(...)
			{
				// Unknown error
				return -1;
			}
		}

	private:
		Soup::Build::Runtime::StringList _runBeforeList;
		Soup::Build::Runtime::StringList _runAfterList;
	};
}
