// <copyright file="IBuildEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildArguments.h"

namespace Soup
{
	/// <summary>
	/// The build engine interface
	/// Mainly used to allow for unit testing client code
	/// </summary>
	class IBuildEngine
	{
	public:
		/// <summary>
		/// The Core Execute task
		/// </summary>
		virtual bool Execute(const BuildArguments& arguments) = 0;
	};
}
