// <copyright file="SearchClient.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization.Metadata;
using System.Threading;
using System.Threading.Tasks;

namespace Soup.Build.Api.Client;

/// <summary>
/// The search client.
/// </summary>
public class SearchClient
{
	private readonly HttpClient httpClient;
	private readonly string bearerToken;

	public SearchClient(HttpClient httpClient, string bearerToken)
	{
		this.httpClient = httpClient;
		this.bearerToken = bearerToken;
	}

	public Uri BaseUrl { get; init; } = new Uri("https://api.soupbuild.com");

	/// <summary>
	/// Search for packages.
	/// </summary>
	/// <param name="q">The query search parameter.</param>
	/// <param name="skip">The query skip parameter.</param>
	/// <param name="take">The query take parameter.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual Task<SearchPackagesModel> SearchPackagesAsync(string q, int? skip, int? take)
	{
		return SearchPackagesAsync(q, skip, take, CancellationToken.None);
	}

	/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
	/// <summary>
	/// Search for packages.
	/// </summary>
	/// <param name="q">The query search parameter.</param>
	/// <param name="skip">The query skip parameter.</param>
	/// <param name="take">The query take parameter.</param>
	/// <returns>The action result.</returns>
	/// <exception cref="ApiException">A server side error occurred.</exception>
	public virtual async Task<SearchPackagesModel> SearchPackagesAsync(
		string q, int? skip, int? take, CancellationToken cancellationToken)
	{
		var urlBuilder = new StringBuilder();
		_ = urlBuilder
			.Append(this.BaseUrl.OriginalString.TrimEnd('/'))
			.Append("/v1/search/packages?");
		if (q is not null)
		{
			_ = urlBuilder
				.Append(Uri.EscapeDataString("q") + "=")
				.Append(Uri.EscapeDataString(q))
				.Append('&');
		}

		if (skip is not null)
		{
			_ = urlBuilder
				.Append(Uri.EscapeDataString("skip") + "=")
				.Append(Uri.EscapeDataString($"{skip}"))
				.Append('&');
		}

		if (take is not null)
		{
			_ = urlBuilder
				.Append(Uri.EscapeDataString("take") + "=")
				.Append(Uri.EscapeDataString($"{take}"))
				.Append('&');
		}

		urlBuilder.Length--;

		var client = this.httpClient;

		using var requestMessage = await CreateHttpRequestMessageAsync().ConfigureAwait(false);
		requestMessage.Method = new HttpMethod("GET");
		requestMessage.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

		var url = urlBuilder.ToString();
		requestMessage.RequestUri = new Uri(url, UriKind.RelativeOrAbsolute);

		using var response = await client.SendAsync(
			requestMessage,
			HttpCompletionOption.ResponseHeadersRead,
			cancellationToken).ConfigureAwait(false);

		var status = (int)response.StatusCode;
		if (status == 200)
		{
			var objectResponse = await ReadObjectResponseAsync(
				response,
				SourceGenerationContext.Default.SearchPackagesModel, cancellationToken).ConfigureAwait(false);
			return objectResponse;
		}
		else
		{
			throw new ApiException(
				"The HTTP status code of the response was not expected.", response.StatusCode, null, null);
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

			var typedBody = await JsonSerializer.DeserializeAsync(
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