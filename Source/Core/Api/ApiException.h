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
        ApiException(const std::string& function, Network::HttpStatusCode statusCode) :
            std::runtime_error("ApiFailed[" + function + "] " + std::to_string((int)statusCode)),
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
