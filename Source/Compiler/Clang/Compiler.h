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
        static constexpr std::string_view ToolsPath = "D:/Repos/llvm/build/Release/";
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
            auto executablePath = Path(ToolsPath) + Path(CompilerExecutable);
            auto commandArgs = ArgumentBuilder::BuildCompilerArguments(args);

            auto result = IProcessManager::Current().Execute(
                executablePath,
                commandArgs,
                args.RootDirectory);
            if (result.ExitCode != 0)
            {
                throw std::runtime_error("Compiler Error: " + std::to_string(result.ExitCode));
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

            return CompileResult();
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
    };
}
