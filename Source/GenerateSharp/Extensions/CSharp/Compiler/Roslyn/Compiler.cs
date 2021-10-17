// <copyright file="Compiler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Collections.Generic;

namespace Soup.Build.CSharp.Compiler.Roslyn
{
	/// <summary>
	/// The Clang compiler implementation
	/// </summary>
	public class Compiler : ICompiler
	{
		public Compiler(Path compilerExecutable)
		{
			_compilerExecutable = compilerExecutable;
		}

		/// <summary>
		/// Gets the unique name for the compiler
		/// </summary>
		public string Name => "Roslyn";

		/// <summary>
		/// Gets the object file extension for the compiler
		/// </summary>
		public string ObjectFileExtension => "obj";

		/// <summary>
		/// Gets the static library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		public string StaticLibraryFileExtension => "lib";

		/// <summary>
		/// Gets the dynamic library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		public string DynamicLibraryFileExtension => "dll";

		/// <summary>
		/// Gets the module file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		public string ModuleFileExtension => "netmodule";

		/// <summary>
		/// Compile
		/// </summary>
		public IList<BuildOperation> CreateCompileOperations(
			CompileArguments arguments)
		{
			var operations = new List<BuildOperation>();

			// Write the shared arguments to the response file
			var responseFile = arguments.ObjectDirectory + new Path("CompileArguments.rsp");
			var sharedCommandArguments = ArgumentBuilder.BuildSharedCompilerArguments(arguments);
			var writeSharedArgumentsOperation = SharedOperations.CreateWriteFileOperation(
				arguments.RootDirectory,
				responseFile,
				string.Join(" ", sharedCommandArguments));
			operations.Add(writeSharedArgumentsOperation);

			// Build up the input/output sets
			var inputFiles = new List<Path>();
			inputFiles.Add(responseFile);
			inputFiles.AddRange(arguments.SourceFiles);
			var outputFiles = new List<Path>()
			{
				arguments.Target,
				arguments.ReferenceTarget,
			};

			// Generate the compile build operation
			var uniqueCommandArguments = ArgumentBuilder.BuildUniqueCompilerArguments();
			var commandArguments = $"@{responseFile} {string.Join(" ", uniqueCommandArguments)}";
			var buildOperation = new BuildOperation(
				$"Compile - {arguments.Target}",
				arguments.RootDirectory,
				_compilerExecutable,
				commandArguments,
				inputFiles,
				outputFiles);
			operations.Add(buildOperation);

			return operations;
		}

		private Path _compilerExecutable;
	}
}
