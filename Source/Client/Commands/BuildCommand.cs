// <copyright file="BuildCommand.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Client
{
    using System.IO;
    using System.Threading.Tasks;

    /// <summary>
    /// Build Command
    /// </summary>
    internal class BuildCommand
    {
        private LocalUserConfig _config;
        private Compiler.ICompiler _compiler;

        /// <summary>
        /// Initializes a new instance of the <see cref="BuildCommand"/> class.
        /// </summary>
        public BuildCommand(LocalUserConfig config, Compiler.ICompiler compiler)
        {
            _config = config;
            _compiler = compiler;
        }

        /// <summary>
        /// Invoke
        /// </summary>
        public async Task InvokeAsync(BuildOptions options)
        {
            var recipePath = "./";
            var recipe = await RecipeManager.LoadFromFileAsync(recipePath);
            if (recipe == null)
            {
                Log.Error("Could not find the recipe file.");
                return;
            }

            // Ensure the library directory exists
            var libraryPath = PackageManager.BuildKitchenLibraryPath(_config);
            if (!Directory.Exists(libraryPath))
            {
                Directory.CreateDirectory(libraryPath);
            }

            // Now build the current project
            Log.Info(string.Empty);
            Log.Info("Building Project");
            var buildPath = Path.Combine(Constants.ProjectGenerateFolderName, Constants.StoreBuildFolderName);
            var buildEngine = new BuildEngine(_config, _compiler);
            await buildEngine.ExecuteAsync(recipePath, recipe, options.Force);
        }
    }
}
