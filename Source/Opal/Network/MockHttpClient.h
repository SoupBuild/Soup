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
			_port(port)
		{
		}

		/// <summary>
		/// Perform an Http Get request
		/// </summary>
		HttpResponse Get(std::string_view requests) override final
		{
			return HttpResponse();
		}

	private:
		std::string _host;
		int _port;
	};
}
