// <copyright file="ApiException.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Api
{
    /// <summary>
    /// Api Exception
    /// </summary>
    class ApiException : public std::runtime_error
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="ApiException"/> class.
        /// </summary>
        ApiException(Network::HttpStatusCode statusCode) :
            std::runtime_error("Api Exception"),
            _statusCode(statusCode)
        {
        }

        Network::HttpStatusCode GetStatusCode() const
        {
            return _statusCode;
        }

    private:
        Network::HttpStatusCode _statusCode;
    };
}
