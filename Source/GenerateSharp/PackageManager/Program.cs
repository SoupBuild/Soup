// <copyright file="Program.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.PackageManager
{
    using Microsoft.Identity.Client;
    using Newtonsoft.Json.Linq;
    using Soup.Build.Api.Client;
    using System;
    using System.Linq;
    using System.Net.Http;
    using System.Net.Http.Headers;
    using System.Text;
    using System.Threading.Tasks;

    public class Program
    {
        private static readonly string Tenant = $"soupbuildauth.onmicrosoft.com";
        private static readonly string AzureAdB2CHostname = $"soupbuildauth.b2clogin.com";
        private static readonly string ClientId = "b728c0d6-5b9e-426d-a1c3-52e86916b717";
        private static readonly string SignUpSignInPolicyId = "B2C_1_signup_signin";
        private static readonly string SoupApiScope = "/3919ce83-b7ef-42db-93f5-4d729dd0406c/soup_api";

        private static readonly string Scope = $"https://{Tenant}{SoupApiScope}";

        private static string RedirectUri => $"https://{AzureAdB2CHostname}/oauth2/nativeclient";

        private static string AuthorityBase => $"https://{AzureAdB2CHostname}/tfp/{Tenant}/";

        public static string SignUpSignInAuthority => $"{AuthorityBase}{SignUpSignInPolicyId}";

        public static async Task Main(string[] args)
        {
            Console.WriteLine("Hello World!");

            IPublicClientApplication publicClientApp = PublicClientApplicationBuilder.Create(ClientId)
                .WithB2CAuthority(SignUpSignInAuthority)
                .WithRedirectUri(RedirectUri)
                .WithLogging(Log, LogLevel.Info, false)
                .Build();

            TokenCache.Bind(publicClientApp.UserTokenCache);

            var result = await CallApiAsync(publicClientApp);

            Console.WriteLine(result.Total);
        }

        private static void Log(LogLevel level, string message, bool containsPii)
        {
            string logs = $"{level} {message}";
            Console.WriteLine(logs);
        }

        private static async Task<string> EnsureSignInAsync(IPublicClientApplication publicClientApp)
        {
            AuthenticationResult authResult = null;
            var scopes = new string[] { Scope };

            try
            {
                // Attempt to silently aquire the user token
                var accounts = await publicClientApp.GetAccountsAsync(SignUpSignInPolicyId);
                authResult = await publicClientApp.AcquireTokenSilent(scopes, accounts.FirstOrDefault())
                    .ExecuteAsync();
            }
            catch (MsalUiRequiredException)
            {
                // Ignore, user will need to sign in interactively.
                Console.WriteLine("You need to sign-in first, and then Call API");
                authResult = await publicClientApp.AcquireTokenInteractive(scopes)
                    .ExecuteAsync();
            }

            DisplayUserInfo(authResult);

            return authResult.AccessToken;
        }

        private static void DisplayUserInfo(AuthenticationResult authResult)
        {
            if (authResult != null)
            {
                JObject user = ParseIdToken(authResult.IdToken);

                Console.WriteLine("");
                Console.WriteLine($"Name: {user["name"]?.ToString()}");
                Console.WriteLine($"User Identifier: {user["oid"]?.ToString()}");
                Console.WriteLine($"Street Address: {user["streetAddress"]?.ToString()}");
                Console.WriteLine($"City: {user["city"]?.ToString()}");
                Console.WriteLine($"State: {user["state"]?.ToString()}");
                Console.WriteLine($"Country: {user["country"]?.ToString()}");
                Console.WriteLine($"Job Title: {user["jobTitle"]?.ToString()}");

                if (user["emails"] is JArray emails)
                {
                    Console.WriteLine($"Emails: {emails[0].ToString()}");
                }

                Console.WriteLine($"Identity Provider: {user["iss"]?.ToString()}");
            }
        }

        private static JObject ParseIdToken(string idToken)
        {
            // Parse the idToken to get user info
            idToken = idToken.Split('.')[1];
            idToken = Base64UrlDecode(idToken);
            return JObject.Parse(idToken);
        }

        private static string Base64UrlDecode(string s)
        {
            s = s.Replace('-', '+').Replace('_', '/');
            s = s.PadRight(s.Length + (4 - s.Length % 4) % 4, '=');
            var byteArray = Convert.FromBase64String(s);
            var decoded = Encoding.UTF8.GetString(byteArray, 0, byteArray.Count());
            return decoded;
        }

        private static async Task<PackageSearchModel> CallApiAsync(IPublicClientApplication publicClientApp)
        {
            var accessToken = await EnsureSignInAsync(publicClientApp);

            using (var httpClient = new HttpClient())
            {
                var client = new PackageClient(httpClient)
                {
                    BearerToken = accessToken,
                };

                var response = await client.SearchPackagesAsync("test", null, null);

                return response;
            }
        }
    }
}
