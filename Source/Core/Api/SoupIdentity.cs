// <copyright file="SoupIdentity.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api
{
    using System.Threading.Tasks;
    using IdentityModel.Client;

    /// <summary>
    /// Represents a collection of functions to interact with the Indentity endpoints
    /// </summary>
    public class SoupIdentity : ISoupIdentity
    {
        /// <summary>
        /// Authenticate user
        /// </summary>
        public async Task<string> AuthenticateUserAsync()
        {
            // Discover endpoints from metadata
            var discoveryResponse = await DiscoveryClient.GetAsync(Constants.SoupIdentityUrl);
            if (discoveryResponse.IsError)
            {
                Log.Error(discoveryResponse.Error);
                throw discoveryResponse.Exception;
            }

            // Request the token
            var tokenClient = new TokenClient(discoveryResponse.TokenEndpoint, "client", "secret");
            var tokenResponse = await tokenClient.RequestClientCredentialsAsync("api1");

            if (tokenResponse.IsError)
            {
                Log.Error(tokenResponse.Error);
                throw tokenResponse.Exception;
            }

            return tokenResponse.Json.ToString();
        }
    }
}
