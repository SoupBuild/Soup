// <copyright file="StandardLibraryIncludeTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp
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
			try
			{
				auto state = Soup::Build::Utilities::BuildStateWrapper(buildState);

				return Soup::Build::ApiCallResult::Success;
			}
			catch(...)
			{
				// Unknown error
				return Soup::Build::ApiCallResult::Error;;
			}
		}

	private:
		Soup::Build::Utilities::ReadOnlyStringList _runBeforeList;
		Soup::Build::Utilities::ReadOnlyStringList _runAfterList;
	};
}
