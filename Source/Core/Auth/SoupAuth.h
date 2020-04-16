// <copyright file="SoupAuth.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "SoupAuthJsonModels.h"
#include "../Api/ApiException.h"

namespace Soup::Api
{
    /// <summary>
    /// Represents a collection of functions to interact with the Auth endpoints
    /// </summary>
    class SoupAuth
    {
    private:
        static const std::string_view ServiceEndpoint;
        static const int ServicePort;

    public:
        /// <summary>
        /// Get the OpenId configuration
        /// </summary>
        static OpenIdConfigurationModel GetOpenIdConfiguration()
        {
            auto client = Network::INetworkManager::Current().CreateClient(
                ServiceEndpoint,
                ServicePort);

            auto urlBuilder = std::stringstream();
            urlBuilder << "/.well-known/openid-configuration";
            auto url = urlBuilder.str();

            Log::Diag(url);
            auto response = client->Get(url);

            // Verify that we got a success
            if (response.StatusCode != Network::HttpStatusCode::Ok)
            {
                throw ApiException("GetOpenIdConfiguration", response.StatusCode);
            }

            // Parse the return result
            auto result = SoupAuthJsonModels::ParseOpenIdConfigurationResult(response.Body);
            return result;
        }

        /// <summary>
        /// Get the OpenId configuration
        /// </summary>
        static ClientCredentialsTokenModel RequestClientCredentialsToken(
            const std::string& tokenEndpoint,
            const std::string& userName,
            const std::string& password)
        {
            // TODO: Implement an opal url class
            // https://stackoverflow.com/questions/5620235/cpp-regular-expression-to-validate-url/31613265
            auto urlParserRegex = std::regex(
                R"(^(([^:\/?#]+):)?(//([^\/?#]*))?([^?#]*)(\?([^#]*))?(#(.*))?)",
                std::regex::extended);
            std::smatch urlParts;
            if (!std::regex_match(tokenEndpoint, urlParts, urlParserRegex))
            {
                throw std::runtime_error("Invalid token endpoint url.");
            }

            auto scheme = urlParts[2].str();
            auto authority = urlParts[4].str();
            auto path = urlParts[5].str();
            auto query = urlParts[7].str();
            auto fragment = urlParts[9].str();

            auto endpoint = authority;
            auto port = 80;
            auto portDelimiter = authority.find(':');
            if (portDelimiter != std::string::npos)
            {
                endpoint = authority.substr(0, portDelimiter);
                port = std::stoi(authority.substr(portDelimiter + 1));
            }

            auto client = Network::INetworkManager::Current().CreateClient(
                endpoint,
                port);

            auto urlBuilder = std::stringstream();
            urlBuilder << path << query << fragment;
            auto url = urlBuilder.str();

            auto secret = "secret_for_client";

            auto content = std::stringstream();
            content << "grant_type=password";
            content << "&username=" << userName;
            content << "&password=" << password;
            content << "&scope=soup_api";
            content << "&client_id=soup.client";
            content << "&client_secret=" << secret;

            auto contentType = "application/x-www-form-urlencoded";

            Log::Diag(url);
            auto response = client->Post(url, contentType, content);

            // Verify that we got a success
            if (response.StatusCode != Network::HttpStatusCode::Ok)
            {
                throw ApiException("RequestClientCredentialsToken", response.StatusCode);
            }

            // Parse the return result
            auto result = SoupAuthJsonModels::ParseClientCredentialsTokenModel(response.Body);
            return result;
        }
    };

// #define LOCAL_DEBUG
#ifdef LOCAL_DEBUG
    /*static*/ const std::string_view SoupAuth::ServiceEndpoint = "localhost";
    /*static*/ const int SoupAuth::ServicePort = 5051;
#else
    /*static*/ const std::string_view SoupAuth::ServiceEndpoint = "auth.soupbuild.com";
    /*static*/ const int SoupAuth::ServicePort = 443;
#endif
}
