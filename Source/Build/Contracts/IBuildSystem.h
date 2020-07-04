// <copyright file="IBuildSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ApiCallResult.h"

namespace Soup::Build
{
	/// <summary>
	/// The shared Build System Extension interface that is passed into a build extension 
	/// registration method. This container allows for a build extension to register any number 
	/// of build tasks that will be executed during the generate phase.
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	export class IBuildSystem
	{
	public:
		virtual ApiCallResult TryRegisterTask(IBuildTask* task) noexcept = 0;
	};
}
