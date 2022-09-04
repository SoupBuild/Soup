
using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.IO;
using System.Net.Http;
using System.Net.Http.Headers;
using System.Text;
using System.Text.Json;
using System.Threading;
using System.Threading.Tasks;

namespace Soup.Build.Api.Client
{
	[UnconditionalSuppressMessage("ReflectionAnalysis", "IL2026", Justification = "Only Serialize Types within our project.")]
	public partial class LanguageClient
	{
		private string _baseUrl = "https://api.soupbuild.com";
		private HttpClient _httpClient;
		private Lazy<JsonSerializerOptions> _settings;

		public LanguageClient(HttpClient httpClient)
		{
			_httpClient = httpClient;
			_settings = new Lazy<JsonSerializerOptions>(CreateSerializerSettings);
		}

		private JsonSerializerOptions CreateSerializerSettings()
		{
			var settings = new JsonSerializerOptions();
			UpdateJsonSerializerOptions(settings);
			return settings;
		}

		public string BaseUrl
		{
			get { return _baseUrl; }
			set { _baseUrl = value; }
		}

		protected JsonSerializerOptions JsonSerializerOptions { get { return _settings.Value; } }

		partial void UpdateJsonSerializerOptions(JsonSerializerOptions settings);


		partial void PrepareRequest(HttpClient client, HttpRequestMessage request, string url);
		partial void PrepareRequest(HttpClient client, HttpRequestMessage request, StringBuilder urlBuilder);
		partial void ProcessResponse(HttpClient client, HttpResponseMessage response);

		/// <summary>Get a language by unique name.</summary>
		/// <param name="languageName">The unique name of the language.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public Task<LanguageModel> GetLanguageAsync(string languageName)
		{
			return GetLanguageAsync(languageName, CancellationToken.None);
		}

		/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
		/// <summary>Get a language by unique name.</summary>
		/// <param name="languageName">The unique name of the language.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public async Task<LanguageModel> GetLanguageAsync(string languageName, CancellationToken cancellationToken)
		{
			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/language/{languageName}");
			urlBuilder_.Replace("{languageName}", Uri.EscapeDataString(ConvertToString(languageName, CultureInfo.InvariantCulture)));

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
				var responseText = await response.Content.ReadAsStringAsync().ConfigureAwait(false);
				try
				{
					var typedBody = JsonSerializer.Deserialize<T>(responseText, JsonSerializerOptions);
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
						var typedBody = JsonSerializer.Deserialize<T>(responseStream, JsonSerializerOptions);
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

		private string ConvertToString(string value, CultureInfo cultureInfo)
		{
			return value;
		}
	}

	[UnconditionalSuppressMessage("ReflectionAnalysis", "IL2026", Justification = "Only Serialize Types within our project.")]
	public partial class PackageClient
	{
		private string _baseUrl = "https://api.soupbuild.com";
		private HttpClient _httpClient;
		private Lazy<JsonSerializerOptions> _settings;

		public PackageClient(HttpClient httpClient)
		{
			_httpClient = httpClient;
			_settings = new Lazy<JsonSerializerOptions>(CreateSerializerSettings);
		}

		private JsonSerializerOptions CreateSerializerSettings()
		{
			var settings = new JsonSerializerOptions();
			UpdateJsonSerializerOptions(settings);
			return settings;
		}

		public string BaseUrl
		{
			get { return _baseUrl; }
			set { _baseUrl = value; }
		}

		protected JsonSerializerOptions JsonSerializerOptions { get { return _settings.Value; } }

		partial void UpdateJsonSerializerOptions(JsonSerializerOptions settings);


		partial void PrepareRequest(HttpClient client, HttpRequestMessage request, string url);
		partial void PrepareRequest(HttpClient client, HttpRequestMessage request, StringBuilder urlBuilder);
		partial void ProcessResponse(HttpClient client, HttpResponseMessage response);

		/// <summary>Get a package by unique name.</summary>
		/// <param name="languageName">The name of the language.</param>
		/// <param name="packageName">The unique name of the package.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public Task<PackageModel> GetPackageAsync(string languageName, string packageName)
		{
			return GetPackageAsync(languageName, packageName, CancellationToken.None);
		}

		/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
		/// <summary>Get a package by unique name.</summary>
		/// <param name="languageName">The name of the language.</param>
		/// <param name="packageName">The unique name of the package.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public async Task<PackageModel> GetPackageAsync(string languageName, string packageName, CancellationToken cancellationToken)
		{
			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/language/{languageName}/package/{packageName}");
			urlBuilder_.Replace("{languageName}", Uri.EscapeDataString(ConvertToString(languageName, CultureInfo.InvariantCulture)));
			urlBuilder_.Replace("{packageName}", Uri.EscapeDataString(ConvertToString(packageName, CultureInfo.InvariantCulture)));

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
							var objectResponse_ = await ReadObjectResponseAsync<PackageModel>(response_, headers_, cancellationToken).ConfigureAwait(false);
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

		/// <summary>Create or update a package identified by the unique name.</summary>
		/// <param name="languageName">The language name.</param>
		/// <param name="packageName">The package name.</param>
		/// <param name="model">The incoming model.</param>
		/// <returns>Updated.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public Task<PackageModel> CreateOrUpdatePackageAsync(string languageName, string packageName, PackageCreateOrUpdateModel model)
		{
			return CreateOrUpdatePackageAsync(languageName, packageName, model, CancellationToken.None);
		}

		/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
		/// <summary>Create or update a package identified by the unique name.</summary>
		/// <param name="languageName">The language name.</param>
		/// <param name="packageName">The package name.</param>
		/// <param name="model">The incoming model.</param>
		/// <returns>Updated.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public async Task<PackageModel> CreateOrUpdatePackageAsync(string languageName, string packageName, PackageCreateOrUpdateModel model, CancellationToken cancellationToken)
		{
			if (model == null)
				throw new System.ArgumentNullException("model");

			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/language/{languageName}/package/{packageName}");
			urlBuilder_.Replace("{languageName}", Uri.EscapeDataString(ConvertToString(languageName, CultureInfo.InvariantCulture)));
			urlBuilder_.Replace("{packageName}", Uri.EscapeDataString(ConvertToString(packageName, CultureInfo.InvariantCulture)));

			var client_ = _httpClient;
			var disposeClient_ = false;
			try
			{
				using (var request_ = await CreateHttpRequestMessageAsync(cancellationToken).ConfigureAwait(false))
				{
					var content_ = new StringContent(JsonSerializer.Serialize(model, _settings.Value));
					content_.Headers.ContentType = MediaTypeHeaderValue.Parse("application/json");
					request_.Content = content_;
					request_.Method = new HttpMethod("PUT");
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
							var objectResponse_ = await ReadObjectResponseAsync<PackageModel>(response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException("Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}
							return objectResponse_.Object;
						}
						else
						if (status_ == 201)
						{
							var objectResponse_ = await ReadObjectResponseAsync<PackageModel>(response_, headers_, cancellationToken).ConfigureAwait(false);
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
							throw new ApiException<ProblemDetails>("Invalid request.", status_, objectResponse_.Text, headers_, objectResponse_.Object, null);
						}
						else
						if (status_ == 404)
						{
							var objectResponse_ = await ReadObjectResponseAsync<ProblemDetails>(response_, headers_, cancellationToken).ConfigureAwait(false);
							if (objectResponse_.Object == null)
							{
								throw new ApiException("Response was null which was not expected.", status_, objectResponse_.Text, headers_, null);
							}
							throw new ApiException<ProblemDetails>("The language does not exist.", status_, objectResponse_.Text, headers_, objectResponse_.Object, null);
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

		protected virtual async Task<ObjectResponseResult<T>> ReadObjectResponseAsync<T>(HttpResponseMessage response, IReadOnlyDictionary<string, IEnumerable<string>> headers, CancellationToken cancellationToken)
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
					var typedBody = JsonSerializer.Deserialize<T>(responseText, JsonSerializerOptions);
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
						var typedBody = JsonSerializer.Deserialize<T>(responseStream, JsonSerializerOptions);
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

		private string ConvertToString(string value, CultureInfo cultureInfo)
		{
			return value;
		}
	}

	[UnconditionalSuppressMessage("ReflectionAnalysis", "IL2026", Justification = "Only Serialize Types within our project.")]
	public partial class PackageVersionClient
	{
		private string _baseUrl = "https://api.soupbuild.com";
		private HttpClient _httpClient;
		private Lazy<JsonSerializerOptions> _settings;

		public PackageVersionClient(HttpClient httpClient)
		{
			_httpClient = httpClient;
			_settings = new Lazy<JsonSerializerOptions>(CreateSerializerSettings);
		}

		private JsonSerializerOptions CreateSerializerSettings()
		{
			var settings = new JsonSerializerOptions();
			UpdateJsonSerializerOptions(settings);
			return settings;
		}

		public string BaseUrl
		{
			get { return _baseUrl; }
			set { _baseUrl = value; }
		}

		protected JsonSerializerOptions JsonSerializerOptions { get { return _settings.Value; } }

		partial void UpdateJsonSerializerOptions(JsonSerializerOptions settings);


		partial void PrepareRequest(HttpClient client, HttpRequestMessage request, string url);
		partial void PrepareRequest(HttpClient client, HttpRequestMessage request, StringBuilder urlBuilder);
		partial void ProcessResponse(HttpClient client, HttpResponseMessage response);
		/// <summary>Get a package version.</summary>
		/// <param name="languageName">The name of the language.</param>
		/// <param name="packageName">The unique name of the package.</param>
		/// <param name="packageVersion">The package version to get.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public Task<PackageVersionModel> GetPackageVersionAsync(string languageName, string packageName, string packageVersion)
		{
			return GetPackageVersionAsync(languageName, packageName, packageVersion, CancellationToken.None);
		}

		/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
		/// <summary>Get a package version.</summary>
		/// <param name="languageName">The name of the language.</param>
		/// <param name="packageName">The unique name of the package.</param>
		/// <param name="packageVersion">The package version to get.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public async Task<PackageVersionModel> GetPackageVersionAsync(string languageName, string packageName, string packageVersion, CancellationToken cancellationToken)
		{
			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/language/{languageName}/package/{packageName}/version/{packageVersion}");
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

		/// <summary>Publish a new version of a package.</summary>
		/// <param name="languageName">The name of the language.</param>
		/// <param name="packageName">The unique name of the package.</param>
		/// <param name="packageVersion">The package version to publish.</param>
		/// <param name="file">The uploaded file.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public Task PublishPackageVersionAsync(string languageName, string packageName, string packageVersion, FileParameter file)
		{
			return PublishPackageVersionAsync(languageName, packageName, packageVersion, file, CancellationToken.None);
		}

		/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
		/// <summary>Publish a new version of a package.</summary>
		/// <param name="languageName">The name of the language.</param>
		/// <param name="packageName">The unique name of the package.</param>
		/// <param name="packageVersion">The package version to publish.</param>
		/// <param name="file">The uploaded file.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public async Task PublishPackageVersionAsync(string languageName, string packageName, string packageVersion, FileParameter file, CancellationToken cancellationToken)
		{
			if (file == null)
				throw new System.ArgumentNullException("file");

			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/language/{languageName}/package/{packageName}/version/{packageVersion}");
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

		/// <summary>Download a package version archive.</summary>
		/// <param name="languageName">The name of the language.</param>
		/// <param name="packageName">The unique name of the package.</param>
		/// <param name="packageVersion">The package version to download.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public Task<FileResponse> DownloadPackageVersionAsync(string languageName, string packageName, string packageVersion)
		{
			return DownloadPackageVersionAsync(languageName, packageName, packageVersion, CancellationToken.None);
		}

		/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
		/// <summary>Download a package version archive.</summary>
		/// <param name="languageName">The name of the language.</param>
		/// <param name="packageName">The unique name of the package.</param>
		/// <param name="packageVersion">The package version to download.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public async Task<FileResponse> DownloadPackageVersionAsync(string languageName, string packageName, string packageVersion, CancellationToken cancellationToken)
		{
			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/language/{languageName}/package/{packageName}/version/{packageVersion}/download");
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
					request_.Headers.Accept.Add(MediaTypeWithQualityHeaderValue.Parse("application/zip"));

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
							var responseStream_ = response_.Content == null ? Stream.Null : await response_.Content.ReadAsStreamAsync().ConfigureAwait(false);
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
				var responseText = await response.Content.ReadAsStringAsync().ConfigureAwait(false);
				try
				{
					var typedBody = JsonSerializer.Deserialize<T>(responseText, JsonSerializerOptions);
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
						var typedBody = JsonSerializer.Deserialize<T>(responseStream, JsonSerializerOptions);
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

		private string ConvertToString(string value, CultureInfo cultureInfo)
		{
			return value;
		}
	}

	[UnconditionalSuppressMessage("ReflectionAnalysis", "IL2026", Justification = "Only Serialize Types within our project.")]
	public partial class SearchClient
	{
		private string _baseUrl = "https://api.soupbuild.com";
		private HttpClient _httpClient;
		private Lazy<JsonSerializerOptions> _settings;

		public SearchClient(HttpClient httpClient)
		{
			_httpClient = httpClient;
			_settings = new Lazy<JsonSerializerOptions>(CreateSerializerSettings);
		}

		private JsonSerializerOptions CreateSerializerSettings()
		{
			var settings = new JsonSerializerOptions();
			UpdateJsonSerializerOptions(settings);
			return settings;
		}

		public string BaseUrl
		{
			get { return _baseUrl; }
			set { _baseUrl = value; }
		}

		protected JsonSerializerOptions JsonSerializerOptions { get { return _settings.Value; } }

		partial void UpdateJsonSerializerOptions(JsonSerializerOptions settings);


		partial void PrepareRequest(HttpClient client, HttpRequestMessage request, string url);
		partial void PrepareRequest(HttpClient client, HttpRequestMessage request, StringBuilder urlBuilder);
		partial void ProcessResponse(HttpClient client, HttpResponseMessage response);

		/// <summary>Search for packages.</summary>
		/// <param name="q">The query search parameter.</param>
		/// <param name="skip">The query skip parameter.</param>
		/// <param name="take">The query take parameter.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public Task<SearchPackagesModel> SearchPackagesAsync(string q, int? skip, int? take)
		{
			return SearchPackagesAsync(q, skip, take, CancellationToken.None);
		}

		/// <param name="cancellationToken">A cancellation token that can be used by other objects or threads to receive notice of cancellation.</param>
		/// <summary>Search for packages.</summary>
		/// <param name="q">The query search parameter.</param>
		/// <param name="skip">The query skip parameter.</param>
		/// <param name="take">The query take parameter.</param>
		/// <returns>The action result.</returns>
		/// <exception cref="ApiException">A server side error occurred.</exception>
		public async Task<SearchPackagesModel> SearchPackagesAsync(string q, int? skip, int? take, CancellationToken cancellationToken)
		{
			var urlBuilder_ = new StringBuilder();
			urlBuilder_.Append(BaseUrl != null ? BaseUrl.TrimEnd('/') : "").Append("/v1/search/packages?");
			if (q != null)
			{
				urlBuilder_.Append(Uri.EscapeDataString("q") + "=").Append(Uri.EscapeDataString(ConvertToString(q, CultureInfo.InvariantCulture))).Append("&");
			}
			if (skip != null)
			{
				urlBuilder_.Append(Uri.EscapeDataString("skip") + "=").Append(Uri.EscapeDataString(ConvertToString(skip, CultureInfo.InvariantCulture))).Append("&");
			}
			if (take != null)
			{
				urlBuilder_.Append(Uri.EscapeDataString("take") + "=").Append(Uri.EscapeDataString(ConvertToString(take, CultureInfo.InvariantCulture))).Append("&");
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
							var objectResponse_ = await ReadObjectResponseAsync<SearchPackagesModel>(response_, headers_, cancellationToken).ConfigureAwait(false);
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
				var responseText = await response.Content.ReadAsStringAsync().ConfigureAwait(false);
				try
				{
					var typedBody = JsonSerializer.Deserialize<T>(responseText, JsonSerializerOptions);
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
						var typedBody = JsonSerializer.Deserialize<T>(responseStream, JsonSerializerOptions);
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

		private string ConvertToString(string value, CultureInfo cultureInfo)
		{
			return value;
		}

		private string ConvertToString(int? value, CultureInfo cultureInfo)
		{
			return value?.ToString(cultureInfo);
		}
	}

	/// <summary>A class representing the package result.</summary>
	public partial class LanguageModel
	{
		/// <summary>Gets or sets the name.</summary>
		public string Name { get; set; }

		/// <summary>Gets or sets the description.</summary>
		public string Description { get; set; }
	}

	public partial class ProblemDetails
	{
		public string Type { get; set; }

		public string Title { get; set; }

		public int? Status { get; set; }

		public string Detail { get; set; }

		public string Instance { get; set; }

		private IDictionary<string, object> _additionalProperties = new Dictionary<string, object>();

		public IDictionary<string, object> AdditionalProperties
		{
			get { return _additionalProperties; }
			set { _additionalProperties = value; }
		}
	}

	/// <summary>A class representing the package result.</summary>
	public partial class PackageModel
	{
		/// <summary>Gets or sets the name.</summary>
		public string Name { get; set; }

		/// <summary>Gets or sets the description.</summary>
		public string Description { get; set; }

		/// <summary>Gets or sets the latest published version.</summary>
		public SemanticVersion Latest { get; set; }

		/// <summary>Gets or sets the list of versions.</summary>
		public ICollection<PackageVersionModel> Versions { get; set; }


	}

	/// <summary>The semantic version class.</summary>
	public partial class SemanticVersion
	{
		/// <summary>Gets or sets the version major.</summary>
		public int Major { get; set; }

		/// <summary>Gets or sets the version minor.</summary>
		public int Minor { get; set; }

		/// <summary>Gets or sets the version patch.</summary>
		public int Patch { get; set; }


	}

	/// <summary>Package version model.</summary>
	public partial class PackageVersionModel
	{
		/// <summary>Gets or sets the version.</summary>
		public SemanticVersion Version { get; set; }

		/// <summary>Gets or sets the date published.</summary>
		public System.DateTimeOffset DatePublished { get; set; }

		/// <summary>Gets or sets the total downloads count.</summary>
		public int TotalDownloads { get; set; }


	}

	public partial class PackageCreateOrUpdateModel
	{
		/// <summary>Gets or sets the description.</summary>
		public string Description { get; set; }


	}

	/// <summary>A container for the search result.</summary>
	public partial class SearchPackagesModel
	{
		/// <summary>Gets or sets the total packages that match the query.</summary>
		public int Total { get; set; }

		/// <summary>Gets or sets the start offset for this set of packages.</summary>
		public int Offset { get; set; }

		/// <summary>Gets or sets the list of packages.</summary>
		public ICollection<PackageSummaryModel> Packages { get; set; }
	}

	/// <summary>A container for the package summary result.</summary>
	public partial class PackageSummaryModel
	{
		/// <summary>Gets or sets the name.</summary>
		public string Name { get; set; }

		/// <summary>Gets or sets the description.</summary>
		public string Description { get; set; }


	}

	public partial class FileParameter
	{
		public FileParameter(Stream data)
			: this(data, null, null)
		{
		}

		public FileParameter(Stream data, string fileName)
			: this(data, fileName, null)
		{
		}

		public FileParameter(Stream data, string fileName, string contentType)
		{
			Data = data;
			FileName = fileName;
			ContentType = contentType;
		}

		public Stream Data { get; private set; }

		public string FileName { get; private set; }

		public string ContentType { get; private set; }
	}

	public partial class FileResponse : System.IDisposable
	{
		private System.IDisposable _client;
		private System.IDisposable _response;

		public int StatusCode { get; private set; }

		public IReadOnlyDictionary<string, IEnumerable<string>> Headers { get; private set; }

		public Stream Stream { get; private set; }

		public bool IsPartial
		{
			get { return StatusCode == 206; }
		}

		public FileResponse(int statusCode, IReadOnlyDictionary<string, IEnumerable<string>> headers, Stream stream, System.IDisposable client, System.IDisposable response)
		{
			StatusCode = statusCode;
			Headers = headers;
			Stream = stream;
			_client = client;
			_response = response;
		}

		public void Dispose()
		{
			Stream.Dispose();
			if (_response != null)
				_response.Dispose();
			if (_client != null)
				_client.Dispose();
		}
	}

	public partial class ApiException : System.Exception
	{
		public int StatusCode { get; private set; }

		public string Response { get; private set; }

		public IReadOnlyDictionary<string, IEnumerable<string>> Headers { get; private set; }

		public ApiException(string message, int statusCode, string response, IReadOnlyDictionary<string, IEnumerable<string>> headers, System.Exception innerException)
			: base(message + "\n\nStatus: " + statusCode + "\nResponse: \n" + ((response == null) ? "(null)" : response.Substring(0, response.Length >= 512 ? 512 : response.Length)), innerException)
		{
			StatusCode = statusCode;
			Response = response;
			Headers = headers;
		}

		public override string ToString()
		{
			return string.Format("HTTP Response: \n\n{0}\n\n{1}", Response, base.ToString());
		}
	}

	public partial class ApiException<TResult> : ApiException
	{
		public TResult Result { get; private set; }

		public ApiException(string message, int statusCode, string response, IReadOnlyDictionary<string, IEnumerable<string>> headers, TResult result, System.Exception innerException)
			: base(message, statusCode, response, headers, innerException)
		{
			Result = result;
		}
	}

}

#pragma warning restore 1591
#pragma warning restore 1573
#pragma warning restore 472
#pragma warning restore 114
#pragma warning restore 108
#pragma warning restore 3016