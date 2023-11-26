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
	private HttpClient _httpClient;
	private string? _bearerToken;

	public PackagesClient(HttpClient httpClient, string? bearerToken)
	{
		_httpClient = httpClient;
		_bearerToken = bearerToken;
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
		var urlBuilder_ = new StringBuilder();
		urlBuilder_.Append(BaseUrl.OriginalString.TrimEnd('/')).Append("/v1/languages/{languageName}/packages/{packageName}");
		urlBuilder_.Replace("{languageName}", Uri.EscapeDataString(languageName));
		urlBuilder_.Replace("{packageName}", Uri.EscapeDataString(packageName));

		var client_ = _httpClient;
		var disposeClient_ = false;
		try
		{
			using (var request_ = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false))
			{
				request_.Method = new HttpMethod("GET");
				request_.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

				var url_ = urlBuilder_.ToString();
				request_.RequestUri = new Uri(url_, UriKind.RelativeOrAbsolute);

				var response_ = await client_.SendAsync(request_, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);
				var disposeResponse_ = true;
				try
				{
					var headers_ = Enumerable.ToDictionary(response_.Headers, h_ => h_.Key, h_ => h_.Value);
					if (response_.Content != null && response_.Content.Headers != null)
					{
						foreach (var item_ in response_.Content.Headers)
							headers_[item_.Key] = item_.Value;
					}

					var status_ = (int)response_.StatusCode;
					if (status_ == 200)
					{
						var objectResponse = await ReadObjectResponseAsync<PackageModel>(
							response_, headers_, SourceGenerationContext.Default.PackageModel, cancellationToken).ConfigureAwait(false);
						return objectResponse;
					}
					else
					{
						throw new ApiException("The HTTP status code of the response was not expected.", status_, headers_, null);
					}
				}
				finally
				{
					if (disposeResponse_)
						response_.Dispose();
				}
			}
		}
		finally
		{
			if (disposeClient_)
				client_.Dispose();
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
		var urlBuilder_ = new StringBuilder();
		urlBuilder_.Append(BaseUrl.OriginalString.TrimEnd('/')).Append("/v1/languages/{languageName}/packages/{packageName}");
		urlBuilder_.Replace("{languageName}", Uri.EscapeDataString(languageName));
		urlBuilder_.Replace("{packageName}", Uri.EscapeDataString(packageName));

		var client_ = _httpClient;
		var disposeClient_ = false;
		try
		{
			using var request = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false);
			using var jsonContent = new MemoryStream();

			await JsonSerializer.SerializeAsync(
				jsonContent, model, SourceGenerationContext.Default.PackageCreateOrUpdateModel, cancellationToken);
			jsonContent.Seek(0, SeekOrigin.Begin);

			using var content = new StreamContent(jsonContent);
			content.Headers.ContentType = MediaTypeHeaderValue.Parse("application/json");

			request.Content = content;
			request.Method = new HttpMethod("PUT");
			request.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

			var url_ = urlBuilder_.ToString();
			request.RequestUri = new Uri(url_, UriKind.RelativeOrAbsolute);

			var response_ = await client_.SendAsync(
				request, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);
			var disposeResponse_ = true;
			try
			{
				var headers_ = Enumerable.ToDictionary(response_.Headers, h_ => h_.Key, h_ => h_.Value);
				if (response_.Content != null && response_.Content.Headers != null)
				{
					foreach (var item_ in response_.Content.Headers)
						headers_[item_.Key] = item_.Value;
				}


				var status_ = (int)response_.StatusCode;
				if (status_ == 200 || status_ == 201)
				{
					var objectResponse = await ReadObjectResponseAsync<PackageModel>(
						response_, headers_, SourceGenerationContext.Default.PackageModel, cancellationToken).ConfigureAwait(false);

					return objectResponse;
				}
				else
				{
					throw new ApiException("The HTTP status code of the response was not expected.", status_, headers_, null);
				}
			}
			finally
			{
				if (disposeResponse_)
					response_.Dispose();
			}
		}
		finally
		{
			if (disposeClient_)
				client_.Dispose();
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
			using (var responseStream = await response.Content.ReadAsStreamAsync(cancellationToken).ConfigureAwait(false))
			{
				var typedBody = await JsonSerializer.DeserializeAsync<T>(
					responseStream, jsonTypeInfo, cancellationToken).ConfigureAwait(false);
				if (typedBody is null)
				{
					var message = "Response body was empty.";
					throw new ApiException(message, (int)response.StatusCode, headers, null);
				}

				return typedBody;
			}
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
	/// <param name="cancellationToken">The cancellation token.</param>
	protected Task<HttpRequestMessage> CreateHttpRequestMessageAsync(CancellationToken cancellationToken)
	{
		var request = new HttpRequestMessage();
		if (!string.IsNullOrEmpty(_bearerToken))
		{
			request.Headers.Authorization = new AuthenticationHeaderValue("Bearer", _bearerToken);
		}

		return Task.FromResult(request);
	}
}
