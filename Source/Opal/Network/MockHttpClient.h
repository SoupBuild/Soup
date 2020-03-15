// <copyright file="MockHttpClient.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IHttpClient.h"

namespace Opal::Network
{
	/// <summary>
	/// A mock Http Client
	/// </summary>
	export class MockHttpClient : public IHttpClient
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='MockHttpClient'/> class.
		/// </summary>
		MockHttpClient(std::string host, int port) :
			_host(std::move(host)),
			_port(port),
			_requests(),
			_getResponses(),
			_putResponses()
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
		/// Get the host
		/// </summary>
		const std::string& GetHost() const
		{
			return _host;
		}

		/// <summary>
		/// Get the port
		/// </summary>
		int GetPort() const
		{
			return _port;
		}

		/// <summary>
		/// Set the response for given request
		/// </summary>
		void SetResponse(std::string url, std::string response)
		{
			auto insertResult = _getResponses.try_emplace(std::move(url), std::move(response));
			if (!insertResult.second)
			{
				throw std::runtime_error("The mock client already has the requested url set.");
			}
		}

		/// <summary>
		/// Perform an Http Get request
		/// </summary>
		HttpResponse Get(std::string_view request) override final
		{
			auto message = std::stringstream();
			message << "Get: " << request;
			_requests.push_back(message.str());

			auto searchClient = _getResponses.find(std::string(request));
			if (searchClient != _getResponses.end())
			{
				return HttpResponse(HttpStatusCode::Ok, searchClient->second);
			}
			else
			{
				// The response was not set, return not found
				return HttpResponse(HttpStatusCode::NotFound);
			}
		}

		/// <summary>
		/// Perform an Http Put request
		/// </summary>
		HttpResponse Put(
			std::string_view request,
			std::string_view contentType,
			std::istream& content) override final
		{
			auto message = std::stringstream();
			message << "Put: " << request;
			message << " [" << contentType << "]";
			_requests.push_back(message.str());

			auto searchClient = _putResponses.find(std::string(request));
			if (searchClient != _putResponses.end())
			{
				return HttpResponse(HttpStatusCode::Ok, searchClient->second);
			}
			else
			{
				// The response was not set, return not found
				return HttpResponse(HttpStatusCode::NotFound);
			}
		}

	private:
		std::string _host;
		int _port;
		std::vector<std::string> _requests;
		std::map<std::string, std::string> _getResponses;
		std::map<std::string, std::string> _putResponses;
	};
}
