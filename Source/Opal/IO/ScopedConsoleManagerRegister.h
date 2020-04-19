// <copyright file="ScopedConsoleManagerRegister.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IConsoleManager.h"

namespace Opal::IO
{
	/// <summary>
	/// A scopped network manager registration helper
	/// </summary>
	export class ScopedConsoleManagerRegister
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedConsoleManagerRegister'/> class.
		/// </summary>
		ScopedConsoleManagerRegister(std::shared_ptr<IConsoleManager> manager)
		{
			IConsoleManager::Register(std::move(manager));
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedConsoleManagerRegister'/> class.
		/// </summary>
		~ScopedConsoleManagerRegister()
		{
			IConsoleManager::Register(nullptr);
		}
	};
}
