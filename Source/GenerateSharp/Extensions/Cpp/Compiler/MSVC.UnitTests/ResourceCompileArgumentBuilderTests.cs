// <copyright file="CompilerArgumentBuilderUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Cpp.Compiler.MSVC.UnitTests
{
	public class ResourceCompileArgumentBuilderTests
	{
		[Fact]
		public void BuildResourceCompilerArguments_Simple()
		{
			var targetRootDirectory = new Path("C:/target/");

			var arguments = new SharedCompileArguments()
			{
				ResourceFile = new ResourceCompileArguments()
				{
					SourceFile = new Path("Resources.rc"),
					TargetFile = new Path("Resources.mock.res"),
				},
			};

			var actualArguments = ArgumentBuilder.BuildResourceCompilerArguments(
				targetRootDirectory,
				arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/D_UNICODE",
				"/DUNICODE",
				"/l\"0x0409\"",
				"/Fo\"C:/target/Resources.mock.res\"",
				"./Resources.rc",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}
	}
}
