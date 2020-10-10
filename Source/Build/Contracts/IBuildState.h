// <copyright file="IBuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ApiCallResult.h"
#include "IValueTable.h"

namespace Soup::Build
{
	export enum class TraceLevel : uint64_t
	{
		// Exceptional state that will fail the build
		Error = 1,

		// A possible issue in the build that may be fine to continue
		Warning = 2,

		// Highest level of logging that will be on in all but the quiet logs
		HighPriority = 3,

		// Important information that will be on in verbose logs. May help users investigate what occurred during a build.
		Information = 4,

		// The most detailed of logs that will only be useful for detailed investigations into runtime issues for build engineers. Diagnostic log level.
		Debug = 5,
	};

	/// <summary>
	/// Build State Extension interface that is the core object granting
	/// build extensions access to the shared build state.
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	export class IBuildState
	{
	public:
		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		virtual IValueTable& GetActiveState() noexcept = 0;

		/// <summary>
		/// Get a reference to the shared state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		virtual IValueTable& GetSharedState() noexcept = 0;

		/// <summary>
		/// Create a build operation
		/// </summary>
		virtual ApiCallResult TryCreateOperation(
			const char* title,
			const char* executable,
			const char* arguments,
			const char* workingDirectory,
			IReadOnlyList<const char*>& declaredInput,
			IReadOnlyList<const char*>& declaredOutput) noexcept = 0;

		/// <summary>
		/// Log a message to the build system
		/// </summary>
		virtual ApiCallResult TryLogTrace(TraceLevel level, const char* message) noexcept = 0;
	};
}
