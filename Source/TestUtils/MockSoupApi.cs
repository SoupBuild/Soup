// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using Newtonsoft.Json;
using Soup.Api;
using Soup.Api.Results;
using System.IO;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Threading.Tasks;

namespace Soup.TestUtils
{
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
		public Task<Package> GetPackageAsync(string name)
		{
			return Task.FromResult<Package>(null);
		}

		/// <summary>
		/// Get a package publication metadata 
		/// </summary>
		public Task<Publication> GetPublicationAsync(string name, SemanticVersion version)
		{
			return Task.FromResult<Publication>(null);
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
		public Task<Search> SearchPackagesAsync(string q)
		{
			return Task.FromResult<Search>(null);
		}
	}
}
