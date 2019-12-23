// <copyright file="STLEnvironment.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IEnvironment.h"

namespace Opal::System
{
	/// <summary>
	/// The standard library file system
	/// </summary>
	export class STLEnvironment : public IEnvironment
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='STLEnvironment'/> class.
		/// </summary>
		STLEnvironment()
		{
		}

		/// <summary>
		/// Gets the value of an environment variable
		/// </summary>
		std::string GetEnvironmentVariable(std::string_view name) override final
		{
			auto value = std::getenv(name.data());
			return value;
		}
	};
}
