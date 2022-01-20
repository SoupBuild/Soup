// <copyright file="MockCompiler.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Collections.Generic;

namespace Soup.Build.Cpp.Compiler.Mock
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
			_compileRequests = new List<SharedCompileArguments>();
			_linkRequests = new List<LinkArguments>();
		}

		/// <summary>
		/// Get the compile requests
		/// </summary>
		public IList<SharedCompileArguments> GetCompileRequests()
		{
			return _compileRequests;
		}

		/// <summary>
		/// Get the link requests
		/// </summary>
		public IList<LinkArguments> GetLinkRequests()
		{
			return _linkRequests;
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
		/// Gets the module file extension for the compiler
		/// </summary>
		public string ModuleFileExtension => "mock.bmi";

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
		/// Gets the resource file extension for the compiler
		/// </summary>
		public string ResourceFileExtension => "mock.res";

		/// <summary>
		/// Compile
		/// </summary>
		public IList<BuildOperation> CreateCompileOperations(SharedCompileArguments arguments)
		{
			_compileRequests.Add(arguments);

			var result = new List<BuildOperation>();
			if (!ReferenceEquals(arguments.InterfaceUnit, null))
			{
				result.Add(
					new BuildOperation(
						$"MockCompileModule: {_compileRequests.Count}",
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
			}

			foreach (var fileArguments in arguments.ImplementationUnits)
			{
				result.Add(
					new BuildOperation(
						$"MockCompile: {_compileRequests.Count}",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							fileArguments.SourceFile,
						},
						new List<Path>()
						{
							fileArguments.TargetFile,
						}));
			}

			return result;
		}

		/// <summary>
		/// Link
		/// </summary>
		public BuildOperation CreateLinkOperation(LinkArguments arguments)
		{
			_linkRequests.Add(arguments);
			return new BuildOperation(
				$"MockLink: {_linkRequests.Count}",
				new Path("MockWorkingDirectory"),
				new Path("MockLinker.exe"),
				"Arguments",
				new List< Path>()
				{
					new Path("InputFile.in"),
				},
				new List<Path>()
				{
					new Path("OutputFile.out"),
				});
		}

		private IList<SharedCompileArguments> _compileRequests;
		private IList<LinkArguments> _linkRequests;
	}
}
