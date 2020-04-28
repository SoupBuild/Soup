// <copyright file="ScopedNetworkManagerRegister.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "INetworkManager.h"

namespace Opal::Network
{
	/// <summary>
	/// A scopped network manager registration helper
	/// </summary>
	export class ScopedNetworkManagerRegister
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='ScopedNetworkManagerRegister'/> class.
		/// </summary>
		ScopedNetworkManagerRegister(std::shared_ptr<INetworkManager> manager)
		{
			INetworkManager::Register(std::move(manager));
		}

		/// <summary>
		/// Finalizes an instance of the <see cref='ScopedNetworkManagerRegister'/> class.
		/// </summary>
		~ScopedNetworkManagerRegister()
		{
			INetworkManager::Register(nullptr);
		}
	};
}
