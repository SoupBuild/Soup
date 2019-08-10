// <copyright file="Compiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ArgumentBuilder.h"

namespace Soup::Compiler::Clang
{
    /// <summary>
    /// The Clang compiler implementation
    /// </summary>
    export class Compiler : public ICompiler
    {
    private:
        // static Path ToolsPath = "C:/Program Files/llvm/";
        static constexpr std::string_view ToolsPath = "D:/Repos/llvm-project/build/Release/";
        static constexpr std::string_view CompilerExecutable = "bin/clang++.exe";
        static constexpr std::string_view LinkerExecutable = "bin/llvm-ar.exe";

    public:
        /// <summary>
        /// Gets the unique name for the compiler
        /// </summary>
        virtual std::string_view GetName() const override final
        {
            return "Clang";
        }

        /// <summary>
        /// Gets the object file extension for the compiler
        /// </summary>
        virtual std::string_view GetObjectFileExtension() const override final
        {
            return "o";
        }

        /// <summary>
        /// Gets the module file extension for the compiler
        /// </summary>
        virtual std::string_view GetModuleFileExtension() const override final
        {
            return "pcm";
        }

        /// <summary>
        /// Gets the static library file extension for the compiler
        /// TODO: This is platform specific
        /// </summary>
        virtual std::string_view GetStaticLibraryFileExtension() const override final
        {
            return "a";
        }

        /// <summary>
        /// Compile
        /// </summary>
        virtual CompileResult Compile(const CompileArguments& args) override final
        {
            // Clang decided to do their module compilation in two stages
            // Now we have to also generate the object file from the precompiled module
            if (args.ExportModule)
            {
                return CompileModuleInterfaceUnit(args);
            }
            else
            {
                return CompileStandard(args);
            }
        }

        /// <summary>
        /// Link
        /// </summary>
        virtual void Link(const LinkArguments& args) override final
        {
            // Select the correct executable for linking libraries or executables
            Path executablePath;
            switch (args.TargetType)
            {
                case LinkTarget::StaticLibrary:
                    executablePath = Path(ToolsPath) + Path(LinkerExecutable);
                    break;
                case LinkTarget::Executable:
                    executablePath = Path(ToolsPath) + Path(CompilerExecutable);
                    break;
                default:
                    throw std::runtime_error("Unknown LinkTarget.");
            }

            auto commandArgs = ArgumentBuilder::BuildLinkerArguments(args);

            auto result = IProcessManager::Current().Execute(
                executablePath,
                commandArgs,
                args.RootDirectory);
            if (result.ExitCode != 0)
            {
                throw std::runtime_error("Linker Error: " + std::to_string(result.ExitCode));
            }

            if (!result.StdOut.empty())
            {
                Log::Verbose(result.StdOut);
            }

            // If there was any error output then the build failed
            if (!result.StdErr.empty())
            {
                Log::Error(result.StdErr);
                throw std::runtime_error(result.StdErr);
            }
        }

    private:
        CompileResult CompileStandard(const CompileArguments& args)
        {
            auto executablePath = Path(ToolsPath) + Path(CompilerExecutable);
            auto commandArgs = ArgumentBuilder::BuildCompilerArguments(args);

            auto result = IProcessManager::Current().Execute(
                executablePath,
                commandArgs,
                args.RootDirectory);

            if (!result.StdOut.empty())
                Log::Verbose(result.StdOut);

            // If there was any error output then the build failed
            // TODO: Find warnings + errors
            if (!result.StdErr.empty())
                Log::Warning(result.StdErr);

            if (result.ExitCode != 0)
                throw std::runtime_error("Compiler Error: " + std::to_string(result.ExitCode));

            return CompileResult();
        }

        CompileResult CompileModuleInterfaceUnit(const CompileArguments& args)
        {
            auto executablePath = Path(ToolsPath) + Path(CompilerExecutable);

            // Replace the final object target with the intermediate precompiled module
            auto generatePrecompiledModuleArgs = CompileArguments();
            generatePrecompiledModuleArgs.Standard = args.Standard;
            generatePrecompiledModuleArgs.Optimize = args.Optimize;
            generatePrecompiledModuleArgs.RootDirectory = args.RootDirectory;
            generatePrecompiledModuleArgs.IncludeDirectories = args.IncludeDirectories;
            generatePrecompiledModuleArgs.IncludeModules = args.IncludeModules;
            generatePrecompiledModuleArgs.ExportModule = true;
            generatePrecompiledModuleArgs.PreprocessorDefinitions = args.PreprocessorDefinitions;

            // Use the target file as input to the build and generate an object with the same name
            generatePrecompiledModuleArgs.SourceFile = args.SourceFile;
            generatePrecompiledModuleArgs.TargetFile = args.TargetFile;
            generatePrecompiledModuleArgs.TargetFile.SetFileExtension(GetModuleFileExtension());

            auto generatePrecompiledModuleCommandArgs = ArgumentBuilder::BuildCompilerArguments(generatePrecompiledModuleArgs);
            auto result = IProcessManager::Current().Execute(
                executablePath,
                generatePrecompiledModuleCommandArgs,
                args.RootDirectory);

            if (!result.StdOut.empty())
                Log::Verbose(result.StdOut);

            // If there was any error output then the build failed
            // TODO: Find warnings + errors
            if (!result.StdErr.empty())
                Log::Warning(result.StdErr);

            if (result.ExitCode != 0)
                throw std::runtime_error("Compiler Precompile Error: " + std::to_string(result.ExitCode));

            // Now we can compile the object file from the precompiled module
            auto compileObjectArgs = CompileArguments();
            compileObjectArgs.Standard = args.Standard;
            compileObjectArgs.Optimize = args.Optimize;
            compileObjectArgs.RootDirectory = args.RootDirectory;
            compileObjectArgs.SourceFile = generatePrecompiledModuleArgs.TargetFile;
            compileObjectArgs.TargetFile = args.TargetFile;

            auto compileObjectCommandArgs = ArgumentBuilder::BuildCompilerArguments(compileObjectArgs);
            result = IProcessManager::Current().Execute(
                executablePath,
                compileObjectCommandArgs,
                args.RootDirectory);

            if (!result.StdOut.empty())
                Log::Verbose(result.StdOut);

            // If there was any error output then the build failed
            // TODO: Find warnings + errors
            if (!result.StdErr.empty())
                Log::Warning(result.StdErr);

            if (result.ExitCode != 0)
                throw std::runtime_error("Compiler Object Error: " + std::to_string(result.ExitCode));

            return CompileResult();
        }
    };
}
