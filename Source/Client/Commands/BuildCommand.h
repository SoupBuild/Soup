// <copyright file="BuildCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "BuildOptions.h"

namespace Soup::Client
{
    /// <summary>
    /// Build Command
    /// </summary>
    class BuildCommand : public ICommand
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="BuildCommand"/> class.
        /// </summary>
        BuildCommand(BuildOptions options) ://LocalUserConfig config, Compiler.ICompiler compiler)
            m_options(std::move(options))
        {
            //_config = config;
            //_compiler = compiler;
        }

        /// <summary>
        /// Main entry point for a unique command
        /// </summary>
        virtual void Run() override final
        {
            Log::Trace("BuildCommand::Run");
            // var recipePath = Directory.GetCurrentDirectory();
            // var recipe = await RecipeManager.LoadFromFileAsync(recipePath);
            // if (recipe == null)
            // {
            //     Log.Error("Could not find the recipe file.");
            //     return;
            // }

            // // Ensure the library directory exists
            // var libraryPath = PackageManager.BuildKitchenLibraryPath(_config);
            // if (!Directory.Exists(libraryPath))
            // {
            //     Directory.CreateDirectory(libraryPath);
            // }

            // // Now build the current project
            // Log.Info(string.Empty);
            // Log.Info("Building Project");
            // var buildPath = Path.Combine(Constants.ProjectGenerateFolderName, Constants.StoreBuildFolderName);
            // var buildEngine = new BuildEngine(_config, _compiler);
            // await buildEngine.ExecuteAsync(recipePath, recipe, options.Force);
        }

    private:
        BuildOptions m_options;
        //private LocalUserConfig _config;
        //private Compiler.ICompiler _compiler;
    };
}
