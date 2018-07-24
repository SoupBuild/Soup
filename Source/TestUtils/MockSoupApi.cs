// <copyright company="Soup" file="MockSoupApi.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.TestUtils
{
	using System.IO;
	using System.Threading.Tasks;
	using Soup.Api;

	/// <summary>
	/// Represents a collection of functions to interact with the API endpoints
	/// </summary>
	public class MockSoupApi : ISoupApi
	{
		/// <summary>
		/// Publish a new package version as an archive 
		/// </summary>
		public Task<Stream> DownloadPackageAsync(string name, SemanticVersion version)
		{
			return Task.FromResult<Stream>(null);
		}

		/// <summary>
		/// Get the metadata for a package identified by the unique name 
		/// </summary>
		public Task<PackageResultModel> GetPackageAsync(string name)
		{
			return Task.FromResult<PackageResultModel>(null);
		}

		/// <summary>
		/// Get a package publication metadata 
		/// </summary>
		public Task<PublicationResultModel> GetPublicationAsync(string name, SemanticVersion version)
		{
			return Task.FromResult<PublicationResultModel>(null);
		}

		/// <summary>
		/// Publish a new package version as an archive 
		/// </summary>
		public Task<bool> PublishPackageAsync(Stream value)
		{
			return Task.FromResult(false);
		}

		/// <summary>
		/// Search for a package using the query string 
		/// </summary>
		public Task<SearchModel> SearchPackagesAsync(string q)
		{
			return Task.FromResult<SearchModel>(null);
		}
	}
}
