// <copyright file="BuildEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IBuildEngine.h"
#include "ICompiler.h"
#include "BuildStateManager.h"

namespace Soup
{
    /// <summary>
    /// The build engine
    /// </summary>
    export class BuildEngine : public IBuildEngine
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="BuildEngine"/> class.
        /// </summary>
        BuildEngine(std::shared_ptr<ICompiler> compiler) :
            _compiler(std::move(compiler))
        {
            if (_compiler == nullptr)
                throw std::runtime_error("Argument Null: compiler");
        }

        /// <summary>
        /// The Core build task
        /// </summary>
        virtual void Execute(const BuildArguments& arguments) override final
        {
            // Load the previous build state if performing an incremental build
            BuildState buildState = {};
            if (arguments.IsIncremental)
            {
                Log::Verbose("Loading previous build state.");
                if (!BuildStateManager::TryLoadState(arguments.WorkingDirectory, buildState))
                {
                    Log::Verbose("No previous state found.");
                    buildState = BuildState();
                }
            }

            CompileSource(arguments);

            switch (arguments.Target)
            {
                case BuildTargetType::Library:
                    LinkLibrary(arguments);
                    break;
                case BuildTargetType::Executable:
                    LinkExecutable(arguments);
                    break;
                default:
                    throw std::runtime_error("Unknown target type.");
            }

            // Save the build state
            BuildStateManager::SaveState(arguments.WorkingDirectory, buildState);
        }

    private:
        /// <summary>
        /// Compile the supporting source files
        /// </summary>
        void CompileSource(const BuildArguments& arguments)
        {
            // // Check if each source file is out of date and requires a rebuild
            // for (var sourceFile in recipe.Source)
            // {
            //     var outputFile = Path.Combine(objectDirectory, $"{Path.GetFileNameWithoutExtension(sourceFile)}.{_compiler.ObjectFileExtension}");
            //     if (force || BuildRequiredChecker.IsSourceFileOutdated(path, buildState, outputFile, sourceFile, sharedDependecies))
            //     {
            //         source.Add(sourceFile);
            //     }
            // }

            // if (source.Count == 0)
            // {
            //     Log.Info("All source is up to date.");
            //     return false;
            // }
            // else
            // {
                Log::Info("Compile Source");
                auto compilerArguments = CompileArguments();
                compilerArguments.Standard = LanguageStandard::CPP20;
                compilerArguments.RootDirectory = arguments.WorkingDirectory;
                compilerArguments.OutputDirectory = arguments.ObjectDirectory;
                compilerArguments.PreprocessorDefinitions = {};
                compilerArguments.SourceFile = Path("");
                compilerArguments.IncludeDirectories = arguments.IncludeDirectories;
                compilerArguments.IncludeModules = arguments.IncludeModules;
                compilerArguments.GenerateIncludeTree = true;

            //     // Ensure the object directory exists
            //     var objectDirectry = Path.Combine(args.RootDirectory, objectDirectory);
            //     if (!Directory.Exists(objectDirectry))
            //     {
            //         Directory.CreateDirectory(objectDirectry);
            //     }

                // Compile the required filess
                auto result = _compiler->Compile(compilerArguments);

            //     // Save the build state
            //     if (result.HeaderIncludeFiles != null)
            //         buildState.UpdateIncludeTree(result.HeaderIncludeFiles);
            // }
        }

        /// <summary>
        /// Link the library
        /// </summary>
        void LinkLibrary(const BuildArguments& arguments)
        {
            Log::Info("Link Library");
        }

        /// <summary>
        /// Link the executable
        /// </summary>
        void LinkExecutable(const BuildArguments& arguments)
        {
            Log::Info("Link Executable");
        //     var allFiles = new List<string>(recipe.Source);
        //     if (recipe.Type == RecipeType.Library)
        //     {
        //         allFiles.Add(recipe.Public);
        //     }

        //     // Add all of the dependencies as module references
        //     var librarySet = new HashSet<string>();
        //     await GenerateDependencyLibrarySetAsync(path, binaryDirectory, recipe, librarySet);

        //     var objectFiles = recipe.Source.Select(file => $"{objectDirectory.EnsureTrailingSlash()}{Path.GetFileNameWithoutExtension(file)}.{_compiler.ObjectFileExtension}").ToList();
        //     var libraryFiles = librarySet.ToList();
        //     var args = new LinkerArguments()
        //     {
        //         Name = recipe.Name,
        //         RootDirectory = path,
        //         OutputDirectory = binaryDirectory,
        //         SourceFiles = objectFiles,
        //         LibraryFiles = libraryFiles,
        //     };

        //     // Ensure the object directory exists
        //     var objectDirectry = Path.Combine(args.RootDirectory, binaryDirectory);
        //     if (!Directory.Exists(objectDirectry))
        //     {
        //         Directory.CreateDirectory(objectDirectry);
        //     }

        //     // Link
        //     await _compiler.LinkExecutableAsync(args);
        }

    private:
        std::shared_ptr<ICompiler> _compiler;
    };
}
