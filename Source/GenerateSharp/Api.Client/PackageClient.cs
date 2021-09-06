// <copyright file="PackageClient.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Api.Client
{
    using System.Net.Http;
    using System.Net.Http.Headers;
    using System.Threading;
    using System.Threading.Tasks;

    /// <summary>
    /// The package client.
    /// </summary>
    public partial class PackageClient
    {
        public string BearerToken { get; set; }

        /// <summary>
        /// Called by implementing swagger client classes.
        /// </summary>
        /// <param name="cancellationToken">The cancellation token.</param>
        protected Task<HttpRequestMessage> CreateHttpRequestMessageAsync(CancellationToken cancellationToken)
        {
            var request = new HttpRequestMessage();
            if (!string.IsNullOrEmpty(this.BearerToken))
            {
                request.Headers.Authorization = new AuthenticationHeaderValue("Bearer", this.BearerToken);
            }

            return Task.FromResult(request);
        }
    }
}
