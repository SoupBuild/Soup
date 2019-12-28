// <copyright file="IBuildTask.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IBuildState.h"

namespace Soup::Build
{
	/// <summary>
	/// Build Task Extension interface
	/// </summary>
	class IBuildTask
	{
	public:
		virtual const char* GetName() = 0;
		virtual void Execute(IBuildState& state) = 0;
	};
}
