// <copyright file="StandardLibraryIncludeTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// The recipe build task that knows how to build a single recipe
	/// </summary>
	export class StandardLibraryIncludeTask : public Memory::ReferenceCounted<IBuildTask>
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
		/// The Core Execute task
		/// </summary>
		BuildSystemResult Execute(IBuildState& buildState) noexcept override final
		{
			try
			{
				auto state = BuildStateWrapper(buildState);

				return 0;
			}
			catch(...)
			{
				// Unknown error
				return -1;
			}
		}
	};
}
