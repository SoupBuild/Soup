// <copyright company="Soup" file="ApiException.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using System;
	using System.Net;

	/// <summary>
	/// Api Exception
	/// </summary>
	public class ApiException : Exception
	{
		public ApiException(HttpStatusCode statusCode)
		{
			StatusCode = statusCode;
		}
		
		public HttpStatusCode StatusCode { get; private set; }
	}
}
