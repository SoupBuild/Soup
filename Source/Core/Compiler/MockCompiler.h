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
        /// Initializes a new instance of the <see cref='MockCompiler'/> class.
        /// </summary>
        MockCompiler() :
            _compileRequests(),
            _linkLibraryRequests(),
            _linkExecutableRequests()
        {
        }

        /// <summary>
        /// Get the compile requests
        /// </summary>
        const std::vector<CompileArguments>& GetCompileRequests() const
        {
            return _compileRequests;
        }

        /// <summary>
        /// Get the link library requests
        /// </summary>
        const std::vector<LinkerArguments>& GetLinkLibraryRequests() const
        {
            return _linkLibraryRequests;
        }

        /// <summary>
        /// Get the link executable requests
        /// </summary>
        const std::vector<LinkerArguments>& GetLinkExecutableRequests() const
        {
            return _linkExecutableRequests;
        }

        /// <summary>
        /// Gets the unique name for the compiler
        /// </summary>
        virtual const std::string& GetName() const override final
        {
            static const std::string value = "MockCompiler";
            return value;
        }

        /// <summary>
        /// Gets the object file extension for the compiler
        /// </summary>
        virtual const std::string& GetObjectFileExtension() const override final
        {
            static const std::string value = ".mock.obj";
            return value;
        }

        /// <summary>
        /// Gets the module file extension for the compiler
        /// </summary>
        virtual const std::string& GetModuleFileExtension() const override final
        {
            static const std::string value = ".mock.bmi";
            return value;
        }

        /// <summary>
        /// Gets the static library file extension for the compiler
        /// TODO: This is platform specific
        /// </summary>
        virtual const std::string& GetStaticLibraryFileExtension() const override final
        {
            static const std::string value = ".mock.lib";
            return value;
        }

        /// <summary>
        /// Compile
        /// </summary>
        virtual CompileResult Compile(const CompileArguments& args) override final
        {
            _compileRequests.push_back(args);
            return CompileResult();
        }

        /// <summary>
        /// Link Library
        /// </summary>
        virtual void LinkLibrary(const LinkerArguments& args) override final
        {
            _linkLibraryRequests.push_back(args);
        }

        /// <summary>
        /// Link Executable
        /// </summary>
        virtual void LinkExecutable(const LinkerArguments& args) override final
        {
            _linkExecutableRequests.push_back(args);
        }

    private:
        std::vector<CompileArguments> _compileRequests;
        std::vector<LinkerArguments> _linkLibraryRequests;
        std::vector<LinkerArguments> _linkExecutableRequests;
    };
}
