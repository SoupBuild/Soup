// <copyright file="CompilerArgumentBuilderUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.CSharp.Compiler.Roslyn.UnitTests
{
	public class CompilerArgumentBuilderTests
	{
		[Fact]
		public void BuildSharedCompilerArguments_DefaultParameters()
		{
			var arguments = new CompileArguments()
			{
				Target = new Path("bin/Target.dll"),
				ReferenceTarget = new Path("ref/Target.dll"),
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/unsafe-",
				"/checked-",
				"/fullpaths",
				"/nostdlib+",
				"/errorreport:prompt",
				"/warn:5",
				"/errorendlocation",
				"/preferreduilang:en-US",
				"/highentropyva+",
				"/nullable:enable",
				"/debug+",
				"/debug:portable",
				"/filealign:512",
				"/optimize-",
				"/out:\"./bin/Target.dll\"",
				"/refout:\"./ref/Target.dll\"",
				"/target:library",
				"/warnaserror-",
				"/utf8output",
				"/deterministic+",
				"/langversion:9.0",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildSharedCompilerArguments_SingleArgument_Executable()
		{
			var arguments = new CompileArguments()
			{
				Target = new Path("bin/Target.dll"),
				ReferenceTarget = new Path("ref/Target.dll"),
				TargetType = LinkTarget.Executable,
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/unsafe-",
				"/checked-",
				"/fullpaths",
				"/nostdlib+",
				"/errorreport:prompt",
				"/warn:5",
				"/errorendlocation",
				"/preferreduilang:en-US",
				"/highentropyva+",
				"/nullable:enable",
				"/debug+",
				"/debug:portable",
				"/filealign:512",
				"/optimize-",
				"/out:\"./bin/Target.dll\"",
				"/refout:\"./ref/Target.dll\"",
				"/target:exe",
				"/warnaserror-",
				"/utf8output",
				"/deterministic+",
				"/langversion:9.0",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildSharedCompilerArguments_SingleArgument_EnableWarningsAsErrors()
		{
			var arguments = new CompileArguments()
			{
				Target = new Path("bin/Target.dll"),
				ReferenceTarget = new Path("ref/Target.dll"),
				EnableWarningsAsErrors = true,
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/unsafe-",
				"/checked-",
				"/fullpaths",
				"/nostdlib+",
				"/errorreport:prompt",
				"/warn:5",
				"/errorendlocation",
				"/preferreduilang:en-US",
				"/highentropyva+",
				"/nullable:enable",
				"/debug+",
				"/debug:portable",
				"/filealign:512",
				"/optimize-",
				"/out:\"./bin/Target.dll\"",
				"/refout:\"./ref/Target.dll\"",
				"/target:library",
				"/warnaserror+",
				"/utf8output",
				"/deterministic+",
				"/langversion:9.0",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildSharedCompilerArguments_SingleArgument_GenerateDebugInformation()
		{
			var arguments = new CompileArguments()
			{
				Target = new Path("bin/Target.dll"),
				ReferenceTarget = new Path("ref/Target.dll"),
				GenerateSourceDebugInfo = true,
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/unsafe-",
				"/checked-",
				"/fullpaths",
				"/nostdlib+",
				"/errorreport:prompt",
				"/warn:5",
				"/errorendlocation",
				"/preferreduilang:en-US",
				"/highentropyva+",
				"/nullable:enable",
				"/debug+",
				"/debug:portable",
				"/filealign:512",
				"/optimize-",
				"/out:\"./bin/Target.dll\"",
				"/refout:\"./ref/Target.dll\"",
				"/target:library",
				"/warnaserror-",
				"/utf8output",
				"/deterministic+",
				"/langversion:9.0",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildSharedCompilerArguments_SingleArgument_PreprocessorDefinitions()
		{
			var arguments = new CompileArguments()
			{
				Target = new Path("bin/Target.dll"),
				ReferenceTarget = new Path("ref/Target.dll"),
				PreprocessorDefinitions = new List<string>()
				{
					"DEBUG",
					"VERSION=1"
				},
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/unsafe-",
				"/checked-",
				"/fullpaths",
				"/nostdlib+",
				"/errorreport:prompt",
				"/warn:5",
				"/define:DEBUG;VERSION=1",
				"/errorendlocation",
				"/preferreduilang:en-US",
				"/highentropyva+",
				"/nullable:enable",
				"/debug+",
				"/debug:portable",
				"/filealign:512",
				"/optimize-",
				"/out:\"./bin/Target.dll\"",
				"/refout:\"./ref/Target.dll\"",
				"/target:library",
				"/warnaserror-",
				"/utf8output",
				"/deterministic+",
				"/langversion:9.0",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildUniqueCompilerArguments()
		{
			var actualArguments = ArgumentBuilder.BuildUniqueCompilerArguments();

			var expectedArguments = new List<string>()
			{
				"/noconfig",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}
	}
}
