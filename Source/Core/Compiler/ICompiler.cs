// <copyright file="ICompiler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Compiler
{
    using System.Threading.Tasks;

    /// <summary>
    /// The compiler interface definition
    /// </summary>
    public interface ICompiler
    {
        /// <summary>
        /// Compile
        /// </summary>
        Task CompileAsync(CompilerArguments args);

        /// <summary>
        /// Link Library
        /// </summary>
        Task LinkLibraryAsync(LinkerArguments args);

        /// <summary>
        /// Link Executable
        /// </summary>
        Task LinkExecutableAsync(LinkerArguments args);
    }
}
