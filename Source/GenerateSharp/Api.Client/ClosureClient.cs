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
		private string _bearerToken;
		private Lazy<JsonSerializerOptions> _settings;

		public ClosureClient(HttpClient httpClient, string bearerToken)
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
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/closure/generate");

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
							var objectResponse_ = await ReadObjectResponseAsync<GenerateClosureResultModel>(
								response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException("Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}
							return objectResponse_.Object;
						}
						else if (status_ == 400)
						{
							var objectResponse_ = await ReadObjectResponseAsync<ProblemDetails>(response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException("Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}

							throw new ApiException<ProblemDetails>("A server side error occurred.", status_, objectResponse_.Text, headers_, objectResponse_.Object, null);
						}
						else
						{
							var responseData_ = response_.Content == null ?
								null :
								await response_.Content.ReadAsStringAsync(cancellationToken).ConfigureAwait(false);
							throw new ApiException("The HTTP status code of the response was not expected (" + status_ + ").", status_, responseData_, headers_, null);
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

		protected struct ObjectResponseResult<T>
		{
			public ObjectResponseResult(T responseObject, string responseText)
			{
				this.Object = responseObject;
				this.Text = responseText;
			}

			public T Object { get; }

			public string Text { get; }
		}

		public bool ReadResponseAsString { get; set; }

		protected virtual async Task<ObjectResponseResult<T>> ReadObjectResponseAsync<T>(
			HttpResponseMessage response,
			IReadOnlyDictionary<string, IEnumerable<string>> headers,
			CancellationToken cancellationToken)
		{
			if (response == null || response.Content == null)
			{
				return new ObjectResponseResult<T>(default(T), string.Empty);
			}

			if (ReadResponseAsString)
			{
				var responseText = await response.Content.ReadAsStringAsync(cancellationToken).ConfigureAwait(false);
				try
				{
					var typedBody = JsonSerializer.Deserialize<T>(responseText, JsonSerializerSettings);
					return new ObjectResponseResult<T>(typedBody, responseText);
				}
				catch (JsonException exception)
				{
					var message = "Could not deserialize the response body string as " + typeof(T).FullName + ".";
					throw new ApiException(message, (int)response.StatusCode, responseText, headers, exception);
				}
			}
			else
			{
				try
				{
					using (var responseStream = await response.Content.ReadAsStreamAsync(cancellationToken).ConfigureAwait(false))
					{
						var typedBody = await JsonSerializer.DeserializeAsync<T>(
							responseStream, JsonSerializerSettings, cancellationToken).ConfigureAwait(false);
						return new ObjectResponseResult<T>(typedBody, string.Empty);
					}
				}
				catch (JsonException exception)
				{
					var message = "Could not deserialize the response body stream as " + typeof(T).FullName + ".";
					throw new ApiException(message, (int)response.StatusCode, string.Empty, headers, exception);
				}
			}
		}

		private string ConvertToString(object value, CultureInfo cultureInfo)
		{
			if (value == null)
			{
				return "";
			}

			if (value is Enum)
			{
				var name = Enum.GetName(value.GetType(), value);
				if (name != null)
				{
					var field = IntrospectionExtensions.GetTypeInfo(value.GetType()).GetDeclaredField(name);
					if (field != null)
					{
						var attribute = CustomAttributeExtensions.GetCustomAttribute(field, typeof(EnumMemberAttribute)) as EnumMemberAttribute;
						if (attribute != null)
						{
							return attribute.Value != null ? attribute.Value : name;
						}
					}

					var converted = Convert.ToString(Convert.ChangeType(value, Enum.GetUnderlyingType(value.GetType()), cultureInfo));
					return converted == null ? string.Empty : converted;
				}
			}
			else if (value is bool)
			{
				return Convert.ToString((bool)value, cultureInfo).ToLowerInvariant();
			}
			else if (value is byte[])
			{
				return Convert.ToBase64String((byte[])value);
			}
			else if (value.GetType().IsArray)
			{
				var array = System.Linq.Enumerable.OfType<object>((System.Array)value);
				return string.Join(",", System.Linq.Enumerable.Select(array, o => ConvertToString(o, cultureInfo)));
			}

			var result = Convert.ToString(value, cultureInfo);
			return result == null ? "" : result;
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
