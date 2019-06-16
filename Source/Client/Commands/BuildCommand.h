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
        BuildCommand(BuildOptions options, std::shared_ptr<ICompiler> compiler) :
            _options(std::move(options)),
            _compiler(std::move(compiler))
        {
        }

        /// <summary>
        /// Main entry point for a unique command
        /// </summary>
        virtual void Run() override final
        {
            Log::Trace("BuildCommand::Run");
            auto workingDirectory = Path::GetCurrentDirectory();
            auto recipePath = 
                workingDirectory +
                Path(Constants::RecipeFileName);
            Recipe recipe = {};
            if (!RecipeExtensions::TryLoadFromFile(recipePath, recipe))
            {
                Log::Error("Could not find the recipe file.");
                return;
            }

            // // Ensure the library directory exists
            // var libraryPath = PackageManager.BuildKitchenLibraryPath(_config);
            // if (!Directory.Exists(libraryPath))
            // {
            //     Directory.CreateDirectory(libraryPath);
            // }

            // Now build the current project
            Log::Info("");
            Log::Info("Building Project");
            auto buildPath =
                Path(Constants::ProjectGenerateFolderName) +
                Path(Constants::StoreBuildFolderName);
            auto buildEngine = BuildEngine(_compiler);

            auto buildArguments = BuildArguments();
            buildArguments.Target = BuildTargetType::Executable;
            buildArguments.WorkingDirectory = workingDirectory;
            buildArguments.ObjectDirectory = Path("obj");
            buildArguments.BinaryDirectory = Path("bin");
            buildArguments.SourceFiles = std::vector<Path>({});
            buildArguments.IncludeDirectories = std::vector<Path>({});
            buildArguments.IncludeModules = std::vector<Path>({});
            buildArguments.IsIncremental = false;

            buildEngine.Execute(buildArguments);
        }

    private:
        BuildOptions _options;
        std::shared_ptr<ICompiler> _compiler;
    };
}
