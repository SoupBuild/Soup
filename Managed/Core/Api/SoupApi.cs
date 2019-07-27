// <copyright file="SoupApi.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api
{
    using System.IO;
    using System.Net;
    using System.Net.Http;
    using System.Net.Http.Headers;
    using System.Threading.Tasks;
    using Newtonsoft.Json;

    /// <summary>
    /// Represents a collection of functions to interact with the API endpoints
    /// </summary>
    public class SoupApi : ISoupApi
    {
        /// <summary>
        /// Download a package version as an archive
        /// </summary>
        public async Task<Stream> DownloadPackageAsync(string name, SemanticVersion version)
        {
            using (HttpClient client = new HttpClient())
            {
                var url = $"{Constants.SoupRESTEndpointV1}/packages/{name}/{version}/{name}_{version}.tgz";
                Log.Verbose(url);
                var response = await client.GetAsync(url);

                // Verify that we got a success
                response.EnsureSuccessStatusCode();

                // Parse the return result
                var result = await response.Content.ReadAsStreamAsync();

                return result;
            }
        }

        /// <summary>
        /// Get the metadata for a package identified by the unique name
        /// </summary>
        public async Task<PackageResultModel> GetPackageAsync(string name)
        {
            using (HttpClient client = new HttpClient())
            {
                var url = $"{Constants.SoupRESTEndpointV1}/packages/{name}";
                Log.Verbose(url);
                var response = await client.GetAsync(url);

                // Verify that we got a success
                if (!response.IsSuccessStatusCode)
                {
                    throw new ApiException(response.StatusCode);
                }

                // Parse the return result
                var content = await response.Content.ReadAsStringAsync();
                var result = JsonConvert.DeserializeObject<PackageResultModel>(content);

                return result;
            }
        }

        /// <summary>
        /// Get a package publication metadata
        /// </summary>
        public async Task<PublicationResultModel> GetPublicationAsync(string name, SemanticVersion version)
        {
            using (HttpClient client = new HttpClient())
            {
                var url = $"{Constants.SoupRESTEndpointV1}/packages/{name}/{version}";
                Log.Verbose(url);
                var response = await client.GetAsync(url);

                // Verify that we got a success
                response.EnsureSuccessStatusCode();

                // Parse the return result
                var content = await response.Content.ReadAsStringAsync();
                var result = JsonConvert.DeserializeObject<PublicationResultModel>(content);

                return result;
            }
        }

        /// <summary>
        /// Publish a new package version as an archive
        /// </summary>
        public async Task<bool> PublishPackageAsync(string name, Stream value)
        {
            using (HttpClient client = new HttpClient())
            {
                var content = new StreamContent(value);
                content.Headers.ContentType = new MediaTypeHeaderValue("application/zip");
                var url = $"{Constants.SoupRESTEndpointV1}/packages/{name}";
                Log.Verbose(url);
                var response = await client.PutAsync(url, content);

                // Check if the publish was a no-op
                if (response.StatusCode == HttpStatusCode.Conflict)
                {
                    return false;
                }

                // Verify that we got a success
                response.EnsureSuccessStatusCode();

                return true;
            }
        }

        /// <summary>
        /// Search for a package using the query string
        /// </summary>
        public async Task<SearchModel> SearchPackagesAsync(string q)
        {
            using (HttpClient client = new HttpClient())
            {
                var url = $"{Constants.SoupRESTEndpointV1}/packages?q={q}";
                Log.Verbose(url);
                var response = await client.GetAsync(url);

                // Verify that we got a success
                response.EnsureSuccessStatusCode();

                // Parse the return result
                var content = await response.Content.ReadAsStringAsync();
                var result = JsonConvert.DeserializeObject<SearchModel>(content);

                return result;
            }
        }
    }
}
