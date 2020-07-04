// <copyright file="ApiCallResult.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// Build system call result type that is used as an intermediate for internal exceptions
	/// when passing over the strict DLL boundary. The general expected behavior is to return zero
	/// to indicate success, positive numbers to indicate warning state that can be continued (unless strict builds is enabled),
	/// and negative to indicate a non-recoverable build failure.
	/// </summary>
	export enum class ApiCallResult : int64_t
	{
		Success = 0,
		Warning = 1,
		Error = -1,

		// Specific error types that make logging better
		WrongType = -2,
		OutOfMemory = -3,
	};
}
