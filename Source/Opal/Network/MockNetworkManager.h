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
		/// Get the requests
		/// </summary>
		const std::vector<std::string>& GetRequests() const
		{
			return _requests;
		}

		/// <summary>
		/// Register a mock Http client
		/// </summary>
		void RegisterClient(const std::shared_ptr<MockHttpClient>& client)
		{
			// Check if the requested client exists
			auto hostPort = std::stringstream();
			hostPort << client->GetHost() << ":" << std::to_string(client->GetPort());

			auto insertResult = _clients.try_emplace(hostPort.str(), client);
			if (!insertResult.second)
			{
				throw std::runtime_error("The mock client already exists.");
			}
		}

		/// <summary>
		/// Create an Http Client
		/// </summary>
		std::shared_ptr<IHttpClient> CreateClient(
			std::string_view host,
			int port) override final
		{
			auto message = std::stringstream();
			message << "CreateClient: " << host << ":" << std::to_string(port);
			_requests.push_back(message.str());

			// Check if the requested client exists
			auto hostPort = std::stringstream();
			hostPort << host << ":" << std::to_string(port);

			auto searchClient = _clients.find(hostPort.str());
			if (searchClient != _clients.end())
			{
				return searchClient->second;
			}
			else
			{
				throw std::runtime_error("No mock client registered.");
			}
		}

	private:
		std::vector<std::string> _requests;
		std::map<std::string, std::shared_ptr<Network::MockHttpClient>> _clients;
	};
}
