// <copyright file="LanguageClient.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Api.Client
{
	using System;
	using System.Collections.Generic;
	using System.Globalization;
	using System.Net.Http;
	using System.Net.Http.Headers;
	using System.Reflection;
	using System.Runtime.Serialization;
	using System.Text;
	using System.Text.Json;
	using System.Threading;
	using System.Threading.Tasks;

	/// <summary>
	/// The closure client.
	/// </summary>
	public class ClosureClient
	{
		private HttpClient _httpClient;
		private string? _bearerToken;
		private Lazy<JsonSerializerOptions> _settings;

		public ClosureClient(HttpClient httpClient, string? bearerToken)
		{
			_httpClient = httpClient;
			_bearerToken = bearerToken;
			_settings = new Lazy<JsonSerializerOptions>(CreateSerializerSettings);
		}

		private JsonSerializerOptions CreateSerializerSettings()
		{
			var settings = new JsonSerializerOptions();
			return settings;
		}

		public string BaseUrl { get; init; } = "http://localhost:7070";

		protected JsonSerializerOptions JsonSerializerSettings => _settings.Value;

		/// <summary>
		/// Generate a package closure.
		/// </summary>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public virtual Task<GenerateClosureResultModel> GenerateClosureAsync(GenerateClosureRequestModel request)
		{
			return GenerateClosureAsync(request, CancellationToken.None);
		}

		/// <param name="request"></param>
		/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
		/// <summary>
		/// Generate a package closure.
		/// </summary>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public virtual async Task<GenerateClosureResultModel> GenerateClosureAsync(
			GenerateClosureRequestModel request, CancellationToken cancellationToken)
		{
			if (request == null)
				throw new ArgumentNullException(nameof(request));

			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl.TrimEnd('/')).Append("/v1/closure/generate");

			var client_ = _httpClient;
			var disposeClient_ = false;
			try
			{
				using (var request_ = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false))
				{
					var json_ = JsonSerializer.Serialize(request, _settings.Value);
					var content_ = new StringContent(json_);
					content_.Headers.ContentType = MediaTypeHeaderValue.Parse("application/json");
					request_.Content = content_;
					request_.Method = new HttpMethod("GET");
					request_.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

					var url_ = urlBuilder_.ToString();
					request_.RequestUri = new Uri(url_, UriKind.RelativeOrAbsolute);

					var response_ = await client_.SendAsync(
						request_, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);
					var disposeResponse_ = true;
					try
					{
						var headers_ = System.Linq.Enumerable.ToDictionary(response_.Headers, h_ => h_.Key, h_ => h_.Value);
						if (response_.Content != null && response_.Content.Headers != null)
						{
							foreach (var item_ in response_.Content.Headers)
								headers_[item_.Key] = item_.Value;
						}

						var status_ = (int)response_.StatusCode;
						if (status_ == 200)
						{
							var objectResponse = await ReadObjectResponseAsync<GenerateClosureResultModel>(
								response_, headers_, cancellationToken).ConfigureAwait(false);
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

		protected virtual async Task<T> ReadObjectResponseAsync<T>(
			HttpResponseMessage response,
			IReadOnlyDictionary<string, IEnumerable<string>> headers,
			CancellationToken cancellationToken)
		{
			try
			{
				using (var responseStream = await response.Content.ReadAsStreamAsync(cancellationToken).ConfigureAwait(false))
				{
					var typedBody = await JsonSerializer.DeserializeAsync<T>(
						responseStream, JsonSerializerSettings, cancellationToken).ConfigureAwait(false);
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
}
