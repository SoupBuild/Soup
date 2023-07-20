// <copyright file="SearchClient.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Api.Client
{
	using System;
	using System.Collections.Generic;
	using System.Linq;
	using System.Net.Http;
	using System.Net.Http.Headers;
	using System.Text;
	using System.Text.Json;
	using System.Text.Json.Serialization.Metadata;
	using System.Threading;
	using System.Threading.Tasks;

	/// <summary>
	/// The search client.
	/// </summary>
	public class SearchClient
	{
		private HttpClient _httpClient;
		private string _bearerToken;

		public SearchClient(HttpClient httpClient, string bearerToken)
		{
			_httpClient = httpClient;
			_bearerToken = bearerToken;
		}

		public string BaseUrl { get; init; } = "http://localhost:7070";

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
			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl.TrimEnd('/')).Append("/v1/search/packages?");
			if (q is not null)
			{
				urlBuilder_
					.Append(Uri.EscapeDataString("q") + "=")
					.Append(Uri.EscapeDataString(q))
					.Append('&');
			}

			if (skip is not null)
			{
				urlBuilder_
					.Append(Uri.EscapeDataString("skip") + "=")
					.Append(Uri.EscapeDataString($"{skip}"))
					.Append('&');
			}

			if (take is not null)
			{
				urlBuilder_
					.Append(Uri.EscapeDataString("take") + "=")
					.Append(Uri.EscapeDataString($"{take}"))
					.Append('&');
			}

			urlBuilder_.Length--;

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
							var objectResponse = await ReadObjectResponseAsync<SearchPackagesModel>(
								response_, headers_, SourceGenerationContext.Default.SearchPackagesModel, cancellationToken).ConfigureAwait(false);
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
}
