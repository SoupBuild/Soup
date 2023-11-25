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
	private HttpClient _httpClient;
	private readonly string? _bearerToken;

	public PackageVersionsClient(HttpClient httpClient, string? bearerToken)
	{
		_httpClient = httpClient;
		_bearerToken = bearerToken;
	}

	public Uri BaseUrl { get; init; } = new Uri("http://localhost:7070");

	/// <summary>
	/// Get a package version.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to get.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task<PackageVersionModel> GetPackageVersionAsync(string languageName, string packageName, string packageVersion)
	{
		return GetPackageVersionAsync(languageName, packageName, packageVersion, CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Get a package version.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to get.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual async Task<PackageVersionModel> GetPackageVersionAsync(
		string languageName, string packageName, string packageVersion, CancellationToken cancellationToken)
	{
		var urlBuilder_ = new StringBuilder();
		urlBuilder_
			.Append(BaseUrl.OriginalString.TrimEnd('/'))
			.Append("/v1/languages/{languageName}/packages/{packageName}/versions/{packageVersion}");
		urlBuilder_.Replace(
			"{languageName}",
			Uri.EscapeDataString(languageName));
		urlBuilder_.Replace(
			"{packageName}",
			Uri.EscapeDataString(packageName));
		urlBuilder_.Replace(
			"{packageVersion}",
			Uri.EscapeDataString(packageVersion));

		var client = _httpClient;
		var disposeClient_ = false;
		try
		{
			using var request_ = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false);
			request_.Method = new HttpMethod("GET");
			request_.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

			var url_ = urlBuilder_.ToString();
			request_.RequestUri = new Uri(url_, UriKind.RelativeOrAbsolute);

			var response_ = await client.SendAsync(request_, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);
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
					var objectResponse = await ReadObjectResponseAsync<PackageVersionModel>(
						response_, headers_, SourceGenerationContext.Default.PackageVersionModel, cancellationToken).ConfigureAwait(false);
					return objectResponse;
				}
				else
				{
					throw new ApiException(
						"The HTTP status code of the response was not expected.", status_, headers_, null);
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
				client.Dispose();
		}
	}

	/// <summary>
	/// Publish a new version of a package.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to publish.</param>
	/// <param name="file">The uploaded file.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task PublishPackageVersionAsync(string languageName, string packageName, string packageVersion, FileParameter file)
	{
		return PublishPackageVersionAsync(languageName, packageName, packageVersion, file, CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Publish a new version of a package.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to publish.</param>
	/// <param name="file">The uploaded file.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual async Task PublishPackageVersionAsync(
		string languageName, string packageName, string packageVersion, FileParameter file, CancellationToken cancellationToken)
	{
		var urlBuilder_ = new StringBuilder();
		urlBuilder_.Append(BaseUrl.OriginalString.TrimEnd('/')).Append("/v1/languages/{languageName}/packages/{packageName}/versions/{packageVersion}");
		urlBuilder_.Replace("{languageName}", Uri.EscapeDataString(languageName));
		urlBuilder_.Replace("{packageName}", Uri.EscapeDataString(packageName));
		urlBuilder_.Replace("{packageVersion}", Uri.EscapeDataString(packageVersion));

		var client_ = _httpClient;
		var disposeClient_ = false;
		try
		{
			using var request = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false);
			using var content = new StreamContent(file.Data);

			content.Headers.ContentType = MediaTypeHeaderValue.Parse(file.ContentType);
			request.Content = content;
			request.Method = new HttpMethod("PUT");

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
				if (status_ == 201)
				{
					return;
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

	/// <summary>
	/// Download a package version archive.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to download.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task<FileResponse> DownloadPackageVersionAsync(string languageName, string packageName, string packageVersion)
	{
		return DownloadPackageVersionAsync(languageName, packageName, packageVersion, CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Download a package version archive.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to download.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual async Task<FileResponse> DownloadPackageVersionAsync(
		string languageName, string packageName, string packageVersion, CancellationToken cancellationToken)
	{
		var urlBuilder_ = new StringBuilder();
		urlBuilder_.Append(BaseUrl.OriginalString.TrimEnd('/')).Append("/v1/languages/{languageName}/packages/{packageName}/versions/{packageVersion}/download");
		urlBuilder_.Replace("{languageName}", Uri.EscapeDataString(languageName));
		urlBuilder_.Replace("{packageName}", Uri.EscapeDataString(packageName));
		urlBuilder_.Replace("{packageVersion}", Uri.EscapeDataString(packageVersion));

		var client_ = _httpClient;
		var disposeClient_ = false;
		try
		{
			using var request_ = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false);
			request_.Method = new HttpMethod("GET");
			request_.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

			var url_ = urlBuilder_.ToString();
			request_.RequestUri = new Uri(url_, UriKind.RelativeOrAbsolute);

			var response_ = await client_.SendAsync(
				request_, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);
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
				if (status_ == 200 || status_ == 206)
				{
					var responseStream_ = response_.Content == null ?
						Stream.Null :
						await response_.Content.ReadAsStreamAsync(cancellationToken).ConfigureAwait(false);
					var fileResponse_ = new FileResponse(status_, headers_, responseStream_, null, response_);

					// response and client are disposed by FileResponse
					disposeClient_ = false;
					disposeResponse_ = false;

					return fileResponse_;
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