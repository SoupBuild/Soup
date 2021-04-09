// <copyright file="Compiler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Soup.Build.Cpp.Compiler.MSVC
{
	/// <summary>
	/// The Clang compiler implementation
	/// </summary>
	public class Compiler : ICompiler
	{
		public Compiler(Path compilerExecutable, Path linkerExecutable, Path libraryExecutable)
		{
			_compilerExecutable = compilerExecutable;
			_linkerExecutable = linkerExecutable;
			_libraryExecutable = libraryExecutable;
		}

		/// <summary>
		/// Gets the unique name for the compiler
		/// </summary>
		public string Name => "MSVC";

		/// <summary>
		/// Gets the object file extension for the compiler
		/// </summary>
		public string ObjectFileExtension => "obj";

		/// <summary>
		/// Gets the module file extension for the compiler
		/// </summary>
		public string ModuleFileExtension => "ifc";

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
		/// Compile
		/// </summary>
		public IList<BuildOperation> CreateCompileOperations(
			SharedCompileArguments arguments)
		{
			var operations = new List<BuildOperation>();

			// Write the shared arguments to the response file
			var responseFile = arguments.ObjectDirectory + new Path("SharedCompileArguments.txt");
			var sharedCommandArguments = ArgumentBuilder.BuildSharedCompilerArguments(arguments);
			var writeSharedArgumentsOperation = SharedOperations.CreateWriteFileOperation(
				arguments.RootDirectory,
				responseFile,
				CombineArguments(sharedCommandArguments));
			operations.Add(writeSharedArgumentsOperation);

			// Initialize a shared input set
			var sharedInputFiles = arguments.IncludeModules;

			// Generate the interface build operation if present
			var internalModules = new List<Path>();
			if (arguments.InterfaceUnit is not null)
			{
				var interfaceUnitArguments = arguments.InterfaceUnit;

				// Build up the input/output sets
				var inputFiles = sharedInputFiles.ToList();
				inputFiles.Add(interfaceUnitArguments.SourceFile);
				inputFiles.Add(responseFile);
				var outputFiles = new List<Path>()
				{
					interfaceUnitArguments.TargetFile,
					interfaceUnitArguments.ModuleInterfaceTarget,
				};

				// Build the unique arguments for this translation unit
				var commandArguments = ArgumentBuilder.BuildInterfaceUnitCompilerArguments(
					interfaceUnitArguments,
					responseFile);

				// Generate the operation
				var buildOperation = new BuildOperation(
					interfaceUnitArguments.SourceFile.ToString(),
					arguments.RootDirectory,
					_compilerExecutable,
					CombineArguments(commandArguments),
					inputFiles,
					outputFiles);
				operations.Add(buildOperation);

				// Add our module interface back in for the downstream compilers
				internalModules.Add(interfaceUnitArguments.ModuleInterfaceTarget);
			}

			foreach (var implementationUnitArguments in arguments.ImplementationUnits)
			{
				// Build up the input/output sets
				var inputFiles = sharedInputFiles.ToList();
				inputFiles.Add(implementationUnitArguments.SourceFile);
				inputFiles.Add(responseFile);
				var outputFiles = new List<Path>()
				{
					implementationUnitArguments.TargetFile,
				};

				// Build the unique arguments for this translation unit
				var commandArguments = ArgumentBuilder.BuildTranslationUnitCompilerArguments(
					implementationUnitArguments,
					responseFile,
					internalModules);

				// Generate the operation
				var buildOperation = new BuildOperation(
					implementationUnitArguments.SourceFile.ToString(),
					arguments.RootDirectory,
					_compilerExecutable,
					CombineArguments(commandArguments),
					inputFiles.ToArray(),
					outputFiles);
				operations.Add(buildOperation);
			}

			return operations;
		}

		/// <summary>
		/// Link
		/// </summary>
		public BuildOperation CreateLinkOperation(
			LinkArguments arguments)
		{
			// Select the correct executable for linking libraries or executables
			Path executablePath;
			switch (arguments.TargetType)
			{
				case LinkTarget.StaticLibrary:
					executablePath = _libraryExecutable;
					break;
				case LinkTarget.DynamicLibrary:
				case LinkTarget.Executable:
					executablePath = _linkerExecutable;
					break;
				default:
					throw new InvalidOperationException("Unknown LinkTarget.");
			}

			// Build the set of input/output files along with the arguments
			var inputFiles = new List<Path>();
			inputFiles.AddRange(arguments.LibraryFiles);
			inputFiles.AddRange(arguments.ObjectFiles);
			var outputFiles = new List<Path>()
			{
				arguments.TargetFile,
			};
			var commandarguments = ArgumentBuilder.BuildLinkerArguments(arguments);

			var buildOperation = new BuildOperation(
				arguments.TargetFile.ToString(),
				arguments.RootDirectory,
				executablePath,
				CombineArguments(commandarguments),
				inputFiles,
				outputFiles);

			return buildOperation;
		}

		private static string CombineArguments(IList<string> arguments)
		{
			var argumentString = new StringBuilder();
			bool isFirst = true;
			foreach (var arg in arguments)
			{
				if (!isFirst)
					argumentString.Append(" ");

				argumentString.Append(arg);
				isFirst = false;
			}

			return argumentString.ToString();
		}

		private Path _compilerExecutable;
		private Path _linkerExecutable;
		private Path _libraryExecutable;
	}
}
