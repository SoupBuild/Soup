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

		public string Response { get; private set; }

		public IReadOnlyDictionary<string, IEnumerable<string>> Headers { get; private set; }

		public ApiException(
			string message,
			int statusCode,
			string response,
			IReadOnlyDictionary<string, IEnumerable<string>> headers,
			Exception innerException)
			: base(message + "\n\nStatus: " + statusCode + "\nResponse: \n" + ((response == null) ? "(null)" : response.Substring(0, response.Length >= 512 ? 512 : response.Length)), innerException)
		{
			StatusCode = statusCode;
			Response = response;
			Headers = headers;
		}

		public override string ToString()
		{
			return string.Format("HTTP Response: \n\n{0}\n\n{1}", Response, base.ToString());
		}
	}

	public class ApiException<TResult> : ApiException
	{
		public TResult Result { get; private set; }

		public ApiException(
			string message,
			int statusCode,
			string response,
			IReadOnlyDictionary<string, IEnumerable<string>> headers,
			TResult result,
			Exception innerException)
			: base(message, statusCode, response, headers, innerException)
		{
			Result = result;
		}
	}
}
