// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using Soup.Api.Results;
using System.IO;
using System.Threading.Tasks;

namespace Soup.Api
{
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
		Task<Package> GetPackageAsync(string name);

		/// <summary>
		/// Get a package publication metadata 
		/// </summary>
		Task<Publication> GetPublicationAsync(string name, SemanticVersion version);

		/// <summary>
		/// Publish a new package version as an archive 
		/// </summary>
		Task<bool> PublishPackageAsync(Stream value);

		/// <summary>
		/// Search for a package using the query string 
		/// </summary>
		Task<Search> SearchPackagesAsync(string q);
	}
}
