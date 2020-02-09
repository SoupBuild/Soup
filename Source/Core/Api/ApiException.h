// <copyright file="ApiException.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Api
{
    /// <summary>
    /// Api Exception
    /// </summary>
    class ApiException : public std::exception
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="ApiException"/> class.
        /// </summary>
        ApiException()
        {
        }
    };
}
