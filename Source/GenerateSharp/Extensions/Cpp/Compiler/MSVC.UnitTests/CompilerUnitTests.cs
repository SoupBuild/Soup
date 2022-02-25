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
		public void Compile_Module_Partition()
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
					"DEBUG",
				},
				InterfacePartitionUnits = new List<InterfaceUnitCompileArguments>()
				{
					new InterfaceUnitCompileArguments()
					{
						SourceFile = new Path("File.cpp"),
						TargetFile = new Path("obj/File.obj"),
						ModuleInterfaceTarget = new Path("obj/File.pcm"),
						IncludeModules = new List<Path>()
						{
							new Path("obj/Other.pcm"),
						},
					},
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
					"./File.cpp",
					new Path("C:/source/"),
					new Path("C:/bin/mock.cl.exe"),
					"@C:/target/ObjectDir/SharedCompileArguments.rsp /reference \"./obj/Other.pcm\" ./File.cpp /Fo\"C:/target/obj/File.obj\" /interface /ifcOutput \"C:/target/obj/File.pcm\"",
					new List<Path>()
					{
						new Path("Module.pcm"),
						new Path("File.cpp"),
						new Path("C:/target/ObjectDir/SharedCompileArguments.rsp"),
						new Path("obj/Other.pcm"),
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
		public void Compile_Module_Interface()
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
					"DEBUG",
				},
			};

			arguments.InterfaceUnit = new InterfaceUnitCompileArguments()
			{
				SourceFile = new Path("File.cpp"),
				TargetFile = new Path("obj/File.obj"),
				ModuleInterfaceTarget = new Path("obj/File.pcm"),
				IncludeModules = new List<Path>()
				{
					new Path("obj/Other.pcm")
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
					"./File.cpp",
					new Path("C:/source/"),
					new Path("C:/bin/mock.cl.exe"),
					"@C:/target/ObjectDir/SharedCompileArguments.rsp /reference \"./obj/Other.pcm\" ./File.cpp /Fo\"C:/target/obj/File.obj\" /interface /ifcOutput \"C:/target/obj/File.pcm\"",
					new List<Path>()
					{
						new Path("Module.pcm"),
						new Path("File.cpp"),
						new Path("C:/target/ObjectDir/SharedCompileArguments.rsp"),
						new Path("obj/Other.pcm"),
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
		public void Compile_Module_PartitionInterfaceAndImplementation()
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
					"DEBUG",
				},
				InterfacePartitionUnits = new List<InterfaceUnitCompileArguments>()
				{
					new InterfaceUnitCompileArguments()
					{
						SourceFile = new Path("File1.cpp"),
						TargetFile = new Path("obj/File1.obj"),
						ModuleInterfaceTarget = new Path("obj/File1.pcm"),
						IncludeModules = new List<Path>()
						{
							new Path("obj/Other1.pcm")
						},
					},
				},
				InterfaceUnit = new InterfaceUnitCompileArguments()
				{
					SourceFile = new Path("File2.cpp"),
					TargetFile = new Path("obj/File2.obj"),
					ModuleInterfaceTarget = new Path("obj/File2.pcm"),
					IncludeModules = new List<Path>()
					{
						new Path("obj/Other2.pcm")
					},
				},
				ImplementationUnits = new List<TranslationUnitCompileArguments>()
				{
					new TranslationUnitCompileArguments()
					{
						SourceFile = new Path("File3.cpp"),
						TargetFile = new Path("obj/File3.obj"),
						IncludeModules = new List<Path>()
						{
							new Path("obj/Other3.pcm")
						},
					},
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
					"./File1.cpp",
					new Path("C:/source/"),
					new Path("C:/bin/mock.cl.exe"),
					"@C:/target/ObjectDir/SharedCompileArguments.rsp /reference \"./obj/Other1.pcm\" ./File1.cpp /Fo\"C:/target/obj/File1.obj\" /interface /ifcOutput \"C:/target/obj/File1.pcm\"",
					new List<Path>()
					{
						new Path("Module.pcm"),
						new Path("File1.cpp"),
						new Path("C:/target/ObjectDir/SharedCompileArguments.rsp"),
						new Path("obj/Other1.pcm"),
					},
					new List<Path>()
					{
						new Path("C:/target/obj/File1.obj"),
						new Path("C:/target/obj/File1.pcm"),
					}),
				new BuildOperation(
					"./File2.cpp",
					new Path("C:/source/"),
					new Path("C:/bin/mock.cl.exe"),
					"@C:/target/ObjectDir/SharedCompileArguments.rsp /reference \"./obj/Other2.pcm\" ./File2.cpp /Fo\"C:/target/obj/File2.obj\" /interface /ifcOutput \"C:/target/obj/File2.pcm\"",
					new List<Path>()
					{
						new Path("Module.pcm"),
						new Path("File2.cpp"),
						new Path("C:/target/ObjectDir/SharedCompileArguments.rsp"),
						new Path("obj/Other2.pcm"),
					},
					new List<Path>()
					{
						new Path("C:/target/obj/File2.obj"),
						new Path("C:/target/obj/File2.pcm"),
					}),
				new BuildOperation(
					"./File3.cpp",
					new Path("C:/source/"),
					new Path("C:/bin/mock.cl.exe"),
					"@C:/target/ObjectDir/SharedCompileArguments.rsp /reference \"./obj/Other3.pcm\" /reference \"C:/target/obj/File1.pcm\" /reference \"C:/target/obj/File2.pcm\" ./File3.cpp /Fo\"C:/target/obj/File3.obj\"",
					new List<Path>()
					{
						new Path("Module.pcm"),
						new Path("File3.cpp"),
						new Path("C:/target/ObjectDir/SharedCompileArguments.rsp"),
						new Path("obj/Other3.pcm"),
						new Path("C:/target/obj/File1.pcm"),
						new Path("C:/target/obj/File2.pcm"),
					},
					new List<Path>()
					{
						new Path("C:/target/obj/File3.obj"),
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
