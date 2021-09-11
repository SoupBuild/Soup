// <copyright file="MockCompiler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Utilities;
using System.Collections.Generic;

namespace Soup.Build.CSharp.Compiler.Mock
{
	/// <summary>
	/// A mock compiler interface implementation
	/// TODO: Move into test projects
	/// </summary>
	public class Compiler : ICompiler
	{
		/// <summary>
		/// Initializes a new instance of the <see cref='Compiler'/> class.
		/// </summary>
		public Compiler()
		{
			_compileRequests = new List<CompileArguments>();
		}

		/// <summary>
		/// Get the compile requests
		/// </summary>
		public IList<CompileArguments> GetCompileRequests()
		{
			return _compileRequests;
		}

		/// <summary>
		/// Gets the unique name for the compiler
		/// </summary>
		public string Name => "MockCompiler";

		/// <summary>
		/// Gets the object file extension for the compiler
		/// </summary>
		public string ObjectFileExtension => "mock.obj";

		/// <summary>
		/// Gets the static library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		public string StaticLibraryFileExtension => "mock.lib";

		/// <summary>
		/// Gets the dynamic library file extension for the compiler
		/// TODO: This is platform specific
		/// </summary>
		public string DynamicLibraryFileExtension =>  "mock.dll";

		/// <summary>
		/// Compile
		/// </summary>
		public IList<BuildOperation> CreateCompileOperations(CompileArguments arguments)
		{
			_compileRequests.Add(arguments);

			var result = new List<BuildOperation>();
			result.Add(
				new BuildOperation(
					$"MockCompile: {_compileRequests.Count}",
					new Path("MockWorkingDirectory"),
					new Path("MockCompiler.exe"),
					"Arguments",
					new List<Path>()
					{
						new Path("InputFile.in"),
					},
					new List<Path>()
					{
						new Path("OutputFile.out"),
					}));

			return result;
		}

		private IList<CompileArguments> _compileRequests;
	}
}
