// <copyright file="ClientCredentialsTokenModel.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Api
{
    /// <summary>
    /// A class representing the Client Credentials Token
    /// </summary>
    export class ClientCredentialsTokenModel
    {
    public:
        ClientCredentialsTokenModel()
        {
        }

        ClientCredentialsTokenModel(
            std::string accessToken,
            int expiresIn,
            std::string tokenType,
            std::string scope) :
            AccessToken(std::move(accessToken)),
            ExpiresIn(expiresIn),
            TokenType(std::move(tokenType)),
            Scope(std::move(scope))
        {
        }

        std::string AccessToken;
        int ExpiresIn;
        std::string TokenType;
        std::string Scope;
    };
}
