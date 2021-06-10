// <copyright file="CompilerArgumentBuilderUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Cpp.Compiler.MSVC.UnitTests
{
	public class CompilerArgumentBuilderTests
	{
		[Fact]
		public void BuildSharedCompilerArguments_DefaultParameters()
		{
			var arguments = new SharedCompileArguments();

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++11",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Theory]
		[InlineData(LanguageStandard.CPP11, "/std:c++11")]
		[InlineData(LanguageStandard.CPP14, "/std:c++14")]
		[InlineData(LanguageStandard.CPP17, "/std:c++17")]
		public void BuildSharedCompilerArguments_SingleArgument_LanguageStandard(
			LanguageStandard standard,
			string expectedFlag)
		{
			var arguments = new SharedCompileArguments()
			{
				Standard = standard,
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				expectedFlag,
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildSharedCompilerArguments_SingleArgument_LanguageStandard_CPP20()
		{
			var arguments = new SharedCompileArguments()
			{
				Standard = LanguageStandard.CPP20,
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++latest",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildSharedCompilerArguments_SingleArgument_OptimizationLevel_Disabled()
		{
			var arguments = new SharedCompileArguments()
			{
				Standard = LanguageStandard.CPP17,
				Optimize = OptimizationLevel.None,
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++17",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Theory]
		[InlineData(OptimizationLevel.Size, "/Os")]
		[InlineData(OptimizationLevel.Speed, "/Ot")]
		public void BuildSharedCompilerArguments_SingleArgument_OptimizationLevel(
			OptimizationLevel level,
			string expectedFlag)
		{
			var arguments = new SharedCompileArguments()
			{
				Standard = LanguageStandard.CPP17,
				Optimize = level,
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++17",
				expectedFlag,
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildSharedCompilerArguments_SingleArgument_EnableWarningsAsErrors()
		{
			var arguments = new SharedCompileArguments()
			{
				Standard = LanguageStandard.CPP17,
				Optimize = OptimizationLevel.None,
				EnableWarningsAsErrors = true,
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/WX",
				"/W4",
				"/std:c++17",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildSharedCompilerArguments_SingleArgument_GenerateDebugInformation()
		{
			var arguments = new SharedCompileArguments()
			{
				Standard = LanguageStandard.CPP17,
				Optimize = OptimizationLevel.None,
				GenerateSourceDebugInfo = true,
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/Z7",
				"/W4",
				"/std:c++17",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MTd",
				"/bigobj",
				"/c",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildSharedCompilerArguments_SingleArgument_IncludePaths()
		{
			var arguments = new SharedCompileArguments()
			{
				IncludeDirectories = new List<Path>()
				{
					new Path("C:/Files/SDK/"),
					new Path("my files/")
				},
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++11",
				"/Od",
				"/I\"C:/Files/SDK/\"",
				"/I\"./my files/\"",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildSharedCompilerArguments_SingleArgument_PreprocessorDefinitions()
		{
			var arguments = new SharedCompileArguments()
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
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++11",
				"/Od",
				"/DDEBUG",
				"/DVERSION=1",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/bigobj",
				"/c",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildSharedCompilerArguments_SingleArgument_Modules()
		{
			var arguments = new SharedCompileArguments()
			{
				IncludeModules = new List<Path>()
				{
					new Path("Module.pcm"),
					new Path("Std.pcm"),
				},
			};

			var actualArguments = ArgumentBuilder.BuildSharedCompilerArguments(
				arguments);

			var expectedArguments = new List<string>()
			{
				"/nologo",
				"/FC",
				"/permissive-",
				"/Zc:__cplusplus",
				"/Zc:externConstexpr",
				"/Zc:inline",
				"/Zc:throwingNew",
				"/W4",
				"/std:c++11",
				"/Od",
				"/X",
				"/RTC1",
				"/EHsc",
				"/MT",
				"/reference",
				"\"./Module.pcm\"",
				"/reference",
				"\"./Std.pcm\"",
				"/bigobj",
				"/c",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildInterfaceUnitCompilerArguments()
		{
			var arguments = new InterfaceUnitCompileArguments();
			arguments.SourceFile = new Path("module.cpp");
			arguments.TargetFile = new Path("module.obj");
			arguments.ModuleInterfaceTarget = new Path("module.ifc");

			var responseFile = new Path("ResponseFile.txt");

			var actualArguments = ArgumentBuilder.BuildInterfaceUnitCompilerArguments(
				arguments,
				responseFile);

			var expectedArguments = new List<string>()
			{
				"@./ResponseFile.txt",
				"./module.cpp",
				"/Fo\"./module.obj\"",
				"/interface",
				"/ifcOutput",
				"\"./module.ifc\"",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildTranslationUnitCompilerArguments_Simple()
		{
			var arguments = new TranslationUnitCompileArguments()
			{
				SourceFile = new Path("module.cpp"),
				TargetFile = new Path("module.obj"),
			};

			var responseFile = new Path("ResponseFile.txt");
			var internalModules = new List<Path>();

			var actualArguments = ArgumentBuilder.BuildTranslationUnitCompilerArguments(
				arguments,
				responseFile,
				internalModules);

			var expectedArguments = new List<string>()
			{
				"@./ResponseFile.txt",
				"./module.cpp",
				"/Fo\"./module.obj\"",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}

		[Fact]
		public void BuildTranslationUnitCompilerArguments_InternalModules()
		{
			var arguments = new TranslationUnitCompileArguments()
			{
				SourceFile = new Path("module.cpp"),
				TargetFile = new Path("module.obj"),
			};

			var responseFile = new Path("ResponseFile.txt");
			var internalModules = new List<Path>()
			{
				new Path("Module1.ifc"),
				new Path("Module2.ifc"),
			};

			var actualArguments = ArgumentBuilder.BuildTranslationUnitCompilerArguments(
				arguments,
				responseFile,
				internalModules);

			var expectedArguments = new List<string>()
			{
				"@./ResponseFile.txt",
				"/reference",
				"\"./Module1.ifc\"",
				"/reference",
				"\"./Module2.ifc\"",
				"./module.cpp",
				"/Fo\"./module.obj\"",
			};

			Assert.Equal(expectedArguments, actualArguments);
		}
	}
}
