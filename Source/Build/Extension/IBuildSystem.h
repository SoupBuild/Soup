// <copyright file="IBuildSystem.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// Build System Extension interface
	/// </summary>
	export class IBuildSystem
	{
	public:
		virtual void RegisterTask(std::shared_ptr<IBuildTask> task) = 0;
	};
}
