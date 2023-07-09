// <copyright file="ApiException.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Soup.Build.Api.Client
{
	public class ApiException : Exception
	{
		public int StatusCode { get; private set; }

		public IReadOnlyDictionary<string, IEnumerable<string>> Headers { get; private set; }

		public ApiException(
			string message,
			int statusCode,
			IReadOnlyDictionary<string, IEnumerable<string>> headers,
			Exception? innerException)
			: base(message, innerException)
		{
			StatusCode = statusCode;
			Headers = headers;
		}
	}

	public class ApiException<TResult> : ApiException
	{
		public TResult Result { get; private set; }

		public ApiException(
			string message,
			int statusCode,
			IReadOnlyDictionary<string, IEnumerable<string>> headers,
			TResult result,
			Exception? innerException)
			: base(message, statusCode, headers, innerException)
		{
			Result = result;
		}
	}
}
