
using Opal;
using Opal.System;
using Soup.Build.Runtime;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Cpp.Compiler.UnitTests
{
	public class BuildEngineUnitTests
	{
		[Fact]
		public void Initialize_Success()
		{
			var compiler = new Mock.Compiler();
			var uut = new BuildEngine(compiler);
		}

		[Fact]
		public void Build_WindowsApplication()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			// Register the test listener
			var testListener = new TestTraceListener();
			using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			{
				// Register the mock compiler
				var compiler = new Mock.Compiler();

				// Register the test process manager

				// Setup the build arguments
				var arguments = new BuildArguments();
				arguments.TargetName = "Program";
				arguments.TargetType = BuildTargetType.WindowsApplication;
				arguments.LanguageStandard = LanguageStandard.CPP20;
				arguments.SourceRootDirectory = new Path("C:/source/");
				arguments.TargetRootDirectory = new Path("C:/target/");
				arguments.ObjectDirectory = new Path("obj/");
				arguments.BinaryDirectory = new Path("bin/");
				arguments.SourceFiles = new List<Path>()
				{
					new Path("TestFile.cpp"),
				};
				arguments.OptimizationLevel = BuildOptimizationLevel.None;
				arguments.LinkDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};

				var uut = new BuildEngine(compiler);
				var fileSystemState = new FileSystemState();
				var readAccessList = new List<Path>();
				var writeAccessList = new List<Path>();
				var buildState = new BuildState(new ValueTable(), fileSystemState, readAccessList, writeAccessList);
				var result = uut.Execute(buildState, arguments);

				// Verify expected process manager requests
				Assert.Equal(
					new List<string>()
					{
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
					},
					processManager.GetRequests());

				// Verify expected logs
				Assert.Equal(
					new List<string>()
					{
						"INFO: Generate Compile Operation: ./TestFile.cpp",
						"INFO: CoreLink",
						"INFO: Linking target",
						"INFO: Generate Link Operation: ./bin/Program.exe",
					},
					testListener.GetMessages());

				var expectedCompileArguments = new SharedCompileArguments()
				{
					Standard = LanguageStandard.CPP20,
					Optimize = OptimizationLevel.None,
					ObjectDirectory = new Path("obj/"),
					SourceRootDirectory = new Path("C:/source/"),
					TargetRootDirectory = new Path("C:/target/"),
				};

				var expectedTranslationUnitArguments = new TranslationUnitCompileArguments()
				{
					SourceFile = new Path("TestFile.cpp"),
					TargetFile = new Path("obj/TestFile.mock.obj"),
				};
				expectedCompileArguments.ImplementationUnits = new List<TranslationUnitCompileArguments>()
				{
					expectedTranslationUnitArguments,
				};

				var expectedLinkArguments = new LinkArguments();
				expectedLinkArguments.TargetType = LinkTarget.WindowsApplication;
				expectedLinkArguments.TargetFile = new Path("bin/Program.exe");
				expectedLinkArguments.TargetRootDirectory = new Path("C:/target/");
				expectedLinkArguments.ObjectFiles = new List<Path>()
				{
					new Path("obj/TestFile.mock.obj"),
				};
				expectedLinkArguments.LibraryFiles = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};

				// Verify expected compiler calls
				Assert.Equal(
					new List<SharedCompileArguments>()
					{
						expectedCompileArguments,
					},
					compiler.GetCompileRequests());
				Assert.Equal(
					new List<LinkArguments>()
					{
						expectedLinkArguments,
					},
					compiler.GetLinkRequests());

				var expectedBuildOperations = new List<BuildOperation>()
				{
					new BuildOperation(
						"MakeDir [./obj/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./obj/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./obj/"),
						}),
					new BuildOperation(
						"MakeDir [./bin/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./bin/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./bin/"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile.mock.obj"),
						}),
					new BuildOperation(
						"MockLink: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockLinker.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("InputFile.in"),
						},
						new List<Path>()
						{
							new Path("OutputFile.out"),
						}),
				};

				Assert.Equal(
					expectedBuildOperations,
					result.BuildOperations);

				Assert.Equal(
					new List<Path>(),
					result.ModuleDependencies);

				Assert.Equal(
					new List<Path>(),
					result.LinkDependencies);

				Assert.Equal(
					new List<Path>()
					{
						new Path("C:/target/bin/Program.exe"),
					},
					result.RuntimeDependencies);
			}
		}

		[Fact]
		public void Build_WindowsApplicationWithResource()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			// Register the test listener
			var testListener = new TestTraceListener();
			using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			{
				// Register the mock compiler
				var compiler = new Mock.Compiler();

				// Register the test process manager

				// Setup the build arguments
				var arguments = new BuildArguments();
				arguments.TargetName = "Program";
				arguments.TargetType = BuildTargetType.WindowsApplication;
				arguments.LanguageStandard = LanguageStandard.CPP20;
				arguments.SourceRootDirectory = new Path("C:/source/");
				arguments.TargetRootDirectory = new Path("C:/target/");
				arguments.ObjectDirectory = new Path("obj/");
				arguments.BinaryDirectory = new Path("bin/");
				arguments.SourceFiles = new List<Path>()
				{
					new Path("TestFile.cpp"),
				};
				arguments.ResourceFile = new Path("Resources.rc");
				arguments.OptimizationLevel = BuildOptimizationLevel.None;
				arguments.LinkDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};

				var uut = new BuildEngine(compiler);
				var fileSystemState = new FileSystemState();
				var readAccessList = new List<Path>();
				var writeAccessList = new List<Path>();
				var buildState = new BuildState(new ValueTable(), fileSystemState, readAccessList, writeAccessList);
				var result = uut.Execute(buildState, arguments);

				// Verify expected process manager requests
				Assert.Equal(
					new List<string>()
					{
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
					},
					processManager.GetRequests());

				// Verify expected logs
				Assert.Equal(
					new List<string>()
					{
						"INFO: Generate Resource File Compile: ./Resources.rc",
						"INFO: Generate Compile Operation: ./TestFile.cpp",
						"INFO: CoreLink",
						"INFO: Linking target",
						"INFO: Generate Link Operation: ./bin/Program.exe",
					},
					testListener.GetMessages());

				var expectedCompileArguments = new SharedCompileArguments()
				{
					Standard = LanguageStandard.CPP20,
					Optimize = OptimizationLevel.None,
					ObjectDirectory = new Path("obj/"),
					SourceRootDirectory = new Path("C:/source/"),
					TargetRootDirectory = new Path("C:/target/"),
				};

				var expectedTranslationUnitArguments = new TranslationUnitCompileArguments()
				{
					SourceFile = new Path("TestFile.cpp"),
					TargetFile = new Path("obj/TestFile.mock.obj"),
				};
				expectedCompileArguments.ImplementationUnits = new List<TranslationUnitCompileArguments>()
				{
					expectedTranslationUnitArguments,
				};

				expectedCompileArguments.ResourceFile = new ResourceCompileArguments()
				{
					SourceFile = new Path("Resources.rc"),
					TargetFile = new Path("obj/Resources.mock.res"),
				};

				var expectedLinkArguments = new LinkArguments();
				expectedLinkArguments.TargetType = LinkTarget.WindowsApplication;
				expectedLinkArguments.TargetFile = new Path("bin/Program.exe");
				expectedLinkArguments.TargetRootDirectory = new Path("C:/target/");
				expectedLinkArguments.ObjectFiles = new List<Path>()
				{
					new Path("obj/Resources.mock.res"),
					new Path("obj/TestFile.mock.obj"),
				};
				expectedLinkArguments.LibraryFiles = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};

				// Verify expected compiler calls
				Assert.Equal(
					new List<SharedCompileArguments>()
					{
						expectedCompileArguments,
					},
					compiler.GetCompileRequests());
				Assert.Equal(
					new List<LinkArguments>()
					{
						expectedLinkArguments,
					},
					compiler.GetLinkRequests());

				var expectedBuildOperations = new List<BuildOperation>()
				{
					new BuildOperation(
						"MakeDir [./obj/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./obj/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./obj/"),
						}),
					new BuildOperation(
						"MakeDir [./bin/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./bin/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./bin/"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile.mock.obj"),
						}),
					new BuildOperation(
						"MockLink: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockLinker.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("InputFile.in"),
						},
						new List<Path>()
						{
							new Path("OutputFile.out"),
						}),
				};

				Assert.Equal(
					expectedBuildOperations,
					result.BuildOperations);

				Assert.Equal(
					new List<Path>(),
					result.ModuleDependencies);

				Assert.Equal(
					new List<Path>(),
					result.LinkDependencies);

				Assert.Equal(
					new List<Path>()
					{
						new Path("C:/target/bin/Program.exe"),
					},
					result.RuntimeDependencies);
			}
		}

		[Fact]
		public void Build_Executable()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			// Register the test listener
			var testListener = new TestTraceListener();
			using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			{
				// Register the mock compiler
				var compiler = new Mock.Compiler();

				// Register the test process manager

				// Setup the build arguments
				var arguments = new BuildArguments();
				arguments.TargetName = "Program";
				arguments.TargetType = BuildTargetType.Executable;
				arguments.LanguageStandard = LanguageStandard.CPP20;
				arguments.SourceRootDirectory = new Path("C:/source/");
				arguments.TargetRootDirectory = new Path("C:/target/");
				arguments.ObjectDirectory = new Path("obj/");
				arguments.BinaryDirectory = new Path("bin/");
				arguments.SourceFiles = new List<Path>()
				{
					new Path("TestFile.cpp"),
				};
				arguments.OptimizationLevel = BuildOptimizationLevel.None;
				arguments.LinkDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};

				var uut = new BuildEngine(compiler);
				var fileSystemState = new FileSystemState();
				var readAccessList = new List<Path>();
				var writeAccessList = new List<Path>();
				var buildState = new BuildState(new ValueTable(), fileSystemState, readAccessList, writeAccessList);
				var result = uut.Execute(buildState, arguments);

				// Verify expected process manager requests
				Assert.Equal(
					new List<string>()
					{
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
					},
					processManager.GetRequests());

				// Verify expected logs
				Assert.Equal(
					new List<string>()
					{
						"INFO: Generate Compile Operation: ./TestFile.cpp",
						"INFO: CoreLink",
						"INFO: Linking target",
						"INFO: Generate Link Operation: ./bin/Program.exe",
					},
					testListener.GetMessages());

				var expectedCompileArguments = new SharedCompileArguments()
				{
					Standard = LanguageStandard.CPP20,
					Optimize = OptimizationLevel.None,
					ObjectDirectory = new Path("obj/"),
					SourceRootDirectory = new Path("C:/source/"),
					TargetRootDirectory = new Path("C:/target/"),
				};

				var expectedTranslationUnitArguments = new TranslationUnitCompileArguments()
				{
					SourceFile = new Path("TestFile.cpp"),
					TargetFile = new Path("obj/TestFile.mock.obj"),
				};
				expectedCompileArguments.ImplementationUnits = new List<TranslationUnitCompileArguments>()
				{
					expectedTranslationUnitArguments,
				};

				var expectedLinkArguments = new LinkArguments();
				expectedLinkArguments.TargetType = LinkTarget.Executable;
				expectedLinkArguments.TargetFile = new Path("bin/Program.exe");
				expectedLinkArguments.TargetRootDirectory = new Path("C:/target/");
				expectedLinkArguments.ObjectFiles = new List<Path>()
				{
					new Path("obj/TestFile.mock.obj"),
				};
				expectedLinkArguments.LibraryFiles = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};

				// Verify expected compiler calls
				Assert.Equal(
					new List<SharedCompileArguments>()
					{
						expectedCompileArguments,
					},
					compiler.GetCompileRequests());
				Assert.Equal(
					new List<LinkArguments>()
					{
						expectedLinkArguments,
					},
					compiler.GetLinkRequests());

				var expectedBuildOperations = new List<BuildOperation>()
				{
					new BuildOperation(
						"MakeDir [./obj/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./obj/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./obj/"),
						}),
					new BuildOperation(
						"MakeDir [./bin/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./bin/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./bin/"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile.mock.obj"),
						}),
					new BuildOperation(
						"MockLink: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockLinker.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("InputFile.in"),
						},
						new List<Path>()
						{
							new Path("OutputFile.out"),
						}),
				};

				Assert.Equal(
					expectedBuildOperations,
					result.BuildOperations);

				Assert.Equal(
					new List<Path>(),
					result.ModuleDependencies);

				Assert.Equal(
					new List<Path>(),
					result.LinkDependencies);

				Assert.Equal(
					new List<Path>()
					{
						new Path("C:/target/bin/Program.exe"),
					},
					result.RuntimeDependencies);
			}
		}

		[Fact]
		public void Build_Library_MultipleFiles()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			// Register the test listener
			var testListener = new TestTraceListener();
			using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			{
				// Register the mock compiler
				var compiler = new Mock.Compiler();

				// Setup the build arguments
				var arguments = new BuildArguments();
				arguments.TargetName = "Library";
				arguments.TargetType = BuildTargetType.StaticLibrary;
				arguments.LanguageStandard = LanguageStandard.CPP20;
				arguments.SourceRootDirectory = new Path("C:/source/");
				arguments.TargetRootDirectory = new Path("C:/target/");
				arguments.ObjectDirectory = new Path("obj/");
				arguments.BinaryDirectory = new Path("bin/");
				arguments.SourceFiles = new List<Path>()
				{
					new Path("TestFile1.cpp"),
					new Path("TestFile2.cpp"),
					new Path("TestFile3.cpp"),
				};
				arguments.OptimizationLevel = BuildOptimizationLevel.Size;
				arguments.IncludeDirectories = new List<Path>()
				{
					new Path("Folder"),
					new Path("AnotherFolder/Sub"),
				};
				arguments.ModuleDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.bmi"),
					new Path("../OtherModule2.mock.bmi"),
				};
				arguments.LinkDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};

				var uut = new BuildEngine(compiler);
				var fileSystemState = new FileSystemState();
				var readAccessList = new List<Path>();
				var writeAccessList = new List<Path>();
				var buildState = new BuildState(new ValueTable(), fileSystemState, readAccessList, writeAccessList);
				var result = uut.Execute(buildState, arguments);

				// Verify expected process manager requests
				Assert.Equal(
					new List<string>()
					{
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
					},
					processManager.GetRequests());

				// Verify expected logs
				Assert.Equal(
					new List<string>()
					{
						"INFO: Generate Compile Operation: ./TestFile1.cpp",
						"INFO: Generate Compile Operation: ./TestFile2.cpp",
						"INFO: Generate Compile Operation: ./TestFile3.cpp",
						"INFO: CoreLink",
						"INFO: Linking target",
						"INFO: Generate Link Operation: ./bin/Library.mock.lib",
					},
					testListener.GetMessages());

				// Setup the shared arguments
				var expectedCompileArguments = new SharedCompileArguments()
				{
					Standard = LanguageStandard.CPP20,
					Optimize = OptimizationLevel.Size,
					SourceRootDirectory = new Path("C:/source/"),
					TargetRootDirectory = new Path("C:/target/"),
					ObjectDirectory = new Path("obj/"),
					IncludeDirectories = new List<Path>()
					{
						new Path("Folder"),
						new Path("AnotherFolder/Sub"),
					},
					IncludeModules = new List<Path>()
					{
						new Path("../Other/bin/OtherModule1.mock.bmi"),
						new Path("../OtherModule2.mock.bmi"),
					},
				};

				var expectedTranslationUnit1Arguments = new TranslationUnitCompileArguments();
				expectedTranslationUnit1Arguments.SourceFile = new Path("TestFile1.cpp");
				expectedTranslationUnit1Arguments.TargetFile = new Path("obj/TestFile1.mock.obj");

				var expectedTranslationUnit2Arguments = new TranslationUnitCompileArguments();
				expectedTranslationUnit2Arguments.SourceFile = new Path("TestFile2.cpp");
				expectedTranslationUnit2Arguments.TargetFile = new Path("obj/TestFile2.mock.obj");

				var expectedTranslationUnit3Arguments = new TranslationUnitCompileArguments();
				expectedTranslationUnit3Arguments.SourceFile = new Path("TestFile3.cpp");
				expectedTranslationUnit3Arguments.TargetFile = new Path("obj/TestFile3.mock.obj");

				expectedCompileArguments.ImplementationUnits = new List<TranslationUnitCompileArguments>()
				{
					expectedTranslationUnit1Arguments,
					expectedTranslationUnit2Arguments,
					expectedTranslationUnit3Arguments,
				};

				var expectedLinkArguments = new LinkArguments();
				expectedLinkArguments.TargetFile = new Path("bin/Library.mock.lib");
				expectedLinkArguments.TargetType = LinkTarget.StaticLibrary;
				expectedLinkArguments.TargetRootDirectory = new Path("C:/target/");
				expectedLinkArguments.ObjectFiles = new List<Path>()
				{
					new Path("obj/TestFile1.mock.obj"),
					new Path("obj/TestFile2.mock.obj"),
					new Path("obj/TestFile3.mock.obj"),
				};

				// Note: There is no need to send along the static libraries for a static library linking
				expectedLinkArguments.LibraryFiles = new List<Path>();

				// Verify expected compiler calls
				Assert.Equal(
					new List<SharedCompileArguments>()
					{
						expectedCompileArguments,
					},
					compiler.GetCompileRequests());
				Assert.Equal(
					new List<LinkArguments>()
					{
						expectedLinkArguments,
					},
					compiler.GetLinkRequests());

				// Verify build state
				var expectedBuildOperations = new List<BuildOperation>()
				{
					new BuildOperation(
						"MakeDir [./obj/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./obj/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./obj/"),
						}),
					new BuildOperation(
						"MakeDir [./bin/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./bin/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("./bin/"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile1.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile1.mock.obj"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile2.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile2.mock.obj"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile3.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile3.mock.obj"),
						}),
					new BuildOperation(
						"MockLink: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockLinker.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("InputFile.in"),
						},
						new List<Path>()
						{
							new Path("OutputFile.out"),
						}),
				};

				Assert.Equal(
					expectedBuildOperations,
					result.BuildOperations);

				Assert.Equal(
					new List<Path>(),
					result.ModuleDependencies);

				Assert.Equal(
					new List<Path>()
					{
						new Path("../Other/bin/OtherModule1.mock.a"),
						new Path("../OtherModule2.mock.a"),
						new Path("C:/target/bin/Library.mock.lib"),
					},
					result.LinkDependencies);

				Assert.Equal(
					new List<Path>(),
					result.RuntimeDependencies);

				Assert.Equal(
					new Path(),
					result.TargetFile);
			}
		}

		[Fact]
		public void Build_Library_ModuleInterface()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			// Register the test listener
			var testListener = new TestTraceListener();
			using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			{
				// Register the mock compiler
				var compiler = new Mock.Compiler();

				// Setup the build arguments
				var arguments = new BuildArguments();
				arguments.TargetName = "Library";
				arguments.TargetType = BuildTargetType.StaticLibrary;
				arguments.LanguageStandard = LanguageStandard.CPP20;
				arguments.SourceRootDirectory = new Path("C:/source/");
				arguments.TargetRootDirectory = new Path("C:/target/");
				arguments.ObjectDirectory = new Path("obj/");
				arguments.BinaryDirectory = new Path("bin/");
				arguments.ModuleInterfaceSourceFile = new Path("Public.cpp");
				arguments.SourceFiles = new List<Path>()
				{
					new Path("TestFile1.cpp"),
					new Path("TestFile2.cpp"),
					new Path("TestFile3.cpp"),
				};
				arguments.OptimizationLevel = BuildOptimizationLevel.None;
				arguments.IncludeDirectories = new List<Path>()
				{
					new Path("Folder"),
					new Path("AnotherFolder/Sub"),
				};
				arguments.ModuleDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.bmi"),
					new Path("../OtherModule2.mock.bmi"),
				};
				arguments.LinkDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};
				arguments.PreprocessorDefinitions = new List<string>()
				{
					"DEBUG",
					"AWESOME",
				};

				var uut = new BuildEngine(compiler);
				var fileSystemState = new FileSystemState();
				var readAccessList = new List<Path>();
				var writeAccessList = new List<Path>();
				var buildState = new BuildState(new ValueTable(), fileSystemState, readAccessList, writeAccessList);
				var result = uut.Execute(buildState, arguments);

				// Verify expected process manager requests
				Assert.Equal(
					new List<string>()
					{
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
					},
					processManager.GetRequests());

				// Verify expected logs
				Assert.Equal(
					new List<string>()
					{
						"INFO: Generate Module Interface Unit Compile: ./Public.cpp",
						"INFO: Generate Compile Operation: ./TestFile1.cpp",
						"INFO: Generate Compile Operation: ./TestFile2.cpp",
						"INFO: Generate Compile Operation: ./TestFile3.cpp",
						"INFO: CoreLink",
						"INFO: Linking target",
						"INFO: Generate Link Operation: ./bin/Library.mock.lib",
					},
					testListener.GetMessages());

				// Setup the shared arguments
				var expectedCompileArguments = new SharedCompileArguments()
				{
					Standard = LanguageStandard.CPP20,
					Optimize = OptimizationLevel.None,
					SourceRootDirectory = new Path("C:/source/"),
					TargetRootDirectory = new Path("C:/target/"),
					ObjectDirectory = new Path("obj/"),
					IncludeDirectories = new List<Path>()
					{
						new Path("Folder"),
						new Path("AnotherFolder/Sub"),
					},
					IncludeModules = new List<Path>()
					{
						new Path("../Other/bin/OtherModule1.mock.bmi"),
						new Path("../OtherModule2.mock.bmi"),
					},
					PreprocessorDefinitions = new List<string>()
					{
						"DEBUG",
						"AWESOME",
					},
				};

				var expectedCompileModuleArguments = new InterfaceUnitCompileArguments()
				{
					SourceFile = new Path("Public.cpp"),
					TargetFile = new Path("obj/Public.mock.obj"),
					ModuleInterfaceTarget = new Path("obj/Public.mock.bmi"),
				};
				expectedCompileArguments.InterfaceUnit = expectedCompileModuleArguments;

				var expectedTranslationUnit1Arguments = new TranslationUnitCompileArguments();
				expectedTranslationUnit1Arguments.SourceFile = new Path("TestFile1.cpp");
				expectedTranslationUnit1Arguments.TargetFile = new Path("obj/TestFile1.mock.obj");

				var expectedTranslationUnit2Arguments = new TranslationUnitCompileArguments();
				expectedTranslationUnit2Arguments.SourceFile = new Path("TestFile2.cpp");
				expectedTranslationUnit2Arguments.TargetFile = new Path("obj/TestFile2.mock.obj");

				var expectedTranslationUnit3Arguments = new TranslationUnitCompileArguments();
				expectedTranslationUnit3Arguments.SourceFile = new Path("TestFile3.cpp");
				expectedTranslationUnit3Arguments.TargetFile = new Path("obj/TestFile3.mock.obj");

				expectedCompileArguments.ImplementationUnits = new List<TranslationUnitCompileArguments>()
				{
					expectedTranslationUnit1Arguments,
					expectedTranslationUnit2Arguments,
					expectedTranslationUnit3Arguments,
				};

				var expectedLinkArguments = new LinkArguments();
				expectedLinkArguments.TargetFile = new Path("bin/Library.mock.lib");
				expectedLinkArguments.TargetType = LinkTarget.StaticLibrary;
				expectedLinkArguments.TargetRootDirectory = new Path("C:/target/");
				expectedLinkArguments.ObjectFiles = new List<Path>()
				{
					new Path("obj/Public.mock.obj"),
					new Path("obj/TestFile1.mock.obj"),
					new Path("obj/TestFile2.mock.obj"),
					new Path("obj/TestFile3.mock.obj"),
				};
				expectedLinkArguments.LibraryFiles = new List<Path>();

				// Verify expected compiler calls
				Assert.Equal(
					new List<SharedCompileArguments>()
					{
						expectedCompileArguments,
					},
					compiler.GetCompileRequests());
				Assert.Equal(
					new List<LinkArguments>()
					{
						expectedLinkArguments,
					},
					compiler.GetLinkRequests());

				// Verify build state
				var expectedBuildOperations = new List<BuildOperation>()
				{
					new BuildOperation(
						"MakeDir [./obj/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./obj/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("obj/"),
						}),
					new BuildOperation(
						"MakeDir [./bin/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./bin/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("bin/"),
						}),
					new BuildOperation(
						"Copy [./obj/Public.mock.bmi] -> [./bin/Library.mock.bmi]",
						new Path("C:/target/"),
						new Path("C:/copy.exe"),
						"\"./obj/Public.mock.bmi\" \"./bin/Library.mock.bmi\"",
						new List<Path>()
						{
							new Path("obj/Public.mock.bmi"),
						},
						new List<Path>()
						{
							new Path("bin/Library.mock.bmi"),
						}),
					new BuildOperation(
						"MockCompileModule: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("Public.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/Public.mock.obj"),
							new Path("obj/Public.mock.bmi"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile1.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile1.mock.obj"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile2.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile2.mock.obj"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile3.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile3.mock.obj"),
						}),
					new BuildOperation(
						"MockLink: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockLinker.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("InputFile.in"),
						},
						new List<Path>()
						{
							new Path("OutputFile.out"),
						}),
				};

				Assert.Equal(
					expectedBuildOperations,
					result.BuildOperations);

				Assert.Equal(
					new List<Path>()
					{
					new Path("C:/target/bin/Library.mock.bmi"),
					},
					result.ModuleDependencies);

				Assert.Equal(
					new List<Path>()
					{
						new Path("../Other/bin/OtherModule1.mock.a"),
						new Path("../OtherModule2.mock.a"),
						new Path("C:/target/bin/Library.mock.lib"),
					},
					result.LinkDependencies);

				Assert.Equal(
					new List<Path>(),
					result.RuntimeDependencies);

				Assert.Equal(
					new Path(),
					result.TargetFile);
			}
		}

		[Fact]
		public void Build_Library_ModuleInterface_WithPartitions()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			// Register the test listener
			var testListener = new TestTraceListener();
			using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			{
				// Register the mock compiler
				var compiler = new Mock.Compiler();

				// Setup the build arguments
				var arguments = new BuildArguments();
				arguments.TargetName = "Library";
				arguments.TargetType = BuildTargetType.StaticLibrary;
				arguments.LanguageStandard = LanguageStandard.CPP20;
				arguments.SourceRootDirectory = new Path("C:/source/");
				arguments.TargetRootDirectory = new Path("C:/target/");
				arguments.ObjectDirectory = new Path("obj/");
				arguments.BinaryDirectory = new Path("bin/");
				arguments.ModuleInterfacePartitionSourceFiles = new List<Path>()
				{
					new Path("TestFile1.cpp"),
					new Path("TestFile2.cpp"),
				};
				arguments.ModuleInterfaceSourceFile = new Path("Public.cpp");
				arguments.SourceFiles = new List<Path>()
				{
					new Path("TestFile3.cpp"),
					new Path("TestFile4.cpp"),
				};
				arguments.OptimizationLevel = BuildOptimizationLevel.None;
				arguments.IncludeDirectories = new List<Path>()
				{
					new Path("Folder"),
					new Path("AnotherFolder/Sub"),
				};
				arguments.ModuleDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.bmi"),
					new Path("../OtherModule2.mock.bmi"),
				};
				arguments.LinkDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};
				arguments.PreprocessorDefinitions = new List<string>()
				{
					"DEBUG",
					"AWESOME",
				};

				var uut = new BuildEngine(compiler);
				var fileSystemState = new FileSystemState();
				var readAccessList = new List<Path>();
				var writeAccessList = new List<Path>();
				var buildState = new BuildState(new ValueTable(), fileSystemState, readAccessList, writeAccessList);
				var result = uut.Execute(buildState, arguments);

				// Verify expected process manager requests
				Assert.Equal(
					new List<string>()
					{
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
					},
					processManager.GetRequests());

				// Verify expected logs
				Assert.Equal(
					new List<string>()
					{
						"INFO: Generate Module Interface Partition Compile Operation: ./TestFile1.cpp",
						"INFO: Generate Module Interface Partition Compile Operation: ./TestFile2.cpp",
						"INFO: Generate Module Interface Unit Compile: ./Public.cpp",
						"INFO: Generate Compile Operation: ./TestFile3.cpp",
						"INFO: Generate Compile Operation: ./TestFile4.cpp",
						"INFO: CoreLink",
						"INFO: Linking target",
						"INFO: Generate Link Operation: ./bin/Library.mock.lib",
					},
					testListener.GetMessages());

				// Setup the shared arguments
				var expectedCompileArguments = new SharedCompileArguments()
				{
					Standard = LanguageStandard.CPP20,
					Optimize = OptimizationLevel.None,
					SourceRootDirectory = new Path("C:/source/"),
					TargetRootDirectory = new Path("C:/target/"),
					ObjectDirectory = new Path("obj/"),
					IncludeDirectories = new List<Path>()
					{
						new Path("Folder"),
						new Path("AnotherFolder/Sub"),
					},
					IncludeModules = new List<Path>()
					{
						new Path("../Other/bin/OtherModule1.mock.bmi"),
						new Path("../OtherModule2.mock.bmi"),
					},
					PreprocessorDefinitions = new List<string>()
					{
						"DEBUG",
						"AWESOME",
					},
					InterfacePartitionUnits = new List<InterfaceUnitCompileArguments>()
					{
						new InterfaceUnitCompileArguments()
						{
							SourceFile = new Path("TestFile1.cpp"),
							TargetFile = new Path("obj/TestFile1.mock.obj"),
							ModuleInterfaceTarget = new Path("obj/TestFile1.mock.bmi"),
						},
						new InterfaceUnitCompileArguments()
						{
							SourceFile = new Path("TestFile2.cpp"),
							TargetFile = new Path("obj/TestFile2.mock.obj"),
							ModuleInterfaceTarget = new Path("obj/TestFile2.mock.bmi"),
						},
					},
					InterfaceUnit = new InterfaceUnitCompileArguments()
					{
						SourceFile = new Path("Public.cpp"),
						TargetFile = new Path("obj/Public.mock.obj"),
						IncludeModules = new List<Path>()
						{
							new Path("C:/target/obj/TestFile1.mock.bmi"),
							new Path("C:/target/obj/TestFile2.mock.bmi"),
						},
						ModuleInterfaceTarget = new Path("obj/Public.mock.bmi"),
					},
					ImplementationUnits = new List<TranslationUnitCompileArguments>()
					{
						new TranslationUnitCompileArguments()
						{
							SourceFile = new Path("TestFile3.cpp"),
							TargetFile = new Path("obj/TestFile3.mock.obj"),
						},
						new TranslationUnitCompileArguments()
						{
							SourceFile = new Path("TestFile4.cpp"),
							TargetFile = new Path("obj/TestFile4.mock.obj"),
						},
					},
				};

				var expectedLinkArguments = new LinkArguments()
				{
					TargetFile = new Path("bin/Library.mock.lib"),
					TargetType = LinkTarget.StaticLibrary,
					TargetRootDirectory = new Path("C:/target/"),
					ObjectFiles = new List<Path>()
					{
						new Path("obj/TestFile1.mock.obj"),
						new Path("obj/TestFile2.mock.obj"),
						new Path("obj/Public.mock.obj"),
						new Path("obj/TestFile3.mock.obj"),
						new Path("obj/TestFile4.mock.obj"),
					},
					LibraryFiles = new List<Path>(),
				};

				// Verify expected compiler calls
				Assert.Equal(
					new List<SharedCompileArguments>()
					{
						expectedCompileArguments,
					},
					compiler.GetCompileRequests());
				Assert.Equal(
					new List<LinkArguments>()
					{
						expectedLinkArguments,
					},
					compiler.GetLinkRequests());

				// Verify build state
				var expectedBuildOperations = new List<BuildOperation>()
				{
					new BuildOperation(
						"MakeDir [./obj/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./obj/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("obj/"),
						}),
					new BuildOperation(
						"MakeDir [./bin/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./bin/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("bin/"),
						}),
					new BuildOperation(
						"Copy [./obj/Public.mock.bmi] -> [./bin/Library.mock.bmi]",
						new Path("C:/target/"),
						new Path("C:/copy.exe"),
						"\"./obj/Public.mock.bmi\" \"./bin/Library.mock.bmi\"",
						new List<Path>()
						{
							new Path("obj/Public.mock.bmi"),
						},
						new List<Path>()
						{
							new Path("bin/Library.mock.bmi"),
						}),
					new BuildOperation(
						"MockCompilePartition: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile1.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile1.mock.obj"),
							new Path("obj/TestFile1.mock.bmi"),
						}),
					new BuildOperation(
						"MockCompilePartition: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile2.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile2.mock.obj"),
							new Path("obj/TestFile2.mock.bmi"),
						}),
					new BuildOperation(
						"MockCompileModule: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("Public.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/Public.mock.obj"),
							new Path("obj/Public.mock.bmi"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile3.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile3.mock.obj"),
						}),
					new BuildOperation(
						"MockCompile: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("TestFile4.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/TestFile4.mock.obj"),
						}),
					new BuildOperation(
						"MockLink: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockLinker.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("InputFile.in"),
						},
						new List<Path>()
						{
							new Path("OutputFile.out"),
						}),
				};

				Assert.Equal(
					expectedBuildOperations,
					result.BuildOperations);

				Assert.Equal(
					new List<Path>()
					{
						new Path("C:/target/bin/Library.mock.bmi"),
					},
					result.ModuleDependencies);

				Assert.Equal(
					new List<Path>()
					{
						new Path("../Other/bin/OtherModule1.mock.a"),
						new Path("../OtherModule2.mock.a"),
						new Path("C:/target/bin/Library.mock.lib"),
					},
					result.LinkDependencies);

				Assert.Equal(
					new List<Path>(),
					result.RuntimeDependencies);

				Assert.Equal(
					new Path(),
					result.TargetFile);
			}
		}

		[Fact]
		public void Build_Library_ModuleInterfaceNoSource()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			// Register the test listener
			var testListener = new TestTraceListener();
			using (var scopedTraceListener = new ScopedTraceListenerRegister(testListener))
			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			{
				// Register the mock compiler
				var compiler = new Mock.Compiler();

				// Setup the build arguments
				var arguments = new BuildArguments();
				arguments.TargetName = "Library";
				arguments.TargetType = BuildTargetType.StaticLibrary;
				arguments.LanguageStandard = LanguageStandard.CPP20;
				arguments.SourceRootDirectory = new Path("C:/source/");
				arguments.TargetRootDirectory = new Path("C:/target/");
				arguments.ObjectDirectory = new Path("obj/");
				arguments.BinaryDirectory = new Path("bin/");
				arguments.ModuleInterfaceSourceFile = new Path("Public.cpp");
				arguments.SourceFiles = new List<Path>();
				arguments.OptimizationLevel = BuildOptimizationLevel.Size;
				arguments.IncludeDirectories = new List<Path>()
				{
					new Path("Folder"),
					new Path("AnotherFolder/Sub"),
				};
				arguments.ModuleDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.bmi"),
					new Path("../OtherModule2.mock.bmi"),
				};
				arguments.LinkDependencies = new List<Path>()
				{
					new Path("../Other/bin/OtherModule1.mock.a"),
					new Path("../OtherModule2.mock.a"),
				};

				var uut = new BuildEngine(compiler);
				var fileSystemState = new FileSystemState();
				var readAccessList = new List<Path>();
				var writeAccessList = new List<Path>();
				var buildState = new BuildState(new ValueTable(), fileSystemState, readAccessList, writeAccessList);
				var result = uut.Execute(buildState, arguments);

				// Verify expected process manager requests
				Assert.Equal(
					new List<string>()
					{
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
						"GetCurrentProcessFileName",
					},
					processManager.GetRequests());

				// Verify expected logs
				Assert.Equal(
					new List<string>()
					{
						"INFO: Generate Module Interface Unit Compile: ./Public.cpp",
						"INFO: CoreLink",
						"INFO: Linking target",
						"INFO: Generate Link Operation: ./bin/Library.mock.lib",
					},
					testListener.GetMessages());

				// Setup the shared arguments
				var expectedCompileArguments = new SharedCompileArguments()
				{
					Standard = LanguageStandard.CPP20,
					Optimize = OptimizationLevel.Size,
					SourceRootDirectory = new Path("C:/source/"),
					TargetRootDirectory = new Path("C:/target/"),
					ObjectDirectory = new Path("obj/"),
					IncludeDirectories = new List<Path>()
					{
						new Path("Folder"),
						new Path("AnotherFolder/Sub"),
					},
					IncludeModules = new List<Path>()
					{
						new Path("../Other/bin/OtherModule1.mock.bmi"),
						new Path("../OtherModule2.mock.bmi"),
					},
				};

				var expectedCompileModuleArguments = new InterfaceUnitCompileArguments()
				{
					SourceFile = new Path("Public.cpp"),
					TargetFile = new Path("obj/Public.mock.obj"),
					ModuleInterfaceTarget = new Path("obj/Public.mock.bmi"),
				};
				expectedCompileArguments.InterfaceUnit = expectedCompileModuleArguments;

				var expectedLinkArguments = new LinkArguments();
				expectedLinkArguments.TargetFile = new Path("bin/Library.mock.lib");
				expectedLinkArguments.TargetType = LinkTarget.StaticLibrary;
				expectedLinkArguments.TargetRootDirectory = new Path("C:/target/");
				expectedLinkArguments.ObjectFiles = new List<Path>()
				{
					new Path("obj/Public.mock.obj"),
				};
				expectedLinkArguments.LibraryFiles = new List<Path>();

				// Verify expected compiler calls
				Assert.Equal(
					new List<SharedCompileArguments>()
					{
						expectedCompileArguments,
					},
					compiler.GetCompileRequests());
				Assert.Equal(
					new List<LinkArguments>()
					{
						expectedLinkArguments,
					},
					compiler.GetLinkRequests());

				// Verify build state
				var expectedBuildOperations = new List<BuildOperation>()
				{
					new BuildOperation(
						"MakeDir [./obj/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./obj/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("obj/"),
						}),
					new BuildOperation(
						"MakeDir [./bin/]",
						new Path("C:/target/"),
						new Path("C:/mkdir.exe"),
						"\"./bin/\"",
						new List<Path>(),
						new List<Path>()
						{
							new Path("bin/"),
						}),
					new BuildOperation(
						"Copy [./obj/Public.mock.bmi] -> [./bin/Library.mock.bmi]",
						new Path("C:/target/"),
						new Path("C:/copy.exe"),
						"\"./obj/Public.mock.bmi\" \"./bin/Library.mock.bmi\"",
						new List<Path>()
						{
							new Path("obj/Public.mock.bmi"),
						},
						new List<Path>()
						{
							new Path("bin/Library.mock.bmi"),
						}),
					new BuildOperation(
						"MockCompileModule: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("Public.cpp"),
						},
						new List<Path>()
						{
							new Path("obj/Public.mock.obj"),
							new Path("obj/Public.mock.bmi"),
						}),
					new BuildOperation(
						"MockLink: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockLinker.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("InputFile.in"),
						},
						new List<Path>()
						{
							new Path("OutputFile.out"),
						}),
				};

				Assert.Equal(
					expectedBuildOperations,
					result.BuildOperations);

				Assert.Equal(
					new List<Path>()
					{
						new Path("C:/target/bin/Library.mock.bmi"),
					},
					result.ModuleDependencies);

				Assert.Equal(
					new List<Path>()
					{
						new Path("../Other/bin/OtherModule1.mock.a"),
						new Path("../OtherModule2.mock.a"),
						new Path("C:/target/bin/Library.mock.lib"),
					},
					result.LinkDependencies);

				Assert.Equal(
					new List<Path>(),
					result.RuntimeDependencies);

				Assert.Equal(
					new Path(),
					result.TargetFile);
			}
		}
	}
}