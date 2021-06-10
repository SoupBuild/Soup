// <copyright file="ICompiler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.Build.Cpp.Compiler
{
	/// <summary>
	/// The compiler interface definition
	/// </summary>
	public interface ICompiler
	{
		/// <summary>
		/// Gets the unique name for the compiler
		/// </summary>
		string Name { get; }

		/// <summary>
		/// Gets the object file extension for the compiler
		/// </summary>
		string ObjectFileExtension { get; }

		/// <summary>
		/// Gets the module file extension for the compiler
		/// </summary>
		string ModuleFileExtension { get; }

		/// <summary>
		/// Gets the static library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		string StaticLibraryFileExtension { get; }

		/// <summary>
		/// Gets the dynamic library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		string DynamicLibraryFileExtension { get; }

		/// <summary>
		/// Compile
		/// </summary>
		IList<Utilities.BuildOperation> CreateCompileOperations(SharedCompileArguments arguments);

		/// <summary>
		/// Link
		/// </summary>
		Utilities.BuildOperation CreateLinkOperation(LinkArguments arguments);
	}
}
