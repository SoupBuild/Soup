// <copyright file="HttpStatusCode.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Opal::Network
{
	/// <summary>
	/// The Http Client interface
	/// Interface mainly used to allow for unit testing client code
	/// </summary>
	export enum class HttpStatusCode
	{
		// The client can continue with its request.
		Continue = 100,

		// The protocol version or protocol type is changing.
		SwitchingProtocols = 101,

		// The request succeeded.
		Ok = 200,

		// The request resulted in a new resource created before the response was sent.
		Created = 201,

		// The request has been accepted for further processing.
		Accepted = 202,

		// The result is from a cached copy instead of the origin server.
		NonAuthoritativeInformation = 203,

		// The request has been successfully processed and the response is intentionally blank.
		NoContent = 204,

		// The client should reset the current resource.
		ResetContent = 205,

		// The response is a partial response as requested by a GET request that includes a byte range.
		PartialContent = 206,

		// The requested information has multiple representations.
		Ambiguous = 300,

		// The requested information has been moved to the URI specified in the Location header.
		// Synonym for MovedPermanently.
		Moved = 301,

		// The requested information is located at the URI specified in the Location header.
		// Synonym for Redirect.
		Found = 302,

		// Automatically redirects the client to the URI specified in the Location header as the result of a POST.
		// The request to the resource specified by the Location header will be made with a GET. 
		// Synonym for SeeOther.
		RedirectMethod = 303,

		// The client's cached copy is up to date. The contents of the resource are not transferred.
		NotModified = 304,

		// The request should use the proxy server at the URI specified in the Location header.
		UseProxy = 305,

		// Not fully specified.
		Unused = 306,

		// The request information is located at the URI specified in the Location header.
		// Synonym for TemporaryRedirect.
		RedirectKeepVerb = 307,

		// The request could not be understood by the server.
		// No other error is applicable, or the exact error is unknown or does not have its own error code.
		BadRequest = 400,

		// The requested resource requires authentication.
		Unauthorized = 401,

		// Reserved for future use.
		PaymentRequired = 402,

		// The server refuses to fulfill the request.
		Forbidden = 403,

		// The requested resource does not exist on the server.
		NotFound = 404,

		// The request method (POST or GET) is not allowed on the requested resource.
		MethodNotAllowed = 405,

		// The client has indicated with Accept headers that it will not accept any of the available representations of the resource.
		NotAcceptable = 406,

		// The requested proxy requires authentication.
		// The Proxy-authenticate header contains the details of how to perform the authentication.
		ProxyAuthenticationRequired = 407,

		// The client did not send a request within the time the server was expecting the request.
		RequestTimeout = 408,

		// The request could not be carried out because of a conflict on the server.
		Conflict = 409,

		// The requested resource is no longer available.
		Gone = 410,

		// The required Content-length header is missing.
		LengthRequired = 411,

		// A condition set for this request failed, and the request cannot be carried out.
		PreconditionFailed = 412,

		// The request is too large for the server to process.
		RequestEntityTooLarge = 413,

		// The URI is too long.
		RequestUriTooLong = 414,

		// The request is an unsupported type.
		UnsupportedMediaType = 415,

		// The range of data requested from the resource cannot be returned,
		// either because the beginning of the range is before the beginning of the resource,
		// or the end of the range is after the end of the resource.
		RequestedRangeNotSatisfiable = 416,

		// An expectation given in an Expect header could not be met by the server.
		ExpectationFailed = 417,

		// The client should switch to a different protocol such as TLS/1.0.
		UpgradeRequired = 426,

		// A generic error has occurred on the server.
		InternalServerError = 500,

		// The server does not support the requested function.
		NotImplemented = 501,

		// An intermediate proxy server received a bad response from another proxy or the origin server.
		BadGateway = 502,

		// The server is temporarily unavailable, usually due to high load or maintenance.
		ServiceUnavailable = 503,

		// An intermediate proxy server timed out while waiting for a response from another proxy or the origin server.
		GatewayTimeout = 504,

		// The requested HTTP version is not supported by the server.
		HttpVersionNotSupported = 505,
	};
}
