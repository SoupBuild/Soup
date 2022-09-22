// <copyright file="AuthenticationManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

namespace Soup.Build.PackageManager
{
	using System.Linq;
	using System.Threading.Tasks;
	using Microsoft.Identity.Client;
	using Microsoft.Identity.Client.Extensibility;
	using Opal;

	internal class AuthenticationManager
	{
		private static readonly string Tenant = $"soupbuild.com";
		private static readonly string AzureAdB2CHostname = $"soupbuild.b2clogin.com";
		private static readonly string ClientId = "29b9e45c-332b-4f93-a41f-af525dee4730";
		private static readonly string SignUpSignInPolicyId = "B2C_1_SignUp_SignIn";
		private static readonly string SoupApiScope = "/ba178231-c318-435d-881a-25f9e00df20a/soup_build_api";

		private static readonly string Scope = $"https://{Tenant}{SoupApiScope}";

		private static string RedirectUri => $"https://{AzureAdB2CHostname}/oauth2/nativeclient";

		private static string AuthorityBase => $"https://{AzureAdB2CHostname}/tfp/{Tenant}/";

		public static string SignUpSignInAuthority => $"{AuthorityBase}{SignUpSignInPolicyId}";

		/// <summary>
		/// Ensure the user is logged in
		/// </summary>
		/// <returns>The access token</returns>
		public static async Task<string> EnsureSignInAsync()
		{
			IPublicClientApplication publicClientApp = PublicClientApplicationBuilder.Create(ClientId)
				.WithB2CAuthority(SignUpSignInAuthority)
				.WithRedirectUri(RedirectUri)
				.WithLogging(PublicClientLog, LogLevel.Info, false)
				.Build();

			TokenCache.Bind(publicClientApp.UserTokenCache);

			AuthenticationResult authResult = null;
			var scopes = new string[] { Scope };

			try
			{
				// Attempt to silently acquire the user token
				var accounts = await publicClientApp.GetAccountsAsync(SignUpSignInPolicyId);
				authResult = await publicClientApp.AcquireTokenSilent(scopes, accounts.FirstOrDefault())
					.ExecuteAsync();
			}
			catch (MsalUiRequiredException)
			{
				// Ignore, user will need to sign in interactively.
				authResult = await publicClientApp.AcquireTokenInteractive(scopes)
					.ExecuteAsync();
			}

			return authResult.AccessToken;
		}

		private static void PublicClientLog(LogLevel level, string message, bool containsPii)
		{
			string logs = $"{level} {message}";
			Log.Diag(logs);
		}
	}
}
