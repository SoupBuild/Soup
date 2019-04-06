// <copyright file="ISoupApi.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api
{
    using System.IO;
    using System.Threading.Tasks;

    /// <summary>
    /// Represents a collection of functions to interact with the API endpoints
    /// </summary>
    public interface ISoupApi
    {
        /// <summary>
        /// Publish a new package version as an archive
        /// </summary>
        Task<Stream> DownloadPackageAsync(string name, SemanticVersion version);

        /// <summary>
        /// Get the metadata for a package identified by the unique name
        /// </summary>
        Task<PackageResultModel> GetPackageAsync(string name);

        /// <summary>
        /// Get a package publication metadata
        /// </summary>
        Task<PublicationResultModel> GetPublicationAsync(string name, SemanticVersion version);

        /// <summary>
        /// Publish a new package version as an archive
        /// </summary>
        Task<bool> PublishPackageAsync(string name, Stream value);

        /// <summary>
        /// Search for a package using the query string
        /// </summary>
        Task<SearchModel> SearchPackagesAsync(string q);
    }
}
