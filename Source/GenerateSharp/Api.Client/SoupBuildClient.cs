// <copyright file="SoupBuildClient.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Api.Client
{
    using System.Net.Http;
    using System.Net.Http.Headers;
    using System.Threading;
    using System.Threading.Tasks;

    public partial class LanguageClient
    {
        public string BearerToken { get; set; }

        /// <summary>
        /// Called by implementing swagger client classes
        /// </summary>
        protected Task<HttpRequestMessage> CreateHttpRequestMessageAsync(CancellationToken cancellationToken)
        {
            var request = new HttpRequestMessage();
            if (!string.IsNullOrEmpty(BearerToken))
            {
                request.Headers.Authorization = new AuthenticationHeaderValue("Bearer", BearerToken);
            }

            return Task.FromResult(request);
        }
    }

    public partial class PackageClient
    {
        public string BearerToken { get; set; }

        /// <summary>
        /// Called by implementing swagger client classes
        /// </summary>
        protected Task<HttpRequestMessage> CreateHttpRequestMessageAsync(CancellationToken cancellationToken)
        {
            var request = new HttpRequestMessage();
            if (!string.IsNullOrEmpty(BearerToken))
            {
                request.Headers.Authorization = new AuthenticationHeaderValue("Bearer", BearerToken);
            }

            return Task.FromResult(request);
        }
    }

    public partial class PackageVersionClient
    {
        public string BearerToken { get; set; }

        /// <summary>
        /// Called by implementing swagger client classes
        /// </summary>
        protected Task<HttpRequestMessage> CreateHttpRequestMessageAsync(CancellationToken cancellationToken)
        {
            var request = new HttpRequestMessage();
            if (!string.IsNullOrEmpty(BearerToken))
            {
                request.Headers.Authorization = new AuthenticationHeaderValue("Bearer", BearerToken);
            }

            return Task.FromResult(request);
        }
    }

    public partial class SearchClient
    {
        public string BearerToken { get; set; }

        /// <summary>
        /// Called by implementing swagger client classes
        /// </summary>
        protected Task<HttpRequestMessage> CreateHttpRequestMessageAsync(CancellationToken cancellationToken)
        {
            var request = new HttpRequestMessage();
            if (!string.IsNullOrEmpty(BearerToken))
            {
                request.Headers.Authorization = new AuthenticationHeaderValue("Bearer", BearerToken);
            }

            return Task.FromResult(request);
        }
    }
}
