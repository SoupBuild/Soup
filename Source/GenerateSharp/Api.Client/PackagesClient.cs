// <copyright file="PackageClient.cs" company="Soup">
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
	using System.Threading;
	using System.Threading.Tasks;

	/// <summary>
	/// The package client.
	/// </summary>
	public class PackagesClient
	{
		private HttpClient _httpClient;
		private string? _bearerToken;
		private Lazy<JsonSerializerOptions> _settings;

		public PackagesClient(HttpClient httpClient, string? bearerToken)
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
		public virtual async Task<PackageModel> GetPackageAsync(string languageName, string packageName, System.Threading.CancellationToken cancellationToken)
		{
			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/languages/{languageName}/packages/{packageName}");
			urlBuilder_.Replace("{languageName}", Uri.EscapeDataString(ConvertToString(languageName, System.Globalization.CultureInfo.InvariantCulture)));
			urlBuilder_.Replace("{packageName}", Uri.EscapeDataString(ConvertToString(packageName, System.Globalization.CultureInfo.InvariantCulture)));

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
							var objectResponse = await ReadObjectResponseAsync<PackageModel>(response_, headers_, cancellationToken).ConfigureAwait(false);
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
			return CreateOrUpdatePackageAsync(languageName, packageName, model, System.Threading.CancellationToken.None);
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
		public virtual async Task<PackageModel> CreateOrUpdatePackageAsync(string languageName, string packageName, PackageCreateOrUpdateModel model, System.Threading.CancellationToken cancellationToken)
		{
			if (model == null)
				throw new ArgumentNullException(nameof(model));

			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/languages/{languageName}/packages/{packageName}");
			urlBuilder_.Replace("{languageName}", Uri.EscapeDataString(ConvertToString(languageName, System.Globalization.CultureInfo.InvariantCulture)));
			urlBuilder_.Replace("{packageName}", Uri.EscapeDataString(ConvertToString(packageName, System.Globalization.CultureInfo.InvariantCulture)));

			var client_ = _httpClient;
			var disposeClient_ = false;
			try
			{
				using (var request_ = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false))
				{
					var json_ = JsonSerializer.Serialize(model, _settings.Value);
					var content_ = new StringContent(json_);
					content_.Headers.ContentType = MediaTypeHeaderValue.Parse("application/json");
					request_.Content = content_;
					request_.Method = new HttpMethod("PUT");
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
						if (status_ == 200 || status_ == 201)
						{
							var objectResponse = await ReadObjectResponseAsync<PackageModel>(response_, headers_, cancellationToken).ConfigureAwait(false);

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
