// <copyright file="ScopedDetourProcessManagerRegister.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IDetourProcessManager.h"

namespace Monitor
{
	/// <summary>
	/// A scopped process manager registration helper
	/// </summary>
	export class ScopedDetourProcessManagerRegister
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedDetourProcessManagerRegister'/> class.
		/// </summary>
		ScopedDetourProcessManagerRegister(std::shared_ptr<IDetourProcessManager> listener)
		{
			IDetourProcessManager::Register(std::move(listener));
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedDetourProcessManagerRegister'/> class.
		/// </summary>
		~ScopedDetourProcessManagerRegister()
		{
			IDetourProcessManager::Register(nullptr);
		}
	};
}
