// <copyright file="HttpLibClient.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IHttpClient.h"

namespace Opal::Network
{
	/// <summary>
	/// The Http Lib Client
	/// </summary>
	export class HttpLibClient : public IHttpClient
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref='HttpLibClient'/> class.
		/// </summary>
		HttpLibClient(std::string host, int port) :
			_client(nullptr)
		{
			if (port == 443)
				_client = std::make_unique<httplib::SSLClient>(host, port);
			else
				_client = std::make_unique<httplib::Client>(host, port);

			// Enable proxy for fiddler
			#ifdef ENABLE_FIDDLER_PROXY
			_client->set_proxy("127.0.0.1", 8888);
			#endif
		}

		/// <summary>
		/// Set the authentication token
		/// </summary>
		void SetAuthenticationToken(std::string_view scheme, std::string_view token) override final
		{
			_client->set_auth_token(scheme.data(), token.data());
		}

		/// <summary>
		/// Perform an Http Get request
		/// </summary>
		HttpResponse Get(std::string_view request) override final
		{
			auto response = _client->Get(request.data());
			if (response == nullptr)
				throw std::runtime_error("HttpLibClient: Get failed.");

			auto statusCode = static_cast<HttpStatusCode>(response->status);
			return HttpResponse(statusCode, std::move(response->body));
		}

		/// <summary>
		/// Perform an Http Post request
		/// </summary>
		HttpResponse Post(
			std::string_view request,
			std::string_view contentType,
			std::istream& content) override final
		{
			auto body = std::string(std::istreambuf_iterator<char>(content), {});
			auto response = _client->Post(request.data(), body, contentType.data());
			if (response == nullptr)
				throw std::runtime_error("HttpLibClient: Post failed");

			auto statusCode = static_cast<HttpStatusCode>(response->status);
			return HttpResponse(statusCode, std::move(response->body));
		}

		/// <summary>
		/// Perform an Http Put request
		/// </summary>
		HttpResponse Put(
			std::string_view request,
			std::string_view contentType,
			std::istream& content) override final
		{
			auto body = std::string(std::istreambuf_iterator<char>(content), {});
			auto response = _client->Put(request.data(), body, contentType.data());
			if (response == nullptr)
				throw std::runtime_error("HttpLibClient: Put failed.");

			auto statusCode = static_cast<HttpStatusCode>(response->status);
			return HttpResponse(statusCode, std::move(response->body));
		}

	private:
		std::unique_ptr<httplib::Client> _client;
	};
}
