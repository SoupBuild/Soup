// <copyright file="SoupApi.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "SoupApiJsonModels.h"
#include "ApiException.h"
#include "../Auth/Models/ClientCredentialsTokenModel.h"

namespace Soup::Api
{
    export enum class PublishPackageResult
    {
        Success,
        PackageDoesNotExist,
        AlreadyExists,
    };

    /// <summary>
    /// Represents a collection of functions to interact with the API endpoints
    /// </summary>
    export class SoupApi
    {
    private:
        static const std::string_view ServiceEndpoint;
        static const int ServicePort;

    public:
        /// <summary>
        /// Download a package version as an archive
        /// </summary>
        static std::string DownloadPackage(std::string_view name, SemanticVersion version)
        {
            auto client = Network::INetworkManager::Current().CreateClient(
                ServiceEndpoint,
                ServicePort);

            auto urlBuilder = std::stringstream();
            urlBuilder << "/v1/packages/" << name << "/v" << version.ToString() << "/download";
            auto url = urlBuilder.str();

            Log::Diag(url);
            auto response = client->Get(url);

            // Verify that we got a success
            if (response.StatusCode != Network::HttpStatusCode::Ok)
            {
                throw ApiException("DownloadPackage", response.StatusCode);
            }

            return response.Body;
        }

        /// <summary>
        /// Get the metadata for a package identified by the unique name
        /// </summary>
        static PackageResultModel GetPackage(std::string_view name)
        {
            auto client = Network::INetworkManager::Current().CreateClient(
                ServiceEndpoint,
                ServicePort);

            auto urlBuilder = std::stringstream();
            urlBuilder << "/v1/packages/" << name;
            auto url = urlBuilder.str();

            Log::Diag(url);
            auto response = client->Get(url);

            // Verify that we got a success
            if (response.StatusCode != Network::HttpStatusCode::Ok)
            {
                throw ApiException("GetPackage", response.StatusCode);
            }

            // Parse the return result
            auto result = SoupApiJsonModels::ParsePackageResult(response.Body);
            return result;
        }

        /// <summary>
        /// Get a package publication metadata
        /// </summary>
        // Task<PublicationResultModel> GetPublicationAsync(string name, SemanticVersion version)
        // {
        //     using (HttpClient client = new HttpClient())
        //     {
        //         var url = $"{Constants.SoupRESTEndpointV1}/packages/{name}/{version}";
        //         Log.Verbose(url);
        //         var response = await client.GetAsync(url);

        //         // Verify that we got a success
        //         response.EnsureSuccessStatusCode();

        //         // Parse the return result
        //         var content = await response.Content.ReadAsStringAsync();
        //         var result = JsonConvert.DeserializeObject<PublicationResultModel>(content);

        //         return result;
        //     }
        // }

        /// <summary>
        /// Publish a new package version as an archive
        /// </summary>
        static PublishPackageResult PublishPackage(
            const ClientCredentialsTokenModel& token,
            std::string_view name,
            SemanticVersion version,
            std::istream& value)
        {
            auto client = Network::INetworkManager::Current().CreateClient(
                ServiceEndpoint,
                ServicePort);

            client->SetAuthenticationToken("Bearer", token.AccessToken);

            auto urlBuilder = std::stringstream();
            urlBuilder << "/v1/packages/" << name << "/v" << version.ToString();
            auto url = urlBuilder.str();

            auto contentType = "application/x-7z-compressed";

            Log::Diag(url);
            auto response = client->Put(url, contentType, value);

            // Verify that we got a success
            switch (response.StatusCode)
            {
                case Network::HttpStatusCode::Created:
                    return PublishPackageResult::Success;
                case Network::HttpStatusCode::NotFound:
                    return PublishPackageResult::PackageDoesNotExist;
                case Network::HttpStatusCode::Conflict:
                    return PublishPackageResult::AlreadyExists;
                default:
                    throw Api::ApiException("PublishPackage", response.StatusCode);
            }
        }

        /// <summary>
        /// Create a package
        /// </summary>
        static PackageResultModel CreatePackage(
            const ClientCredentialsTokenModel& token,
            std::string_view name,
            const PackageCreateOrUpdateModel& model)
        {
            auto client = Network::INetworkManager::Current().CreateClient(
                ServiceEndpoint,
                ServicePort);

            client->SetAuthenticationToken("Bearer", token.AccessToken);

            auto urlBuilder = std::stringstream();
            urlBuilder << "/v1/packages/" << name;
            auto url = urlBuilder.str();

            std::stringstream content;
            SoupApiJsonModels::SerializePackageCreateOrUpdate(model, content);

            auto contentType = "application/json";

            Log::Diag(url);
            auto response = client->Put(url, contentType, content);

            // Verify that we got a success
            if (response.StatusCode != Network::HttpStatusCode::Created)
            {
                throw ApiException("CreatePackage", response.StatusCode);
            }

            // Parse the return result
            auto result = SoupApiJsonModels::ParsePackageResult(response.Body);
            return result;
        }

        /// <summary>
        /// Search for a package using the query string
        /// </summary>
        // Task<SearchModel> SearchPackagesAsync(string q)
        // {
        //     using (HttpClient client = new HttpClient())
        //     {
        //         var url = $"{Constants.SoupRESTEndpointV1}/packages?q={q}";
        //         Log.Verbose(url);
        //         var response = await client.GetAsync(url);

        //         // Verify that we got a success
        //         response.EnsureSuccessStatusCode();

        //         // Parse the return result
        //         var content = await response.Content.ReadAsStringAsync();
        //         var result = JsonConvert.DeserializeObject<SearchModel>(content);

        //         return result;
        //     }
        // }
    };

// #define LOCAL_DEBUG
#ifdef LOCAL_DEBUG
    /*static*/ const std::string_view SoupApi::ServiceEndpoint = "localhost";
    /*static*/ const int SoupApi::ServicePort = 7071;
#else
    /*static*/ const std::string_view SoupApi::ServiceEndpoint = "api.soupbuild.com";
    /*static*/ const int SoupApi::ServicePort = 443;
#endif
}
