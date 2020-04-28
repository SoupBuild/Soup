// <copyright file="HttpLibNetworkManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "INetworkManager.h"
#include "HttpLibClient.h"

namespace Opal::Network
{
	/// <summary>
	/// The Http Lib network manager
	/// </summary>
	export class HttpLibNetworkManager : public INetworkManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='HttpLibNetworkManager'/> class.
		/// </summary>
		HttpLibNetworkManager()
		{
		}

		/// <summary>
		/// Create an Http Client
		/// </summary>
		std::shared_ptr<IHttpClient> CreateClient(
			std::string_view host,
			int port) override final
		{
			return std::make_shared<HttpLibClient>(std::string(host), port);
		}
	};
}
