// <copyright file="PackageVersionClient.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Api.Client
{
	using System;
	using System.Globalization;
	using System.Net.Http;
	using System.Net.Http.Headers;
	using System.Text;
	using System.Text.Json;
	using System.Threading;
	using System.Threading.Tasks;

	/// <summary>
	/// The package version client.
	/// </summary>
	public partial class PackageVersionsClient
	{
		private string _baseUrl = "http://localhost:7070";
		private HttpClient _httpClient;
		private string _bearerToken;
		private System.Lazy<JsonSerializerOptions> _settings;

		public PackageVersionsClient(HttpClient httpClient, string bearerToken)
		{
			_httpClient = httpClient;
			_bearerToken = bearerToken;
			_settings = new System.Lazy<JsonSerializerOptions>(CreateSerializerSettings);
		}

		private JsonSerializerOptions CreateSerializerSettings()
		{
			var settings = new JsonSerializerOptions();
			UpdateJsonSerializerSettings(settings);
			return settings;
		}

		public string BaseUrl
		{
			get { return _baseUrl; }
			set { _baseUrl = value; }
		}

		protected JsonSerializerOptions JsonSerializerSettings { get { return _settings.Value; } }

		partial void UpdateJsonSerializerSettings(JsonSerializerOptions settings);

		partial void PrepareRequest(HttpClient client, HttpRequestMessage request, string url);
		partial void PrepareRequest(HttpClient client, HttpRequestMessage request, StringBuilder urlBuilder);
		partial void ProcessResponse(HttpClient client, HttpResponseMessage response);

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
				.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "")
				.Append("/v1/languages/{languageName}/packages/{packageName}/versions/{packageVersion}");
			urlBuilder_.Replace(
				"{languageName}",
				Uri.EscapeDataString(ConvertToString(languageName, CultureInfo.InvariantCulture)));
			urlBuilder_.Replace(
				"{packageName}",
				Uri.EscapeDataString(ConvertToString(packageName, CultureInfo.InvariantCulture)));
			urlBuilder_.Replace(
				"{packageVersion}",
				Uri.EscapeDataString(ConvertToString(packageVersion, CultureInfo.InvariantCulture)));

			var client_ = _httpClient;
			var disposeClient_ = false;
			try
			{
				using (var request_ = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false))
				{
					request_.Method = new HttpMethod("GET");
					request_.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

					PrepareRequest(client_, request_, urlBuilder_);

					var url_ = urlBuilder_.ToString();
					request_.RequestUri = new Uri(url_, UriKind.RelativeOrAbsolute);

					PrepareRequest(client_, request_, url_);

					var response_ = await client_.SendAsync(request_, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);
					var disposeResponse_ = true;
					try
					{
						var headers_ = System.Linq.Enumerable.ToDictionary(response_.Headers, h_ => h_.Key, h_ => h_.Value);
						if (response_.Content != null && response_.Content.Headers != null)
						{
							foreach (var item_ in response_.Content.Headers)
								headers_[item_.Key] = item_.Value;
						}

						ProcessResponse(client_, response_);

						var status_ = (int)response_.StatusCode;
						if (status_ == 200)
						{
							var objectResponse_ = await ReadObjectResponseAsync<PackageVersionModel>(response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException("Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}
							return objectResponse_.Object;
						}
						else
						if (status_ == 400)
						{
							var objectResponse_ = await ReadObjectResponseAsync<ProblemDetails>(response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException("Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}
							throw new ApiException<ProblemDetails>("A server side error occurred.", status_, objectResponse_.Text, headers_, objectResponse_.Object, null);
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
		public virtual async Task PublishPackageVersionAsync(string languageName, string packageName, string packageVersion, FileParameter file, CancellationToken cancellationToken)
		{
			if (file == null)
				throw new System.ArgumentNullException("file");

			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/languages/{languageName}/packages/{packageName}/versions/{packageVersion}");
			urlBuilder_.Replace("{languageName}", Uri.EscapeDataString(ConvertToString(languageName, CultureInfo.InvariantCulture)));
			urlBuilder_.Replace("{packageName}", Uri.EscapeDataString(ConvertToString(packageName, CultureInfo.InvariantCulture)));
			urlBuilder_.Replace("{packageVersion}", Uri.EscapeDataString(ConvertToString(packageVersion, CultureInfo.InvariantCulture)));

			var client_ = _httpClient;
			var disposeClient_ = false;
			try
			{
				using (var request_ = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false))
				{
					var content_ = new StreamContent(file.Data);
					content_.Headers.ContentType = MediaTypeHeaderValue.Parse(file.ContentType);
					request_.Content = content_;
					request_.Method = new HttpMethod("PUT");

					PrepareRequest(client_, request_, urlBuilder_);

					var url_ = urlBuilder_.ToString();
					request_.RequestUri = new Uri(url_, UriKind.RelativeOrAbsolute);

					PrepareRequest(client_, request_, url_);

					var response_ = await client_.SendAsync(request_, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);
					var disposeResponse_ = true;
					try
					{
						var headers_ = System.Linq.Enumerable.ToDictionary(response_.Headers, h_ => h_.Key, h_ => h_.Value);
						if (response_.Content != null && response_.Content.Headers != null)
						{
							foreach (var item_ in response_.Content.Headers)
								headers_[item_.Key] = item_.Value;
						}

						ProcessResponse(client_, response_);

						var status_ = (int)response_.StatusCode;
						if (status_ == 201)
						{
							return;
						}
						else
						if (status_ == 400)
						{
							var objectResponse_ = await ReadObjectResponseAsync<ProblemDetails>(response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException("Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}
							throw new ApiException<ProblemDetails>("A server side error occurred.", status_, objectResponse_.Text, headers_, objectResponse_.Object, null);
						}
						else
						if (status_ == 403)
						{
							var objectResponse_ = await ReadObjectResponseAsync<ProblemDetails>(response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException("Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}
							throw new ApiException<ProblemDetails>("A server side error occurred.", status_, objectResponse_.Text, headers_, objectResponse_.Object, null);
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
						if (status_ == 409)
						{
							var objectResponse_ = await ReadObjectResponseAsync<ProblemDetails>(response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException("Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}
							throw new ApiException<ProblemDetails>("A server side error occurred.", status_, objectResponse_.Text, headers_, objectResponse_.Object, null);
						}
						else
						if (status_ == 413)
						{
							var objectResponse_ = await ReadObjectResponseAsync<ProblemDetails>(response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException("Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}
							throw new ApiException<ProblemDetails>("A server side error occurred.", status_, objectResponse_.Text, headers_, objectResponse_.Object, null);
						}
						else
						if (status_ == 415)
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
		public virtual async Task<FileResponse> DownloadPackageVersionAsync(string languageName, string packageName, string packageVersion, CancellationToken cancellationToken)
		{
			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/languages/{languageName}/packages/{packageName}/versions/{packageVersion}/download");
			urlBuilder_.Replace("{languageName}", Uri.EscapeDataString(ConvertToString(languageName, CultureInfo.InvariantCulture)));
			urlBuilder_.Replace("{packageName}", Uri.EscapeDataString(ConvertToString(packageName, CultureInfo.InvariantCulture)));
			urlBuilder_.Replace("{packageVersion}", Uri.EscapeDataString(ConvertToString(packageVersion, CultureInfo.InvariantCulture)));

			var client_ = _httpClient;
			var disposeClient_ = false;
			try
			{
				using (var request_ = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false))
				{
					request_.Method = new HttpMethod("GET");
					request_.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/json"));

					PrepareRequest(client_, request_, urlBuilder_);

					var url_ = urlBuilder_.ToString();
					request_.RequestUri = new Uri(url_, UriKind.RelativeOrAbsolute);

					PrepareRequest(client_, request_, url_);

					var response_ = await client_.SendAsync(request_, HttpCompletionOption.ResponseHeadersRead, cancellationToken).ConfigureAwait(false);
					var disposeResponse_ = true;
					try
					{
						var headers_ = System.Linq.Enumerable.ToDictionary(response_.Headers, h_ => h_.Key, h_ => h_.Value);
						if (response_.Content != null && response_.Content.Headers != null)
						{
							foreach (var item_ in response_.Content.Headers)
								headers_[item_.Key] = item_.Value;
						}

						ProcessResponse(client_, response_);

						var status_ = (int)response_.StatusCode;
						if (status_ == 200 || status_ == 206)
						{
							var responseStream_ = response_.Content == null ?
								System.IO.Stream.Null :
								await response_.Content.ReadAsStreamAsync(cancellationToken).ConfigureAwait(false);
							var fileResponse_ = new FileResponse(status_, headers_, responseStream_, null, response_);
							disposeClient_ = false; disposeResponse_ = false; // response and client are disposed by FileResponse
							return fileResponse_;
						}
						else
						if (status_ == 400)
						{
							var objectResponse_ = await ReadObjectResponseAsync<ProblemDetails>(response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException(
									"Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}

							throw new ApiException<ProblemDetails>(
								"A server side error occurred.", status_, objectResponse_.Text, headers_, objectResponse_.Object, null);
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

		private string ConvertToString(object value, CultureInfo cultureInfo)
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
