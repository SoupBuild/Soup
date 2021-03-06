﻿// <copyright file="HttpResponse.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "HttpStatusCode.h"

namespace Opal::Network
{
	/// <summary>
	/// The Http Client interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	export struct HttpResponse
	{
	public:
		HttpResponse(HttpStatusCode statusCode) :
			StatusCode(statusCode),
			Body()
		{
		}

		HttpResponse(HttpStatusCode statusCode, std::string body) :
			StatusCode(statusCode),
			Body(std::move(body))
		{
		}

		/// <summary>
		/// The Status Code
		/// </summary>
		HttpStatusCode StatusCode;

		/// <summary>
		/// The response body
		/// </summary>
		std::string Body;
	};
}
