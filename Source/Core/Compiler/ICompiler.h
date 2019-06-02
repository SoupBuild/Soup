// <copyright file="ICompiler.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "CompileArguments.h"
#include "CompileResults.h"
#include "LinkerArguments.h"

namespace Soup
{
    /// <summary>
    /// The compiler interface definition
    /// </summary>
    export class ICompiler
    {
    public:
        /// <summary>
        /// Gets the unique name for the compiler
        /// </summary>
        virtual const std::string& GetName() const = 0;

        /// <summary>
        /// Gets the object file extension for the compiler
        /// </summary>
        virtual const std::string& GetObjectFileExtension() const = 0;

        /// <summary>
        /// Gets the module file extension for the compiler
        /// </summary>
        virtual const std::string& GetModuleFileExtension() const = 0;

        /// <summary>
        /// Gets the static library file extension for the compiler
        /// TODO: This is platform specific
        /// </summary>
        virtual const std::string& GetStaticLibraryFileExtension() const = 0;

        /// <summary>
        /// Compile
        /// </summary>
        virtual const CompileResults Compile(const CompileArguments& args) = 0;

        /// <summary>
        /// Link Library
        /// </summary>
        virtual void LinkLibrary(LinkerArguments args) = 0;

        /// <summary>
        /// Link Executable
        /// </summary>
        virtual void LinkExecutable(LinkerArguments args) = 0;
    };
}
