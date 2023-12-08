// <copyright file="PackageClient.cs" company="Soup">
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
/// The package client.
/// </summary>
public class PackagesClient
{
	private readonly HttpClient httpClient;
	private readonly string? bearerToken;

	public PackagesClient(HttpClient httpClient, string? bearerToken)
	{
		this.httpClient = httpClient;
		this.bearerToken = bearerToken;
	}

	public Uri BaseUrl { get; init; } = new Uri("http://localhost:7070");

	/// <summary>
	/// Get a package by unique name.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task<PackageModel> GetPackageAsync(string languageName, string packageName)
	{
		return GetPackageAsync(languageName, packageName, CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Get a package by unique name.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual async Task<PackageModel> GetPackageAsync(string languageName, string packageName, CancellationToken cancellationToken)
	{
		var urlBuilder = new StringBuilder();
		_ = urlBuilder.Append(BaseUrl.OriginalString.TrimEnd('/')).Append("/v1/languages/{languageName}/packages/{packageName}");
		_ = urlBuilder.Replace("{languageName}", Uri.EscapeDataString(languageName));
		_ = urlBuilder.Replace("{packageName}", Uri.EscapeDataString(packageName));

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
			var objectResponse = await ReadObjectResponseAsync<PackageModel>(
				response, headers, SourceGenerationContext.Default.PackageModel, cancellationToken).ConfigureAwait(false);
			return objectResponse;
		}
		else
		{
			throw new ApiException("The HTTP status code of the response was not expected.", status, headers, null);
		}
	}

	/// <summary>
	/// Create or update a package identified by the unique name.
	/// </summary>
	/// <param name="languageName">The language name.</param>
	/// <param name="packageName">The package name.</param>
	/// <param name="model">The incoming model.</param>
	/// <returns>Updated.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task<PackageModel> CreateOrUpdatePackageAsync(string languageName, string packageName, PackageCreateOrUpdateModel model)
	{
		return CreateOrUpdatePackageAsync(languageName, packageName, model, CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Create or update a package identified by the unique name.
	/// </summary>
	/// <param name="languageName">The language name.</param>
	/// <param name="packageName">The package name.</param>
	/// <param name="model">The incoming model.</param>
	/// <returns>Updated.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual async Task<PackageModel> CreateOrUpdatePackageAsync(
		string languageName, string packageName, PackageCreateOrUpdateModel model, CancellationToken cancellationToken)
	{
		var urlBuilder = new StringBuilder();
		_ = urlBuilder.Append(BaseUrl.OriginalString.TrimEnd('/')).Append("/v1/languages/{languageName}/packages/{packageName}");
		_ = urlBuilder.Replace("{languageName}", Uri.EscapeDataString(languageName));
		_ = urlBuilder.Replace("{packageName}", Uri.EscapeDataString(packageName));

		var client = this.httpClient;

		using var requestMessage = await CreateHttpRequestMessageAsync().ConfigureAwait(false);
		using var jsonContent = new MemoryStream();

		await JsonSerializer.SerializeAsync(
			jsonContent, model, SourceGenerationContext.Default.PackageCreateOrUpdateModel, cancellationToken);
		_ = jsonContent.Seek(0, SeekOrigin.Begin);

		using var content = new StreamContent(jsonContent);
		content.Headers.ContentType = MediaTypeHeaderValue.Parse("application/json");

		requestMessage.Content = content;
		requestMessage.Method = new HttpMethod("PUT");
		requestMessage.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

		var url = urlBuilder.ToString();
		requestMessage.RequestUri = new Uri(url, UriKind.RelativeOrAbsolute);

		using var response = await client.SendAsync(
			requestMessage, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);

		var headers = Enumerable.ToDictionary(response.Headers, h => h.Key, h => h.Value);
		if (response.Content != null && response.Content.Headers != null)
		{
			foreach (var item in response.Content.Headers)
				headers[item.Key] = item.Value;
		}

		var status = (int)response.StatusCode;
		if (status is 200 or 201)
		{
			var objectResponse = await ReadObjectResponseAsync<PackageModel>(
				response, headers, SourceGenerationContext.Default.PackageModel, cancellationToken).ConfigureAwait(false);

			return objectResponse;
		}
		else
		{
			throw new ApiException("The HTTP status code of the response was not expected.", status, headers, null);
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
