// <copyright file="ScopedProcessManagerRegister.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IProcessManager.h"

namespace Opal::System
{
	/// <summary>
	/// A scopped process manager registration helper
	/// </summary>
	export class ScopedProcessManagerRegister
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedProcessManagerRegister'/> class.
		/// </summary>
		ScopedProcessManagerRegister(std::shared_ptr<IProcessManager> listener)
		{
			IProcessManager::Register(std::move(listener));
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedProcessManagerRegister'/> class.
		/// </summary>
		~ScopedProcessManagerRegister()
		{
			IProcessManager::Register(nullptr);
		}
	};
}
