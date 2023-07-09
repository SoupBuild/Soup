// <copyright file="LanguageClient.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Api.Client
{
	using System.Net.Http;
	using System.Net.Http.Headers;
	using System.Text;
	using System.Text.Json;
	using System.Threading;
	using System.Threading.Tasks;

	/// <summary>
	/// The language client.
	/// </summary>
	public class LanguagesClient
	{

		private string _baseUrl = "http://localhost:7070";
		private HttpClient _httpClient;
		private string _bearerToken;
		private System.Lazy<JsonSerializerOptions> _settings;

		public LanguagesClient(HttpClient httpClient, string bearerToken)
		{
			_httpClient = httpClient;
			_bearerToken = bearerToken;
			_settings = new System.Lazy<JsonSerializerOptions>(CreateSerializerSettings);
		}

		private JsonSerializerOptions CreateSerializerSettings()
		{
			var settings = new JsonSerializerOptions();
			return settings;
		}

		public string BaseUrl
		{
			get { return _baseUrl; }
			set { _baseUrl = value; }
		}

		protected JsonSerializerOptions JsonSerializerSettings { get { return _settings.Value; } }

		/// <summary>
		/// Get a language by unique name.
		/// </summary>
		/// <param name="languageName">The unique name of the language.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public virtual Task<LanguageModel> GetLanguageAsync(string languageName)
		{
			return GetLanguageAsync(languageName, CancellationToken.None);
		}

		/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
		/// <summary>
		/// Get a language by unique name.
		/// </summary>
		/// <param name="languageName">The unique name of the language.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public virtual async Task<LanguageModel> GetLanguageAsync(string languageName, CancellationToken cancellationToken)
		{
			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/languages/{languageName}");
			urlBuilder_.Replace("{languageName}", System.Uri.EscapeDataString(ConvertToString(languageName, System.Globalization.CultureInfo.InvariantCulture)));

			var client_ = _httpClient;
			var disposeClient_ = false;
			try
			{
				using (var request_ = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false))
				{
					request_.Method = new HttpMethod("GET");
					request_.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

					var url_ = urlBuilder_.ToString();
					request_.RequestUri = new System.Uri(url_, System.UriKind.RelativeOrAbsolute);

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
							var objectResponse_ = await ReadObjectResponseAsync<LanguageModel>(response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException("Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}
							return objectResponse_.Object;
						}
						else
						if (status_ == 404)
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
							var responseData_ = response_.Content == null ? null : await response_.Content.ReadAsStringAsync().ConfigureAwait(false);
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

		protected virtual async Task<ObjectResponseResult<T>> ReadObjectResponseAsync<T>(HttpResponseMessage response, System.Collections.Generic.IReadOnlyDictionary<string, System.Collections.Generic.IEnumerable<string>> headers, CancellationToken cancellationToken)
		{
			if (response == null || response.Content == null)
			{
				return new ObjectResponseResult<T>(default(T), string.Empty);
			}

			if (ReadResponseAsString)
			{
				var responseText = await response.Content.ReadAsStringAsync().ConfigureAwait(false);
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
					using (var responseStream = await response.Content.ReadAsStreamAsync().ConfigureAwait(false))
					{
						var typedBody = await JsonSerializer.DeserializeAsync<T>(responseStream, JsonSerializerSettings, cancellationToken).ConfigureAwait(false);
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

		private string ConvertToString(object value, System.Globalization.CultureInfo cultureInfo)
		{
			if (value == null)
			{
				return "";
			}

			if (value is System.Enum)
			{
				var name = System.Enum.GetName(value.GetType(), value);
				if (name != null)
				{
					var field = System.Reflection.IntrospectionExtensions.GetTypeInfo(value.GetType()).GetDeclaredField(name);
					if (field != null)
					{
						var attribute = System.Reflection.CustomAttributeExtensions.GetCustomAttribute(field, typeof(System.Runtime.Serialization.EnumMemberAttribute))
							as System.Runtime.Serialization.EnumMemberAttribute;
						if (attribute != null)
						{
							return attribute.Value != null ? attribute.Value : name;
						}
					}

					var converted = System.Convert.ToString(System.Convert.ChangeType(value, System.Enum.GetUnderlyingType(value.GetType()), cultureInfo));
					return converted == null ? string.Empty : converted;
				}
			}
			else if (value is bool)
			{
				return System.Convert.ToString((bool)value, cultureInfo).ToLowerInvariant();
			}
			else if (value is byte[])
			{
				return System.Convert.ToBase64String((byte[])value);
			}
			else if (value.GetType().IsArray)
			{
				var array = System.Linq.Enumerable.OfType<object>((System.Array)value);
				return string.Join(",", System.Linq.Enumerable.Select(array, o => ConvertToString(o, cultureInfo)));
			}

			var result = System.Convert.ToString(value, cultureInfo);
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
