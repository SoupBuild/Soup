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
            Log::Verbose("TargetName = " + arguments.TargetName);
            Log::Verbose("TargetType = " + ToString(arguments.TargetType));
            Log::Verbose("WorkingDirectory = " + arguments.WorkingDirectory.ToString());
            Log::Verbose("ObjectDirectory = " + arguments.ObjectDirectory.ToString());
            Log::Verbose("BinaryDirectory = " + arguments.BinaryDirectory.ToString());
            Log::Verbose("ModuleInterfaceSourceFile = " + arguments.ModuleInterfaceSourceFile.ToString());
            Log::Verbose("IsIncremental = " + ToString(arguments.IsIncremental));
            Log::Verbose("IncludeDirectories = " + ToString(arguments.IncludeDirectories));
            Log::Verbose("IncludeModules = " + ToString(arguments.IncludeModules));
            Log::Verbose("PreprocessorDefinitions = " + ToString(arguments.PreprocessorDefinitions));

            // Perform the core compilation of the source files
            bool sourceCompiled = CoreCompile(arguments);

            // Link the final target
            bool targetLinked = CoreLink(arguments, sourceCompiled);
            if (!targetLinked)
            {
                Log::Info("Up to date");
            }
        }

    private:
        /// <summary>
        /// Compile the module and source files
        /// Returns true if any files were compiled
        /// </summary>
        bool CoreCompile(const BuildArguments& arguments)
        {
            Log::Verbose("Task: CoreCompile");

            // Load the previous build state if performing an incremental build
            BuildState buildState = {};
            bool forceBuild = !arguments.IsIncremental;
            if (arguments.IsIncremental)
            {
                Log::Verbose("Loading previous build state");
                if (!BuildStateManager::TryLoadState(arguments.WorkingDirectory, buildState))
                {
                    Log::Verbose("No previous state found, full rebuild required");
                    buildState = BuildState();
                    forceBuild = true;
                }
            }

            // Ensure the output directories exists
            auto objectDirectry = arguments.WorkingDirectory + arguments.ObjectDirectory;
            if (!IFileSystem::Current().Exists(objectDirectry))
            {
                Log::Verbose("Create Directory: " + arguments.ObjectDirectory.ToString());
                IFileSystem::Current().CreateDirectory(objectDirectry);
            }

            auto binaryDirectry = arguments.WorkingDirectory + arguments.BinaryDirectory;
            if (!IFileSystem::Current().Exists(binaryDirectry))
            {
                Log::Verbose("Create Directory: " + arguments.BinaryDirectory.ToString());
                IFileSystem::Current().CreateDirectory(binaryDirectry);
            }

            // Compile the module interface unit if present
            bool codeCompiled = false;
            if (!arguments.ModuleInterfaceSourceFile.ToString().empty())
            {
                auto moduleCompiled = CompileModuleInterfaceUnit(
                    arguments,
                    buildState,
                    forceBuild);

                // Force recompile the source files if the module was compiled
                forceBuild |= moduleCompiled;
                codeCompiled |= moduleCompiled;
            }

            if (!arguments.SourceFiles.empty())
            {
                auto sourceCompiled = CompileSourceFiles(
                    arguments,
                    buildState,
                    forceBuild);

                // Force link if any of the source was compiled
                codeCompiled |= sourceCompiled;
            }

            if (codeCompiled)
            {
                Log::Verbose("Saving updated build state");
                BuildStateManager::SaveState(arguments.WorkingDirectory, buildState);
            }

            return codeCompiled;
        }

        /// <summary>
        /// Compile the single module interface unit
        /// Returns true if the file was compiled
        /// </summary>
        bool CompileModuleInterfaceUnit(
            const BuildArguments& arguments,
            BuildState& buildState,
            bool forceBuild)
        {
            Log::Verbose("Task: CompileModuleInterfaceUnit");

            bool buildRequired = forceBuild;
            if (!forceBuild)
            {
                // Check if each source file is out of date and requires a rebuild
                Log::Verbose("Check for updated source");
                
                // Try to build up the closure of include dependencies
                auto inputClosure = std::vector<Path>();
                const auto& sourceFile = arguments.ModuleInterfaceSourceFile;
                if (buildState.TryBuildIncludeClosure(sourceFile, inputClosure))
                {
                    // Include the source file itself
                    inputClosure.push_back(sourceFile);

                    // All modules are input dependencies
                    std::copy(
                        arguments.IncludeModules.begin(),
                        arguments.IncludeModules.end(),
                        std::back_inserter(inputClosure));

                    // Build the expected object file
                    auto outputFile = arguments.ObjectDirectory + Path(sourceFile.GetFileName());
                    outputFile.SetFileExtension(_compiler->GetObjectFileExtension());

                    // TODO: Include the module binary interface as output too

                    // Check if any of the input files have changed since lsat build
                    if (BuildStateChecker::IsOutdated(
                        outputFile,
                        inputClosure,
                        arguments.WorkingDirectory))
                    {
                        // The file or a dependecy has changed
                        buildRequired = true;
                    }
                    else
                    {
                        Log::Verbose("File up to date: " + sourceFile.ToString());
                    }
                }
                else
                {
                    // Could not determine the set of input files, not enough info to perform incremental build
                    buildRequired = true;
                }
            }

            // Check if we can skip the whole dang thing
            if (buildRequired)
            {
                // Setup the shared properties
                auto compileArguments = CompileArguments();
                compileArguments.Standard = LanguageStandard::CPP20;
                compileArguments.Optimize = OptimizationLevel::Speed;
                compileArguments.RootDirectory = arguments.WorkingDirectory;
                compileArguments.PreprocessorDefinitions = {};
                compileArguments.IncludeDirectories = arguments.IncludeDirectories;
                compileArguments.IncludeModules = arguments.IncludeModules;
                compileArguments.GenerateIncludeTree = false;
                compileArguments.ExportModule = true;
                compileArguments.PreprocessorDefinitions = arguments.PreprocessorDefinitions;

                // Compile the individual translation unit
                const auto& file = arguments.ModuleInterfaceSourceFile;
                Log::Verbose(file.ToString());
                compileArguments.SourceFile = file;
                compileArguments.TargetFile = arguments.ObjectDirectory + Path(file.GetFileName());
                compileArguments.TargetFile.SetFileExtension(_compiler->GetObjectFileExtension());

                auto result = _compiler->Compile(compileArguments);

                // Copy the binary module interface to the binary directory
                auto objectOutputModuleInterfaceFile = arguments.ObjectDirectory + Path(file.GetFileName());
                objectOutputModuleInterfaceFile.SetFileExtension(_compiler->GetModuleFileExtension());
                auto binaryOutputModuleInterfaceFile = arguments.BinaryDirectory + Path(arguments.TargetName + "." + std::string(_compiler->GetModuleFileExtension()));
                Log::Verbose("Copy: [" + objectOutputModuleInterfaceFile.ToString() + "] -> [" + binaryOutputModuleInterfaceFile.ToString() + "]");
                IFileSystem::Current().CopyFile(
                    arguments.WorkingDirectory + objectOutputModuleInterfaceFile,
                    arguments.WorkingDirectory + binaryOutputModuleInterfaceFile);

                // // Save the build state
                // if (result.HeaderIncludeFiles != null)
                //     buildState.UpdateIncludeTree(result.HeaderIncludeFiles);

                return true;
            }
            else
            {
                Log::Verbose("Module up to date");
                return false;
            }
        }

        /// <summary>
        /// Compile the supporting source files
        /// Returns true if any files were compiled
        /// </summary>
        bool CompileSourceFiles(
            const BuildArguments& arguments,
            BuildState& buildState,
            bool force)
        {
            Log::Verbose("Task: CompileSourceFiles");

            auto source = std::vector<Path>();
            if (!force)
            {
                // Check if each source file is out of date and requires a rebuild
                Log::Verbose("Check for updated source");
                for (auto& sourceFile : arguments.SourceFiles)
                {
                    // Try to build up the closure of include dependencies
                    auto inputClosure = std::vector<Path>();
                    if (buildState.TryBuildIncludeClosure(sourceFile, inputClosure))
                    {
                        // Include the source file itself
                        inputClosure.push_back(sourceFile);

                        // All modules are input dependencies
                        std::copy(
                            arguments.IncludeModules.begin(),
                            arguments.IncludeModules.end(),
                            std::back_inserter(inputClosure));

                        // Build the expected object file
                        auto outputFile = arguments.ObjectDirectory + Path(sourceFile.GetFileName());
                        outputFile.SetFileExtension(_compiler->GetObjectFileExtension());

                        // Check if any of the input files have changed since lsat build
                        if (BuildStateChecker::IsOutdated(
                            outputFile,
                            inputClosure,
                            arguments.WorkingDirectory))
                        {
                            // The file or a dependecy has changed
                            source.push_back(sourceFile);
                        }
                        else
                        {
                            Log::Verbose("File up to date: " + sourceFile.ToString());
                        }
                    }
                    else
                    {
                        // Could not determine the set of input files, not enough info to perform incremental build
                        source.push_back(sourceFile);
                    }
                }
            }
            else
            {
                // The build is forced, rebuild all input
                source = arguments.SourceFiles;
            }

            // Check if we can skip the whole dang thing
            if (!source.empty())
            {
                Log::Verbose("Compiling source files");

                // Setup the shared properties
                auto compileArguments = CompileArguments();
                compileArguments.Standard = LanguageStandard::CPP20;
                compileArguments.Optimize = OptimizationLevel::Speed;
                compileArguments.RootDirectory = arguments.WorkingDirectory;
                compileArguments.PreprocessorDefinitions = {};
                compileArguments.IncludeDirectories = arguments.IncludeDirectories;
                compileArguments.IncludeModules = arguments.IncludeModules;
                compileArguments.GenerateIncludeTree = false;
                compileArguments.ExportModule = false;
                compileArguments.PreprocessorDefinitions = arguments.PreprocessorDefinitions;

                for (auto& file : source)
                {
                    // Compile the individual translation unit
                    Log::Verbose(file.ToString());
                    compileArguments.SourceFile = file;
                    compileArguments.TargetFile = arguments.ObjectDirectory + Path(file.GetFileName());
                    compileArguments.TargetFile.SetFileExtension(_compiler->GetObjectFileExtension());

                    auto result = _compiler->Compile(compileArguments);
                }

                // // Save the build state
                // if (result.HeaderIncludeFiles != null)
                //     buildState.UpdateIncludeTree(result.HeaderIncludeFiles);

                return true;
            }
            else
            {
                Log::Verbose("Objects up to date");
                return false;
            }
        }

        /// <summary>
        /// Link the library
        /// Returns true if the target was linked, otherwise returns false
        /// </summary>
        bool CoreLink(const BuildArguments& arguments, bool force)
        {
            Log::Verbose("Task: CoreLink");

            Path targetFile;
            switch (arguments.TargetType)
            {
                case BuildTargetType::Library:
                    targetFile = 
                        arguments.BinaryDirectory + 
                        Path(arguments.TargetName + "." + std::string(_compiler->GetStaticLibraryFileExtension()));
                    break;
                case BuildTargetType::Executable:
                    targetFile = 
                        arguments.BinaryDirectory + 
                        Path(arguments.TargetName + ".exe");
                    break;
                default:
                    throw std::runtime_error("Unknown build target type.");
            }

            // Check if the output target is missing
            bool linkRequired = force;
            if (!force)
            {
                if (!IFileSystem::Current().Exists(arguments.WorkingDirectory + targetFile))
                {
                    Log::Verbose("Link target does not exist: " + targetFile.ToString());
                    linkRequired = true;
                }
            }

            if (linkRequired)
            {
                Log::Verbose("Linking target");

                auto linkArguments = LinkArguments();

                linkArguments.TargetFile = std::move(targetFile);
                linkArguments.LibraryFiles = arguments.LinkLibraries;
                linkArguments.RootDirectory = arguments.WorkingDirectory;

                // Translate the target type into the link target
                switch (arguments.TargetType)
                {
                    case BuildTargetType::Library:
                        linkArguments.TargetType = LinkTarget::StaticLibrary;
                        break;
                    case BuildTargetType::Executable:
                        linkArguments.TargetType = LinkTarget::Executable;
                        break;
                    default:
                        throw std::runtime_error("Unknown build target type.");
                }

                // Build up the set of object files
                std::vector<Path> objectFiles;
                for (auto& sourceFile : arguments.SourceFiles)
                {
                    auto objectFile = arguments.ObjectDirectory + Path(sourceFile.GetFileName());
                    objectFile.SetFileExtension(_compiler->GetObjectFileExtension());
                    objectFiles.push_back(objectFile);
                }

                // Add the module interface object file if present
                if (!arguments.ModuleInterfaceSourceFile.ToString().empty())
                {
                    auto objectFile = arguments.ObjectDirectory + Path(arguments.ModuleInterfaceSourceFile.GetFileName());
                    objectFile.SetFileExtension(_compiler->GetObjectFileExtension());
                    objectFiles.push_back(objectFile);
                }

                linkArguments.ObjectFiles = std::move(objectFiles);

                // Ensure the binary directory exists
                // var objectDirectry = Path.Combine(args.RootDirectory, binaryDirectory);
                // if (!Directory.Exists(objectDirectry))
                // {
                //     Directory.CreateDirectory(objectDirectry);
                // }

                // Perform the link
                Log::Info(linkArguments.TargetFile.ToString());
                _compiler->Link(linkArguments);

                return true;
            }
            else
            {
                Log::Verbose("Final target up to date");
                return false;
            }
        }

    private:
        std::shared_ptr<ICompiler> _compiler;
    };
}
