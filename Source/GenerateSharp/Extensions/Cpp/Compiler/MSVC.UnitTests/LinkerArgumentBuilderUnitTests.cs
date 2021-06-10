// <copyright file="LinkerArgumentBuilderUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Cpp.Compiler.MSVC.UnitTests
{
	public class LinkerArgumentBuilderTests
	{
		[Fact]
		public void ZeroObjectFiles()
		{
			var arguments = new LinkArguments()
			{
				TargetType = LinkTarget.StaticLibrary,
				TargetArchitecture = "x64",
				TargetFile = new Path("Library.mock.lib"),
				ObjectFiles = new List<Path>(),
			};

			var actualArguments = ArgumentBuilder.BuildLinkerArguments(arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/machine:X64",
				"/out:\"./Library.mock.lib\"",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void EmptyTargetFile_Throws()
		{
			var arguments = new LinkArguments();
			arguments.TargetType = LinkTarget.StaticLibrary;
			arguments.TargetFile = new Path("");
			arguments.ObjectFiles = new List<Path>()
			{
				new Path("File.mock.obj"),
			};
			Assert.Throws<InvalidOperationException>(() => {
				var actualArguments = ArgumentBuilder.BuildLinkerArguments(arguments);
			});
		}

		[Fact]
		public void StaticLibrary()
		{
			var arguments = new LinkArguments();
			arguments.TargetType = LinkTarget.StaticLibrary;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = new Path("Library.mock.lib");
			arguments.ObjectFiles = new List<Path>()
			{
				new Path("File.mock.o"),
			};

			var actualArguments = ArgumentBuilder.BuildLinkerArguments(arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/machine:X64",
				"/out:\"./Library.mock.lib\"",
				"./File.mock.o",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void StaticLibrary_LibraryPaths()
		{
			var arguments = new LinkArguments();
			arguments.TargetType = LinkTarget.StaticLibrary;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = new Path("Library.mock.lib");
			arguments.ObjectFiles = new List<Path>()
			{
				new Path("File.mock.o"),
			};
			arguments.LibraryPaths = new List<Path>()
			{
				new Path("../libraries/"),
			};

			var actualArguments = ArgumentBuilder.BuildLinkerArguments(arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/machine:X64",
				"/libpath:\"../libraries/\"",
				"/out:\"./Library.mock.lib\"",
				"./File.mock.o",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void DynamicLibrary()
		{
			var arguments = new LinkArguments();
			arguments.TargetType = LinkTarget.DynamicLibrary;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = new Path("Library.mock.dll");
			arguments.ImplementationFile = new Path("Library.mock.lib");
			arguments.ObjectFiles = new List<Path>()
			{
				new Path("File.mock.obj"),
			};

			var actualArguments = ArgumentBuilder.BuildLinkerArguments(arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/subsystem:console",
				"/dll",
				"/implib:\"./Library.mock.lib\"",
				"/machine:X64",
				"/out:\"./Library.mock.dll\"",
				"./File.mock.obj",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void Executable()
		{
			var arguments = new LinkArguments();
			arguments.TargetType = LinkTarget.Executable;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = new Path("out/Something.exe");
			arguments.ObjectFiles = new List<Path>()
			{
				new Path("File.mock.obj"),
			};
			arguments.LibraryFiles = new List<Path>()
			{
				new Path("Library.mock.lib"),
			};

			var actualArguments = ArgumentBuilder.BuildLinkerArguments(arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/subsystem:console",
				"/machine:X64",
				"/out:\"./out/Something.exe\"",
				"./Library.mock.lib",
				"./File.mock.obj",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}
	}
}
