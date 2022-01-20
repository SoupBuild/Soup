// <copyright file="CompilerUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Cpp.Compiler.MSVC.UnitTests
{
	public class CompilerTests
	{
		[Fact]
		public void Initialize()
		{
			var uut = new Compiler(
				new Path("C:/bin/mock.cl.exe"),
				new Path("C:/bin/mock.link.exe"),
				new Path("C:/bin/mock.lib.exe"),
				new Path("C:/bin/mock.rc.exe"));
			Assert.Equal("MSVC", uut.Name);
			Assert.Equal("obj", uut.ObjectFileExtension);
			Assert.Equal("ifc", uut.ModuleFileExtension);
			Assert.Equal("lib", uut.StaticLibraryFileExtension);
			Assert.Equal("dll", uut.DynamicLibraryFileExtension);
			Assert.Equal("res", uut.ResourceFileExtension);
		}

		[Fact]
		public void Compile_Simple()
		{
			var uut = new Compiler(
				new Path("C:/bin/mock.cl.exe"),
				new Path("C:/bin/mock.link.exe"),
				new Path("C:/bin/mock.lib.exe"),
				new Path("C:/bin/mock.rc.exe"));

			var arguments = new SharedCompileArguments()
			{
				SourceRootDirectory = new Path("C:/source/"),
				TargetRootDirectory = new Path("C:/target/"),
				ObjectDirectory = new Path("ObjectDir/"),
			};

			var translationUnitArguments = new TranslationUnitCompileArguments()
			{
				SourceFile = new Path("File.cpp"),
				TargetFile = new Path("obj/File.obj"),
			};

			arguments.ImplementationUnits = new List<TranslationUnitCompileArguments>()
			{
				translationUnitArguments,
			};

			var result = uut.CreateCompileOperations(arguments);

			// Verify result
			var expected = new List<BuildOperation>()
			{
				new BuildOperation(
					"WriteFile [./ObjectDir/SharedCompileArguments.rsp]",
					new Path("C:/target/"),
					new Path("./writefile.exe"),
					"\"./ObjectDir/SharedCompileArguments.rsp\" \"/nologo /FC /permissive- /Zc:__cplusplus /Zc:externConstexpr /Zc:inline /Zc:throwingNew /W4 /std:c++11 /Od /X /RTC1 /EHsc /MT /bigobj /c\"",
					new List<Path>(),
					new List<Path>()
					{
						new Path("./ObjectDir/SharedCompileArguments.rsp"),
					}),
				new BuildOperation(
					"./File.cpp",
					new Path("C:/source/"),
					new Path("C:/bin/mock.cl.exe"),
					"@C:/target/ObjectDir/SharedCompileArguments.rsp ./File.cpp /Fo\"C:/target/obj/File.obj\"",
					new List<Path>()
					{
						new Path("File.cpp"),
						new Path("C:/target/ObjectDir/SharedCompileArguments.rsp"),
					},
					new List<Path>()
					{
						new Path("C:/target/obj/File.obj"),
					}),
				};

			Assert.Equal(expected, result);
		}

		[Fact]
		public void Compile_Module()
		{
			var uut = new Compiler(
				new Path("C:/bin/mock.cl.exe"),
				new Path("C:/bin/mock.link.exe"),
				new Path("C:/bin/mock.lib.exe"),
				new Path("C:/bin/mock.rc.exe"));

			var arguments = new SharedCompileArguments()
			{
				SourceRootDirectory = new Path("C:/source/"),
				TargetRootDirectory = new Path("C:/target/"),
				ObjectDirectory = new Path("ObjectDir/"),
				IncludeDirectories = new List<Path>()
				{
					new Path("Includes"),
				},
				IncludeModules = new List<Path>()
				{
					new Path("Module.pcm"),
				},
				PreprocessorDefinitions = new List<string>()
				{
					"DEBUG"
				},
			};

			var compileModuleArguments = new InterfaceUnitCompileArguments();
			compileModuleArguments.SourceFile = new Path("File.cpp");
			compileModuleArguments.TargetFile = new Path("obj/File.obj");
			compileModuleArguments.ModuleInterfaceTarget = new Path("obj/File.pcm");
			arguments.InterfaceUnit = compileModuleArguments;

			var result = uut.CreateCompileOperations(arguments);

			// Verify result
			var expected = new List<BuildOperation>()
			{
				new BuildOperation(
					"WriteFile [./ObjectDir/SharedCompileArguments.rsp]",
					new Path("C:/target/"),
					new Path("./writefile.exe"),
					"\"./ObjectDir/SharedCompileArguments.rsp\" \"/nologo /FC /permissive- /Zc:__cplusplus /Zc:externConstexpr /Zc:inline /Zc:throwingNew /W4 /std:c++11 /Od /I\"./Includes\" /DDEBUG /X /RTC1 /EHsc /MT /reference \"./Module.pcm\" /bigobj /c\"",
					new List<Path>(),
					new List<Path>()
					{
						new Path("./ObjectDir/SharedCompileArguments.rsp"),
					}),
				new BuildOperation(
					"./File.cpp",
					new Path("C:/source/"),
					new Path("C:/bin/mock.cl.exe"),
					"@C:/target/ObjectDir/SharedCompileArguments.rsp ./File.cpp /Fo\"C:/target/obj/File.obj\" /interface /ifcOutput \"C:/target/obj/File.pcm\"",
					new List<Path>()
					{
						new Path("Module.pcm"),
						new Path("File.cpp"),
						new Path("C:/target/ObjectDir/SharedCompileArguments.rsp"),
					},
					new List<Path>()
					{
						new Path("C:/target/obj/File.obj"),
						new Path("C:/target/obj/File.pcm"),
					}),
				};

			Assert.Equal(expected, result);
		}

		[Fact]
		public void Compile_Resource()
		{
			var uut = new Compiler(
				new Path("C:/bin/mock.cl.exe"),
				new Path("C:/bin/mock.link.exe"),
				new Path("C:/bin/mock.lib.exe"),
				new Path("C:/bin/mock.rc.exe"));

			var arguments = new SharedCompileArguments()
			{
				SourceRootDirectory = new Path("C:/source/"),
				TargetRootDirectory = new Path("C:/target/"),
				ObjectDirectory = new Path("ObjectDir/"),
				IncludeDirectories = new List<Path>()
				{
					new Path("Includes"),
				},
				IncludeModules = new List<Path>()
				{
					new Path("Module.pcm"),
				},
				PreprocessorDefinitions = new List<string>()
				{
					"DEBUG"
				},
				ResourceFile = new ResourceCompileArguments()
				{
					SourceFile = new Path("Resources.rc"),
					TargetFile = new Path("obj/Resources.res"),
				},
			};

			var result = uut.CreateCompileOperations(arguments);

			// Verify result
			var expected = new List<BuildOperation>()
			{
				new BuildOperation(
					"WriteFile [./ObjectDir/SharedCompileArguments.rsp]",
					new Path("C:/target/"),
					new Path("./writefile.exe"),
					"\"./ObjectDir/SharedCompileArguments.rsp\" \"/nologo /FC /permissive- /Zc:__cplusplus /Zc:externConstexpr /Zc:inline /Zc:throwingNew /W4 /std:c++11 /Od /I\"./Includes\" /DDEBUG /X /RTC1 /EHsc /MT /reference \"./Module.pcm\" /bigobj /c\"",
					new List<Path>(),
					new List<Path>()
					{
						new Path("./ObjectDir/SharedCompileArguments.rsp"),
					}),
				new BuildOperation(
					"./Resources.rc",
					new Path("C:/source/"),
					new Path("C:/bin/mock.rc.exe"),
					"/nologo /D_UNICODE /DUNICODE /l\"0x0409\" /I\"./Includes\" /Fo\"C:/target/obj/Resources.res\" ./Resources.rc",
					new List<Path>()
					{
						new Path("Module.pcm"),
						new Path("Resources.rc"),
						new Path("C:/target/fake_file"),
					},
					new List<Path>()
					{
						new Path("C:/target/obj/Resources.res"),
					}),
				};

			Assert.Equal(expected, result);
		}

		[Fact]
		public void LinkStaticLibrary_Simple()
		{
			var uut = new Compiler(
				new Path("C:/bin/mock.cl.exe"),
				new Path("C:/bin/mock.link.exe"),
				new Path("C:/bin/mock.lib.exe"),
				new Path("C:/bin/mock.rc.exe"));

			var arguments = new LinkArguments();
			arguments.TargetType = LinkTarget.StaticLibrary;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = new Path("Library.mock.a");
			arguments.TargetRootDirectory = new Path("C:/target/");
			arguments.ObjectFiles = new List<Path>()
			{
				new Path("File.mock.obj"),
			};

			var result = uut.CreateLinkOperation(arguments);

			// Verify result
			var expected = new BuildOperation(
				"./Library.mock.a",
				new Path("C:/target/"),
				new Path("C:/bin/mock.lib.exe"),
				"/nologo /machine:X64 /out:\"./Library.mock.a\" ./File.mock.obj",
				new List<Path>()
				{
					new Path("File.mock.obj"),
				},
				new List<Path>()
				{
					new Path("C:/target/Library.mock.a"),
				});

			Assert.Equal(expected, result);
		}

		[Fact]
		public void LinkExecutable_Simple()
		{
			var uut = new Compiler(
				new Path("C:/bin/mock.cl.exe"),
				new Path("C:/bin/mock.link.exe"),
				new Path("C:/bin/mock.lib.exe"),
				new Path("C:/bin/mock.rc.exe"));

			var arguments = new LinkArguments();
			arguments.TargetType = LinkTarget.Executable;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = new Path("Something.exe");
			arguments.TargetRootDirectory = new Path("C:/target/");
			arguments.ObjectFiles = new List<Path>()
			{
				new Path("File.mock.obj"),
			};
			arguments.LibraryFiles = new List<Path>()
			{
				new Path("Library.mock.a"),
			};

			var result = uut.CreateLinkOperation(arguments);

			// Verify result
			var expected = new BuildOperation(
				"./Something.exe",
				new Path("C:/target/"),
				new Path("C:/bin/mock.link.exe"),
				"/nologo /subsystem:console /machine:X64 /out:\"./Something.exe\" ./Library.mock.a ./File.mock.obj",
				new List<Path>()
				{
					new Path("Library.mock.a"),
					new Path("File.mock.obj"),
				},
				new List<Path>()
				{
					new Path("C:/target/Something.exe"),
				});

			Assert.Equal(expected, result);
		}

		[Fact]
		public void LinkWindowsApplication_Simple()
		{
			var uut = new Compiler(
				new Path("C:/bin/mock.cl.exe"),
				new Path("C:/bin/mock.link.exe"),
				new Path("C:/bin/mock.lib.exe"),
				new Path("C:/bin/mock.rc.exe"));

			var arguments = new LinkArguments();
			arguments.TargetType = LinkTarget.WindowsApplication;
			arguments.TargetArchitecture = "x64";
			arguments.TargetFile = new Path("Something.exe");
			arguments.TargetRootDirectory = new Path("C:/target/");
			arguments.ObjectFiles = new List<Path>()
			{
				new Path("File.mock.obj"),
			};
			arguments.LibraryFiles = new List<Path>()
			{
				new Path("Library.mock.a"),
			};

			var result = uut.CreateLinkOperation(arguments);

			// Verify result
			var expected = new BuildOperation(
				"./Something.exe",
				new Path("C:/target/"),
				new Path("C:/bin/mock.link.exe"),
				"/nologo /subsystem:windows /machine:X64 /out:\"./Something.exe\" ./Library.mock.a ./File.mock.obj",
				new List<Path>()
				{
					new Path("Library.mock.a"),
					new Path("File.mock.obj"),
				},
				new List<Path>()
				{
					new Path("C:/target/Something.exe"),
				});

			Assert.Equal(expected, result);
		}
	}
}
