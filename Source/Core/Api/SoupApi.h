// <copyright file="SoupApi.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "SoupApiJsonModels.h"
#include "ApiException.h"

namespace Soup::Api
{
    /// <summary>
    /// Represents a collection of functions to interact with the API endpoints
    /// </summary>
    class SoupApi
    {
    public:
        /// <summary>
        /// Download a package version as an archive
        /// </summary>
        // Task<Stream> DownloadPackageAsync(string name, SemanticVersion version)
        // {
        //     using (HttpClient client = new HttpClient())
        //     {
        //         var url = $"{Constants.SoupRESTEndpointV1}/packages/{name}/{version}/{name}_{version}.tgz";
        //         Log.Verbose(url);
        //         var response = await client.GetAsync(url);

        //         // Verify that we got a success
        //         response.EnsureSuccessStatusCode();

        //         // Parse the return result
        //         var result = await response.Content.ReadAsStreamAsync();

        //         return result;
        //     }
        // }

        /// <summary>
        /// Get the metadata for a package identified by the unique name
        /// </summary>
        static PackageResultModel GetPackage(std::string_view name)
        {
            auto client = Network::INetworkManager::Current().CreateClient(
                "localhost",
                7071);

            auto urlBuilder = std::stringstream();
            urlBuilder << "/api/v1/packages/" << name;
            auto url = urlBuilder.str();

            auto response = client->Get(url);

            // Verify that we got a success
            if (response.StatusCode != Network::HttpStatusCode::Ok)
            {
                throw ApiException(response.StatusCode);
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
        static void PublishPackage(
            std::string_view name,
            SemanticVersion version,
            std::istream& value)
        {
            auto client = Network::INetworkManager::Current().CreateClient(
                "localhost",
                7071);

            auto urlBuilder = std::stringstream();
            urlBuilder << "/api/v1/packages/" << name << "/v" << version.ToString();
            auto url = urlBuilder.str();

            auto contentType = "application/x-7z-compressed";

            auto response = client->Put(url, contentType, value);

            // Verify that we got a success
            switch (response.StatusCode)
            {
                case Network::HttpStatusCode::Created:
                    // All Good
                    break;
                case Network::HttpStatusCode::Conflict:
                    // This version already existed
                    throw std::runtime_error("Package version already exists");
                default:
                    throw ApiException(response.StatusCode);
            }
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
}
