// <copyright file="PackageVersionClient.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.IO;
using System.Net;
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

	public Uri BaseUrl { get; init; } = new Uri("https://api.soupbuild.com");

	/// <summary>
	/// Get a package version.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to get.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task<PackageVersionModel> GetPackageVersionAsync(
		string languageName,
		string ownerName,
		string packageName,
		string packageVersion)
	{
		return GetPackageVersionAsync(languageName, ownerName, packageName, packageVersion, CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Get a package version.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to get.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual async Task<PackageVersionModel> GetPackageVersionAsync(
		string languageName,
		string ownerName,
		string packageName,
		string packageVersion,
		CancellationToken cancellationToken)
	{
		var urlBuilder = new StringBuilder();
		_ = urlBuilder
			.Append(BaseUrl.OriginalString.TrimEnd('/'))
			.Append(CultureInfo.InvariantCulture, $"/v1/packages/{Uri.EscapeDataString(languageName)}/{Uri.EscapeDataString(ownerName)}/{Uri.EscapeDataString(packageName)}/versions/{Uri.EscapeDataString(packageVersion)}");

		var client = this.httpClient;

		using var requestMessage = await CreateHttpRequestMessageAsync().ConfigureAwait(false);
		requestMessage.Method = new HttpMethod("GET");
		requestMessage.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

		var url = urlBuilder.ToString();
		requestMessage.RequestUri = new Uri(url, UriKind.RelativeOrAbsolute);

		using var response = await client.SendAsync(
			requestMessage, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);

		var status = (int)response.StatusCode;
		if (status == 200)
		{
			var objectResponse = await ReadObjectResponseAsync<PackageVersionModel>(
				response,
				SourceGenerationContext.Default.PackageVersionModel,
				cancellationToken).ConfigureAwait(false);
			return objectResponse;
		}
		else
		{
			throw new ApiException(
				"The HTTP status code of the response was not expected.", response.StatusCode, null, null);
		}
	}

	/// <summary>
	/// Publish a new version of a package.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to publish.</param>
	/// <param name="file">The uploaded file.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task PublishPackageVersionAsync(
		string languageName,
		string ownerName,
		string packageName,
		string packageVersion,
		FileParameter file)
	{
		return PublishPackageVersionAsync(
			languageName,
			ownerName,
			packageName,
			packageVersion,
			file,
			CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Publish a new version of a package.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to publish.</param>
	/// <param name="file">The uploaded file.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	[SuppressMessage("Style", "IDE0010:Add missing cases", Justification = "Allow default handler")]
	public virtual async Task PublishPackageVersionAsync(
		string languageName,
		string ownerName,
		string packageName,
		string packageVersion,
		FileParameter file,
		CancellationToken cancellationToken)
	{
		var urlBuilder = new StringBuilder();
		_ = urlBuilder
			.Append(BaseUrl.OriginalString.TrimEnd('/'))
			.Append(CultureInfo.InvariantCulture, $"/v1/packages/{Uri.EscapeDataString(languageName)}/{Uri.EscapeDataString(ownerName)}/{Uri.EscapeDataString(packageName)}/versions/{Uri.EscapeDataString(packageVersion)}");

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

		switch (response.StatusCode)
		{
			case HttpStatusCode.Created:
				break;
			default:
				throw new ApiException("The HTTP status code of the response was not expected.", response.StatusCode, null, null);
		};
	}

	/// <summary>
	/// Download a package version archive.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to download.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task<FileResponse> DownloadPackageVersionAsync(
		string languageName,
		string ownerName,
		string packageName,
		string packageVersion)
	{
		return DownloadPackageVersionAsync(languageName, ownerName, packageName, packageVersion, CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Download a package version archive.
	/// </summary>
	/// <param name="languageName">The name of the language.</param>
	/// <param name="ownerName">The owner user name.</param>
	/// <param name="packageName">The unique name of the package.</param>
	/// <param name="packageVersion">The package version to download.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual async Task<FileResponse> DownloadPackageVersionAsync(
		string languageName,
		string ownerName,
		string packageName,
		string packageVersion,
		CancellationToken cancellationToken)
	{
		var urlBuilder = new StringBuilder();
		_ = urlBuilder
			.Append(BaseUrl.OriginalString.TrimEnd('/'))
			.Append(CultureInfo.InvariantCulture, $"/v1/packages/{Uri.EscapeDataString(languageName)}/{Uri.EscapeDataString(ownerName)}/{Uri.EscapeDataString(packageName)}/versions/{Uri.EscapeDataString(packageVersion)}/download");

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
			var status = (int)response.StatusCode;
			if (status is 200 or 206)
			{
				var responseStream = response.Content == null ?
					Stream.Null :
					await response.Content.ReadAsStreamAsync(cancellationToken).ConfigureAwait(false);
				var fileResponse = new FileResponse(status, null, responseStream, null, response);

				// response is disposed by FileResponse
				disposeResponse = false;

				return fileResponse;
			}
			else
			{
				throw new ApiException("The HTTP status code of the response was not expected.", response.StatusCode, null, null);
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
				throw new ApiException(message, response.StatusCode, null, null);
			}

			return typedBody;
		}
		catch (JsonException exception)
		{
			var message = "Could not deserialize the response body stream as " + typeof(T).FullName + ".";
			throw new ApiException(message, response.StatusCode, null, exception);
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