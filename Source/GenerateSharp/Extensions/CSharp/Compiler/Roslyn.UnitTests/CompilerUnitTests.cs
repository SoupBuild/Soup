// <copyright file="CompilerUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
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
				Target = new Path("bin/Target.dll"),
				ReferenceTarget = new Path("ref/Target.dll"),
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
					"\"./ObjectDir/CompileArguments.rsp\" \"/unsafe- /checked- /fullpaths /nostdlib+ /errorreport:prompt /warn:5 /errorendlocation /preferreduilang:en-US /highentropyva+ /nullable:enable /debug+ /debug:portable /filealign:512 /optimize- /out:\"./bin/Target.dll\" /refout:\"./ref/Target.dll\" /target:library /warnaserror- /utf8output /deterministic+ /langversion:9.0 \"./File.cs\"\"",
					new List<Path>(),
					new List<Path>()
					{
						new Path("./ObjectDir/CompileArguments.rsp"),
					}),
				new BuildOperation(
					"Compile - ./bin/Target.dll",
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
						new Path("./bin/Target.dll"),
						new Path("./ref/Target.dll"),
					}),
				};

			Assert.Equal(expected, result);
		}
	}
}
