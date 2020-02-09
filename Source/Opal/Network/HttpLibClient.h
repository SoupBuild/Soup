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
			return HttpResponse(HttpStatusCode::NotFound);
		}

	private:
		httplib::Client _client;
	};
}
