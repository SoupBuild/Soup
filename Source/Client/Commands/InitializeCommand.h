// <copyright file="InitializeCommand.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ICommand.h"
#include "InitializeOptions.h"

namespace Soup::Client
{
    /// <summary>
    /// Initialize Command
    /// </summary>
    class InitializeCommand : public ICommand
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="InitializeCommand"/> class.
        /// </summary>
        InitializeCommand(InitializeOptions options) :
            m_options(std::move(options))
        {
        }

        /// <summary>
        /// Main entry point for a unique command
        /// </summary>
        virtual void Run() override final
        {
            Log::Trace("InitializeCommand::Run");
            // Log.Info("The initialize utility will walk through the creation of the most basic Console recipe.\n");

            // // Use the current directory as the default names
            // var currentDirectory = new DirectoryInfo(Directory.GetCurrentDirectory());

            // var recipe = new Recipe(
            //     currentDirectory.Name)
            // {
            //     Version = new SemanticVersion(1, 0, 0)
            // };

            // Log.Info($"Name: ({recipe.Name}) ");
            // var newName = Console.ReadLine();
            // if (!string.IsNullOrWhiteSpace(newName))
            // {
            //     recipe.Name = newName;
            // }

            // bool setVersion = false;
            // while (!setVersion)
            // {
            //     Log.Info($"Version: ({recipe.Version}) ");
            //     var newVersion = Console.ReadLine();
            //     if (string.IsNullOrEmpty(newVersion))
            //     {
            //         // Use the default
            //         setVersion = true;
            //     }
            //     else
            //     {
            //         if (SemanticVersion.TryParse(newVersion, out SemanticVersion value))
            //         {
            //             recipe.Version = value;
            //             setVersion = true;
            //         }
            //         else
            //         {
            //             Log.Warning($"Invalid version: \"{newVersion}\"");
            //         }
            //     }
            // }

            // // Save the state of the recipe if it has changed
            // await RecipeManager.SaveToFileAsync(recipe);
        }

    private:
        InitializeOptions m_options;
    };
}
