// <copyright file="MockCompiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup
{
    /// <summary>
    /// A mock compiler interface implementation
    /// TODO: Move into test projects
    /// </summary>
    export class MockCompiler : public ICompiler
    {
    public:
        /// <summary>
        /// Gets the unique name for the compiler
        /// </summary>
        virtual const std::string& GetName() const override final
        {
            throw std::runtime_error("MOCK NotImplemented.");
        }

        /// <summary>
        /// Gets the object file extension for the compiler
        /// </summary>
        virtual const std::string& GetObjectFileExtension() const override final
        {
            throw std::runtime_error("MOCK NotImplemented.");
        }

        /// <summary>
        /// Gets the module file extension for the compiler
        /// </summary>
        virtual const std::string& GetModuleFileExtension() const override final
        {
            throw std::runtime_error("MOCK NotImplemented.");
        }

        /// <summary>
        /// Gets the static library file extension for the compiler
        /// TODO: This is platform specific
        /// </summary>
        virtual const std::string& GetStaticLibraryFileExtension() const override final
        {
            throw std::runtime_error("MOCK NotImplemented.");
        }

        /// <summary>
        /// Compile
        /// </summary>
        virtual const CompileResults Compile(const CompileArguments& args) override final
        {
            throw std::runtime_error("MOCK NotImplemented.");
        }

        /// <summary>
        /// Link Library
        /// </summary>
        virtual void LinkLibrary(LinkerArguments args) override final
        {
            throw std::runtime_error("MOCK NotImplemented.");
        }

        /// <summary>
        /// Link Executable
        /// </summary>
        virtual void LinkExecutable(LinkerArguments args) override final
        {
            throw std::runtime_error("MOCK NotImplemented.");
        }
    };
}
