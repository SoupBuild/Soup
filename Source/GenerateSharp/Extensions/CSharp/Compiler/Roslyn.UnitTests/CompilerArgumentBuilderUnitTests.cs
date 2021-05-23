// <copyright file="CompilerArgumentBuilderUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using Xunit;

namespace Soup.Build.CSharp.Compiler.Roslyn.UnitTests
{
	public class CompilerArgumentBuilderTests
	{
		[Fact]
		public void BuildSharedCompilerArguments_DefaultParameters()
		{
			var arguments = new CompileArguments();

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
				"/out:\"./\"",
				"/refout:\"./\"",
				"/target:library",
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
				"/out:\"./\"",
				"/refout:\"./\"",
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
				"/out:\"./\"",
				"/refout:\"./\"",
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
				"/out:\"./\"",
				"/refout:\"./\"",
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
