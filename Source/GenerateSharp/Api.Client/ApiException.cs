// <copyright file="ApiException.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Net;

namespace Soup.Build.Api.Client;

[SuppressMessage("Design", "CA1032:Implement standard exception constructors", Justification = "Required parameters")]
public class ApiException : Exception
{
	public HttpStatusCode StatusCode { get; private set; }

	public IReadOnlyDictionary<string, IEnumerable<string>> Headers { get; private set; }

	public ApiException(
		string message,
		HttpStatusCode statusCode,
		IReadOnlyDictionary<string, IEnumerable<string>> headers,
		Exception? innerException)
		: base($"{message} - StatusCode {statusCode}", innerException)
	{
		StatusCode = statusCode;
		Headers = headers;
	}
}

[SuppressMessage("Design", "CA1032:Implement standard exception constructors", Justification = "Required parameters")]
public class ApiException<TResult> : ApiException
{
	public TResult Result { get; private set; }

	public ApiException(
		string message,
		HttpStatusCode statusCode,
		IReadOnlyDictionary<string, IEnumerable<string>> headers,
		TResult result,
		Exception? innerException)
		: base(message, statusCode, headers, innerException)
	{
		Result = result;
	}
}
