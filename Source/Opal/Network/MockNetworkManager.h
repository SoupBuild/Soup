// <copyright file="MockNetworkManager.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "INetworkManager.h"
#include "MockHttpClient.h"

namespace Opal::Network
{
	/// <summary>
	/// The mock network manager
	/// TODO: Move into test project
	/// </summary>
	export class MockNetworkManager : public INetworkManager
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockNetworkManager'/> class.
		/// </summary>
		MockNetworkManager() :
			_requests()
		{
		}

		/// <summary>
		/// Get the load requests
		/// </summary>
		const std::vector<std::string>& GetRequests() const
		{
			return _requests;
		}

		/// <summary>
		/// Create an Http Client
		/// </summary>
		std::shared_ptr<IHttpClient> CreateClient(
			std::string host,
			int port) override final
		{
			std::stringstream message;
			message << "CreateClient: " << host << " " << std::to_string(port);
			_requests.push_back(message.str());

			return std::make_shared<MockHttpClient>(std::move(host), port);
		}

	private:
		std::vector<std::string> _requests;
	};
}
