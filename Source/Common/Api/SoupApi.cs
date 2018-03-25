// <copyright company="Soup">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

using Newtonsoft.Json;
using Soup.Api.Results;
using System.IO;
using System.Net;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Threading.Tasks;

namespace Soup.Api
{
	/// <summary>
	/// Represents a collection of functions to interact with the API endpoints
	/// </summary>
	public class SoupApi : ISoupApi
	{
		/// <summary>
		/// Publish a new package version as an archive 
		/// </summary>
		public async Task<Stream> DownloadPackageAsync(string name, SemanticVersion version)
		{
			using (HttpClient client = new HttpClient())
			{
				var url = $"{Constants.SoupRESTEndpointV1}/package/{name}/{version}/{name}_{version}.tgz";
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
		public async Task<Package> GetPackageAsync(string name)
		{
			using (HttpClient client = new HttpClient())
			{
				var url = $"{Constants.SoupRESTEndpointV1}/package/{name}";
				var response = await client.GetAsync(url);

				// Verify that we got a success
				response.EnsureSuccessStatusCode();

				// Parse the return result
				var content = await response.Content.ReadAsStringAsync();
				var result = JsonConvert.DeserializeObject<Package>(content);

				return result;
			}
		}

		/// <summary>
		/// Get a package publication metadata 
		/// </summary>
		public async Task<Publication> GetPublicationAsync(string name, SemanticVersion version)
		{
			using (HttpClient client = new HttpClient())
			{
				var url = $"{Constants.SoupRESTEndpointV1}/package/{name}/{version}";
				var response = await client.GetAsync(url);

				// Verify that we got a success
				response.EnsureSuccessStatusCode();

				// Parse the return result
				var content = await response.Content.ReadAsStringAsync();
				var result = JsonConvert.DeserializeObject<Publication>(content);

				return result;
			}
		}

		/// <summary>
		/// Publish a new package version as an archive 
		/// </summary>
		public async Task<bool> PublishPackageAsync(Stream value)
		{
			using (HttpClient client = new HttpClient())
			{
				var content = new StreamContent(value);
				content.Headers.ContentType = new MediaTypeHeaderValue("application/zip");
				var url = $"{Constants.SoupRESTEndpointV1}/package";
				var response = await client.PutAsync(url, content);

				// Check if the publish was a no-op
				if (response.StatusCode == HttpStatusCode.Conflict)
					return false;

				// Verify that we got a success
				response.EnsureSuccessStatusCode();

				return true;
			}
		}

		/// <summary>
		/// Search for a package using the query string 
		/// </summary>
		public async Task<Search> SearchPackagesAsync(string q)
		{
			using (HttpClient client = new HttpClient())
			{
				var url = $"{Constants.SoupRESTEndpointV1}/packages/search?q={q}";
				var response = await client.GetAsync(url);

				// Verify that we got a success
				response.EnsureSuccessStatusCode();

				// Parse the return result
				var content = await response.Content.ReadAsStringAsync();
				var result = JsonConvert.DeserializeObject<Search>(content);

				return result;
			}
		}
	}
}
