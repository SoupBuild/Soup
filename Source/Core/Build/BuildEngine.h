// <copyright file="BuildEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IBuildEngine.h"
#include "ICompiler.h"
#include "BuildStateChecker.h"
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
            // Log the incoming request for verbose logs
            Log::Verbose("Target = " + ToString(arguments.Target));
            Log::Verbose("WorkingDirectory = " + arguments.WorkingDirectory.ToString());
            Log::Verbose("ObjectDirectory = " + arguments.ObjectDirectory.ToString());
            Log::Verbose("BinaryDirectory = " + arguments.BinaryDirectory.ToString());
            Log::Verbose("ModuleSourceFile = " + arguments.ModuleSourceFile.ToString());
            // Log::Verbose("SourceFiles = " + arguments.SourceFiles.ToString());
            // Log::Verbose("IncludeDirectories = " + arguments.IncludeDirectories.ToString());
            // Log::Verbose("IncludeModules = " + arguments.IncludeModules.ToString());
            Log::Verbose("IsIncremental = " + ToString(arguments.IsIncremental));

            // Perform the core compilation of the source files
            bool sourceCompiled = CompileSource(arguments);

            if (sourceCompiled)
            {
                // Link the final target
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
            }
        }

    private:
        /// <summary>
        /// Compile the supporting source files
        /// Returns true if any files were compiled
        /// </summary>
        bool CompileSource(const BuildArguments& arguments)
        {
            Log::Info("Task: CoreCompile");

            // Load the previous build state if performing an incremental build
            BuildState buildState = {};
            auto source = std::vector<Path>();
            if (arguments.IsIncremental)
            {
                Log::Verbose("Loading previous build state.");
                if (!BuildStateManager::TryLoadState(arguments.WorkingDirectory, buildState))
                {
                    Log::Verbose("No previous state found, full rebuild required.");
                    buildState = BuildState();
                    source = arguments.SourceFiles;
                }
                else
                {
                    // Check if each source file is out of date and requires a rebuild
                    Log::Verbose("Check for updated source.");
                    for (auto& sourceFile : arguments.SourceFiles)
                    {
                        // Try to build up the closure of include dependencies
                        auto inputClosure = std::vector<Path>();
                        if (buildState.TryBuildIncludeClosure(sourceFile, inputClosure))
                        {
                            // Build the expected object file
                            auto outputFile = arguments.ObjectDirectory + Path(sourceFile.GetFileStem() + _compiler->GetObjectFileExtension());

                            // Check if any of the input files have changed since lsat build
                            if (BuildStateChecker::IsOutdated(
                                outputFile,
                                inputClosure,
                                arguments.WorkingDirectory))
                            {
                                // The file or a dependecy has changed
                                source.push_back(sourceFile);
                            }
                        }
                        else
                        {
                            // Could not determine the set of input files, not enough info to perform incremental build
                            source.push_back(sourceFile);
                        }
                    }
                }
            }
            else
            {
                // The build is forced, rebuild all input
                source = arguments.SourceFiles;
            }

            // Check if we can skip the whole thing
            if (!source.empty())
            {
                Log::Verbose("Compiling source files.");

                // // Ensure the object directory exists
                // var objectDirectry = Path.Combine(args.RootDirectory, objectDirectory);
                // if (!Directory.Exists(objectDirectry))
                // {
                //     Directory.CreateDirectory(objectDirectry);
                // }

                // Setup the shared properties
                auto compilerArguments = CompileArguments();
                compilerArguments.Standard = LanguageStandard::CPP20;
                compilerArguments.RootDirectory = arguments.WorkingDirectory;
                compilerArguments.OutputDirectory = arguments.ObjectDirectory;
                compilerArguments.PreprocessorDefinitions = {};
                compilerArguments.IncludeDirectories = arguments.IncludeDirectories;
                compilerArguments.IncludeModules = arguments.IncludeModules;
                compilerArguments.GenerateIncludeTree = true;

                for (auto& file : source)
                {
                    // Compile the individual translation unit
                    Log::Verbose(file.ToString());
                    compilerArguments.SourceFile = file;
                    auto result = _compiler->Compile(compilerArguments);
                }

                // // Save the build state
                // if (result.HeaderIncludeFiles != null)
                //     buildState.UpdateIncludeTree(result.HeaderIncludeFiles);
            
                // Save the build state
                // BuildStateManager::SaveState(arguments.WorkingDirectory, buildState);

                return true;
            }
            else
            {
                Log::Info("All source is up to date.");
                return false;
            }
        }

        /// <summary>
        /// Link the library
        /// </summary>
        void LinkLibrary(const BuildArguments& arguments)
        {
            Log::Info("Task: LinkStaticLibrary");
        }

        /// <summary>
        /// Link the executable
        /// </summary>
        void LinkExecutable(const BuildArguments& arguments)
        {
            Log::Info("Task: LinkExecutable");
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
