// <copyright file="AuthenticationManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

namespace Soup.Build.PackageManager
{
	using System;
	using System.Linq;
	using System.Text;
	using System.Threading.Tasks;
	using Microsoft.Identity.Client;
	using Newtonsoft.Json.Linq;
	using Opal;

	internal class AuthenticationManager
	{
		private static readonly string Tenant = $"soupbuild.com";
		private static readonly string AzureAdB2CHostname = $"login.soupbuild.com";
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
				// Attempt to silently aquire the user token
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

		private static void DisplayUserInfo(AuthenticationResult authResult)
		{
			if (authResult != null)
			{
				JObject user = ParseIdToken(authResult.IdToken);

				Log.Diag("");
				Log.Diag($"Name: {user["name"]?.ToString()}");
				Log.Diag($"User Identifier: {user["oid"]?.ToString()}");
				Log.Diag($"Street Address: {user["streetAddress"]?.ToString()}");
				Log.Diag($"City: {user["city"]?.ToString()}");
				Log.Diag($"State: {user["state"]?.ToString()}");
				Log.Diag($"Country: {user["country"]?.ToString()}");
				Log.Diag($"Job Title: {user["jobTitle"]?.ToString()}");

				if (user["emails"] is JArray emails)
				{
					Log.Diag($"Emails: {emails[0]}");
				}

				Log.Diag($"Identity Provider: {user["iss"]?.ToString()}");
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
	}

}
