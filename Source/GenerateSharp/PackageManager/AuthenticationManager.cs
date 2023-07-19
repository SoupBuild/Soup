// <copyright file="AuthenticationManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright

namespace Soup.Build.PackageManager
{
	using System.Threading.Tasks;
	using IdentityModel.OidcClient;

	internal class AuthenticationManager : IAuthenticationManager
	{
		static string _authority = "https://auth.soupbuild.com/";
		// static string _authority = "https://localhost:5001/";

		/// <summary>
		/// Ensure the user is logged in
		/// </summary>
		/// <returns>The access token</returns>
		public async Task<string> EnsureSignInAsync()
		{
			var token = await Login();
			return token;
		}

		private async Task<string> Login()
		{
			// Create a redirect URI using an available port on the loopback address
			var browser = new SystemBrowser();
			string redirectUri = string.Format($"http://127.0.0.1:{browser.Port}");

			var options = new OidcClientOptions()
			{
				Authority = _authority,
				ClientId = "Soup.Native",
				RedirectUri = redirectUri,
				Scope = "openid profile soup_api",
				FilterClaims = false,
				Browser = browser,
			};

			var oidcClient = new OidcClient(options);
			var result = await oidcClient.LoginAsync(new LoginRequest());

			return result.AccessToken;
		}
	}
}
