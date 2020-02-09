// <copyright file="INetworkManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IHttpClient.h"

namespace Opal::Network
{
	/// <summary>
	/// The network manager interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	export class INetworkManager
	{
	public:
		/// <summary>
		/// Gets the current active manager
		/// </summary>
		static INetworkManager& Current()
		{
			if (_current == nullptr)
				throw std::runtime_error("No network manager implementation registered.");
			return *_current;
		}

		/// <summary>
		/// Register a new active network manager
		/// </summary>
		static void Register(std::shared_ptr<INetworkManager> manager)
		{
			_current = std::move(manager);
		}

	public:
		/// <summary>
		/// Create an Http Client
		/// </summary>
		virtual std::shared_ptr<IHttpClient> CreateClient(
			std::string host,
			int port) = 0;

	private:
		static std::shared_ptr<INetworkManager> _current;
	};

	std::shared_ptr<INetworkManager> INetworkManager::_current = nullptr;
}
