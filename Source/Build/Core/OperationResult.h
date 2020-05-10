// <copyright file="OperationResult.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// Build system call result type that is used as an intermediate for internal exceptions
	/// when passing over the strict DLL boundary.
	/// </summary>
	export using OperationResult = int64_t;
}
