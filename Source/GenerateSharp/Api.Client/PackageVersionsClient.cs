// <copyright file="PackageVersionClient.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization.Metadata;
using System.Threading;
using System.Threading.Tasks;

namespace Soup.Build.Api.Client;

/// <summary>
/// The package version client.
/// </summary>
public class PackageVersionsClient
{
	private readonly HttpClient httpClient;
	private readonly string? bearerToken;

	public PackageVersionsClient(HttpClient httpClient, string? bearerToken)
	{
		this.httpClient = httpClient;
		this.bearerToken = bearerToken;
	}

	public Uri BaseUrl { get; init; } = new Uri("http://localhost:7070");

	/// <summary>
	/// Get a package version.
	/// </summary>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to get.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task<PackageVersionModel> GetPackageVersionAsync(
		string ownerName,
		string languageName,
		string packageName,
		string packageVersion)
	{
		return GetPackageVersionAsync(ownerName, languageName, packageName, packageVersion, CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Get a package version.
	/// </summary>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to get.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual async Task<PackageVersionModel> GetPackageVersionAsync(
		string ownerName,
		string languageName,
		string packageName,
		string packageVersion,
		CancellationToken cancellationToken)
	{
		var urlBuilder = new StringBuilder();
		_ = urlBuilder
			.Append(BaseUrl.OriginalString.TrimEnd('/'))
			.Append("/v1/users/{ownerName}/packages/{languageName}/{packageName}/versions/{packageVersion}");
		_ = urlBuilder.Replace(
			"{ownerName}",
			Uri.EscapeDataString(ownerName));
		_ = urlBuilder.Replace(
			"{languageName}",
			Uri.EscapeDataString(languageName));
		_ = urlBuilder.Replace(
			"{packageName}",
			Uri.EscapeDataString(packageName));
		_ = urlBuilder.Replace(
			"{packageVersion}",
			Uri.EscapeDataString(packageVersion));

		var client = this.httpClient;

		using var requestMessage = await CreateHttpRequestMessageAsync().ConfigureAwait(false);
		requestMessage.Method = new HttpMethod("GET");
		requestMessage.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

		var url = urlBuilder.ToString();
		requestMessage.RequestUri = new Uri(url, UriKind.RelativeOrAbsolute);

		using var response = await client.SendAsync(requestMessage, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);

		var headers = Enumerable.ToDictionary(response.Headers, h => h.Key, h => h.Value);
		if (response.Content != null && response.Content.Headers != null)
		{
			foreach (var item in response.Content.Headers)
				headers[item.Key] = item.Value;
		}

		var status = (int)response.StatusCode;
		if (status == 200)
		{
			var objectResponse = await ReadObjectResponseAsync<PackageVersionModel>(
				response, headers, SourceGenerationContext.Default.PackageVersionModel, cancellationToken).ConfigureAwait(false);
			return objectResponse;
		}
		else
		{
			throw new ApiException(
				"The HTTP status code of the response was not expected.", status, headers, null);
		}
	}

	/// <summary>
	/// Publish a new version of a package.
	/// </summary>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to publish.</param>
	/// <param name="file">The uploaded file.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task PublishPackageVersionAsync(
		string ownerName,
		string languageName,
		string packageName,
		string packageVersion,
		FileParameter file)
	{
		return PublishPackageVersionAsync(ownerName, languageName, packageName, packageVersion, file, CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Publish a new version of a package.
	/// </summary>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to publish.</param>
	/// <param name="file">The uploaded file.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual async Task PublishPackageVersionAsync(
		string ownerName,
		string languageName,
		string packageName,
		string packageVersion,
		FileParameter file,
		CancellationToken cancellationToken)
	{
		var urlBuilder = new StringBuilder();
		_ = urlBuilder
			.Append(BaseUrl.OriginalString.TrimEnd('/'))
			.Append("/v1/users/{ownerName}/packages/{languageName}/{packageName}/versions/{packageVersion}");
		_ = urlBuilder.Replace("{ownerName}", Uri.EscapeDataString(ownerName));
		_ = urlBuilder.Replace("{languageName}", Uri.EscapeDataString(languageName));
		_ = urlBuilder.Replace("{packageName}", Uri.EscapeDataString(packageName));
		_ = urlBuilder.Replace("{packageVersion}", Uri.EscapeDataString(packageVersion));

		var client = this.httpClient;

		using var request = await CreateHttpRequestMessageAsync().ConfigureAwait(false);
		using var content = new StreamContent(file.Data);

		content.Headers.ContentType = MediaTypeHeaderValue.Parse(file.ContentType);
		request.Content = content;
		request.Method = new HttpMethod("PUT");

		var url = urlBuilder.ToString();
		request.RequestUri = new Uri(url, UriKind.RelativeOrAbsolute);

		using var response = await client.SendAsync(
			request, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);

		var headers = Enumerable.ToDictionary(response.Headers, h => h.Key, h => h.Value);
		if (response.Content != null && response.Content.Headers != null)
		{
			foreach (var item in response.Content.Headers)
				headers[item.Key] = item.Value;
		}

		var status = (int)response.StatusCode;
		if (status == 201)
		{
			return;
		}
		else
		{
			throw new ApiException("The HTTP status code of the response was not expected.", status, headers, null);
		}
	}

	/// <summary>
	/// Download a package version archive.
	/// </summary>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to download.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task<FileResponse> DownloadPackageVersionAsync(
		string ownerName,
		string languageName,
		string packageName,
		string packageVersion)
	{
		return DownloadPackageVersionAsync(ownerName, languageName, packageName, packageVersion, CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Download a package version archive.
	/// </summary>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to download.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual async Task<FileResponse> DownloadPackageVersionAsync(
		string ownerName,
		string languageName,
		string packageName,
		string packageVersion,
		CancellationToken cancellationToken)
	{
		var urlBuilder = new StringBuilder();
		_ = urlBuilder
			.Append(BaseUrl.OriginalString.TrimEnd('/'))
			.Append("/v1/users/{ownerName}/packages/{languageName}/{packageName}/versions/{packageVersion}/download");
		_ = urlBuilder.Replace("{ownerName}", Uri.EscapeDataString(ownerName));
		_ = urlBuilder.Replace("{languageName}", Uri.EscapeDataString(languageName));
		_ = urlBuilder.Replace("{packageName}", Uri.EscapeDataString(packageName));
		_ = urlBuilder.Replace("{packageVersion}", Uri.EscapeDataString(packageVersion));

		var client = this.httpClient;

		using var requestMessage = await CreateHttpRequestMessageAsync().ConfigureAwait(false);
		requestMessage.Method = new HttpMethod("GET");
		requestMessage.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

		var url = urlBuilder.ToString();
		requestMessage.RequestUri = new Uri(url, UriKind.RelativeOrAbsolute);

		var response = await client.SendAsync(
			requestMessage, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);
		var disposeResponse = true;
		try
		{
			var headers = Enumerable.ToDictionary(response.Headers, h => h.Key, h => h.Value);
			if (response.Content != null && response.Content.Headers != null)
			{
				foreach (var item in response.Content.Headers)
					headers[item.Key] = item.Value;
			}

			var status = (int)response.StatusCode;
			if (status is 200 or 206)
			{
				var responseStream = response.Content == null ?
					Stream.Null :
					await response.Content.ReadAsStreamAsync(cancellationToken).ConfigureAwait(false);
				var fileResponse = new FileResponse(status, headers, responseStream, null, response);

				// response is disposed by FileResponse
				disposeResponse = false;

				return fileResponse;
			}
			else
			{
				throw new ApiException("The HTTP status code of the response was not expected.", status, headers, null);
			}
		}
		finally
		{
			if (disposeResponse)
				response.Dispose();
		}
	}

	protected virtual async Task<T> ReadObjectResponseAsync<T>(
		HttpResponseMessage response,
		IReadOnlyDictionary<string, IEnumerable<string>> headers,
		JsonTypeInfo<T> jsonTypeInfo,
		CancellationToken cancellationToken)
	{
		try
		{
			using var responseStream = await response.Content.ReadAsStreamAsync(cancellationToken).ConfigureAwait(false);
			var typedBody = await JsonSerializer.DeserializeAsync<T>(
				responseStream, jsonTypeInfo, cancellationToken).ConfigureAwait(false);
			if (typedBody is null)
			{
				var message = "Response body was empty.";
				throw new ApiException(message, (int)response.StatusCode, headers, null);
			}

			return typedBody;
		}
		catch (JsonException exception)
		{
			var message = "Could not deserialize the response body stream as " + typeof(T).FullName + ".";
			throw new ApiException(message, (int)response.StatusCode, headers, exception);
		}
	}

	/// <summary>
	/// Called by implementing swagger client classes.
	/// </summary>
	protected Task<HttpRequestMessage> CreateHttpRequestMessageAsync()
	{
		var request = new HttpRequestMessage();
		if (!string.IsNullOrEmpty(this.bearerToken))
		{
			request.Headers.Authorization = new AuthenticationHeaderValue("Bearer", this.bearerToken);
		}

		return Task.FromResult(request);
	}
}