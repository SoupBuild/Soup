// <copyright file="CompilerUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Utilities;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.CSharp.Compiler.Roslyn.UnitTests
{
	public class CompilerTests
	{
		[Fact]
		public void Initialize()
		{
			var uut = new Compiler(
				new Path("C:/bin/mock.csc.exe"));
			Assert.Equal("Roslyn", uut.Name);
			Assert.Equal("obj", uut.ObjectFileExtension);
			Assert.Equal("lib", uut.StaticLibraryFileExtension);
			Assert.Equal("dll", uut.DynamicLibraryFileExtension);
		}

		[Fact]
		public void Compile_Simple()
		{
			var uut = new Compiler(
				new Path("C:/bin/mock.csc.exe"));

			var arguments = new CompileArguments()
			{
				RootDirectory = new Path("Source/"),
				ObjectDirectory = new Path("ObjectDir/"),
				SourceFiles = new List<Path>()
                {
					new Path("File.cs"),
                }
			};

			var result = uut.CreateCompileOperations(arguments);

			// Verify result
			var expected = new List<BuildOperation>()
			{
				new BuildOperation(
					"WriteFile [./ObjectDir/CompileArguments.rsp]",
					new Path("Source/"),
					new Path("./writefile.exe"),
					"\"./ObjectDir/CompileArguments.rsp\" \"/unsafe- /checked- /fullpaths /nostdlib+ /errorreport:prompt /warn:5 /errorendlocation /preferreduilang:en-US /highentropyva+ /nullable:enable /debug+ /debug:portable /filealign:512 /optimize- /out:\"./\" /refout:\"./\" /target:library /warnaserror- /utf8output /deterministic+ /langversion:9.0 \"./File.cs\"\"",
					new List<Path>(),
					new List<Path>()
					{
						new Path("./ObjectDir/CompileArguments.rsp"),
					}),
				new BuildOperation(
					"Compile - ./",
					new Path("Source/"),
					new Path("C:/bin/mock.csc.exe"),
					"@./ObjectDir/CompileArguments.rsp /noconfig",
					new List<Path>()
					{
						new Path("./ObjectDir/CompileArguments.rsp"),
						new Path("File.cs"),
					},
					new List<Path>()
					{
						new Path("./"),
					}),
				};

			Assert.Equal(expected, result);
		}
	}
}
