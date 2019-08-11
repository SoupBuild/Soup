// <copyright file="RecipeBuildGenerator.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
    /// <summary>
    /// The recipe build generator that knows how to generate the source
    /// required to compile a specified Recipe.
    /// </summary>
    export class RecipeBuildGenerator
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="RecipeBuildGenerator"/> class.
        /// </summary>
        RecipeBuildGenerator()
        {
        }

        /// <summary>
        /// The Core Execute task
        /// </summary>
        void Execute(const Path& workingDirectory, const Recipe& recipe)
        {
            auto generateBuildPath = 
                workingDirectory +
                Path(Constants::ProjectGenerateFolderName) +
                Path(Constants::ProjectGenerateBuildFolderName);

            // Ensure the build directories exists
            if (!IFileSystem::Current().Exists(generateBuildPath))
            {
                Log::Verbose("Create Directory: " + generateBuildPath.ToString());
                IFileSystem::Current().CreateDirectory(generateBuildPath);
            }
        }

    private:
        
    };
}
