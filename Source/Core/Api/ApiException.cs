// <copyright file="ApiException.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
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
        /// <summary>
        /// Initializes a new instance of the <see cref="ApiException"/> class.
        /// </summary>
        public ApiException(HttpStatusCode statusCode)
        {
            StatusCode = statusCode;
        }

        /// <summary>
        /// Gets the Status Code
        /// </summary>
        public HttpStatusCode StatusCode { get; private set; }
    }
}
