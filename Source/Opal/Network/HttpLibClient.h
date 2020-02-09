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
			_client(std::move(host), port)
		{
		}

		/// <summary>
		/// Perform an Http Get request
		/// </summary>
		HttpResponse Get(std::string_view request) override final
		{
			auto response = _client.Get(request.data());
			if (response == nullptr)
				throw std::runtime_error("HttpLibClient: Get failed.");

			auto statusCode = static_cast<HttpStatusCode>(response->status);
			return HttpResponse(statusCode, std::move(response->body));
		}

	private:
		httplib::Client _client;
	};
}
