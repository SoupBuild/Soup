// <copyright file="PublishCommand.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Client
{
    using System.IO;
    using System.Net.Http;
    using System.Threading.Tasks;
    using Soup.Api;

    /// <summary>
    /// Publish Command
    /// </summary>
    internal class PublishCommand
    {
        private ISoupIdentity _soupIdentity;
        private ISoupApi _soupApi;

        /// <summary>
        /// Initializes a new instance of the <see cref="PublishCommand"/> class.
        /// </summary>
        public PublishCommand(
            ISoupIdentity soupIdentity,
            ISoupApi soupApi)
        {
            _soupIdentity = soupIdentity;
            _soupApi = soupApi;
        }

        /// <summary>
        /// Invoke
        /// </summary>
        public async Task InvokeAsync(PublishOptions options)
        {
            var projectDirectory = Directory.GetCurrentDirectory();
            var recipe = await RecipeManager.LoadFromFileAsync(projectDirectory);
            Log.Info($"Publish Project: {recipe.Name}@{recipe.Version}");

            var result = await _soupIdentity.AuthenticateUserAsync();

            using (var stream = new MemoryStream())
            {
                // Pack the project into a memory stream
                await PackageManager.PackAsync(recipe, projectDirectory, stream);

                // Reset the stream so we can read from it
                stream.Seek(0, SeekOrigin.Begin);

                // Publish the package to the service
                try
                {
                    bool created = await _soupApi.PublishPackageAsync(recipe.Name, stream);
                    if (!created)
                    {
                        Log.Warning("The package version already existed! No change was made.");
                    }
                }
                catch (HttpRequestException ex)
                {
                    Log.Error($"Failed request: {ex.ToString()}");
                }
            }
        }
    }
}
