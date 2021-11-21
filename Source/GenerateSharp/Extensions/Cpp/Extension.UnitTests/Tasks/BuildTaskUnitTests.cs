// <copyright file="BuildTaskUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using Soup.Build.Cpp.Compiler;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Cpp.UnitTests
{
	public class BuildTaskUnitTests
	{
		[Fact]
		public void Initialize_Success()
		{
			// Register the test process manager
			var processManager = new MockProcessManager();

			using (var scopedProcesManager = new ScopedSingleton<IProcessManager>(processManager))
			{
				var buildState = new MockBuildState();
				var factory = new ValueFactory();
				var uut = new BuildTask(buildState, factory);
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
				// Setup the input build state
				var buildState = new MockBuildState();
				var state = buildState.ActiveState;

				// Setup build table
				var buildTable = new ValueTable();
				state.Add("Build", new Value(buildTable));
				buildTable.Add("TargetName", new Value("Program"));
				buildTable.Add("TargetType", new Value((long)BuildTargetType.Executable));
				buildTable.Add("LanguageStandard", new Value((long)LanguageStandard.CPP20));
				buildTable.Add("SourceRootDirectory", new Value("C:/source/"));
				buildTable.Add("TargetRootDirectory", new Value("C:/target/"));
				buildTable.Add("ObjectDirectory", new Value("obj/"));
				buildTable.Add("BinaryDirectory", new Value("bin/"));
				buildTable.Add(
					"Source",
					new Value(new ValueList()
						{
							new Value("TestFile.cpp"),
						}));

				// Setup parameters table
				var parametersTable = new ValueTable();
				state.Add("Parameters", new Value(parametersTable));
				parametersTable.Add("Architecture", new Value("x64"));
				parametersTable.Add("Compiler", new Value("MOCK"));

				// Register the mock compiler
				var compiler = new Compiler.Mock.Compiler();
				var compilerFactory = new Dictionary<string, Func<IValueTable, ICompiler>>();
				compilerFactory.Add("MOCK", (IValueTable state) => { return compiler; });

				var factory = new ValueFactory();
				var uut = new BuildTask(buildState, factory, compilerFactory);

				uut.Execute();

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
						"INFO: Build Generate Done",
					},
					testListener.GetMessages());

				var expectedCompileArguments = new SharedCompileArguments()
				{
					Standard = LanguageStandard.CPP20,
					Optimize = OptimizationLevel.None,
					SourceRootDirectory = new Path("C:/source/"),
					TargetRootDirectory = new Path("C:/target/"),
					ObjectDirectory = new Path("obj/"),
				};

				var expectedTranslationUnitArguments = new TranslationUnitCompileArguments();
				expectedTranslationUnitArguments.SourceFile = new Path("TestFile.cpp");
				expectedTranslationUnitArguments.TargetFile = new Path("obj/TestFile.mock.obj");

				expectedCompileArguments.ImplementationUnits = new List<TranslationUnitCompileArguments>()
				{
					expectedTranslationUnitArguments,
				};

				var expectedLinkArguments = new LinkArguments();
				expectedLinkArguments.TargetType = LinkTarget.Executable;
				expectedLinkArguments.TargetArchitecture = "x64";
				expectedLinkArguments.TargetFile = new Path("bin/Program.exe");
				expectedLinkArguments.TargetRootDirectory = new Path("C:/target/");
				expectedLinkArguments.ObjectFiles = new List<Path>()
				{
					new Path("obj/TestFile.mock.obj"),
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
					buildState.GetBuildOperations());
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
				// Setup the input build state
				var buildState = new MockBuildState();
				var state = buildState.ActiveState;

				// Setup build table
				var buildTable = new ValueTable();
				state.Add("Build", new Value(buildTable));
				buildTable.Add("TargetName", new Value("Library"));
				buildTable.Add("TargetType", new Value((long)BuildTargetType.StaticLibrary));
				buildTable.Add("LanguageStandard", new Value((long)LanguageStandard.CPP20));
				buildTable.Add("SourceRootDirectory", new Value("C:/source/"));
				buildTable.Add("TargetRootDirectory", new Value("C:/target/"));
				buildTable.Add("ObjectDirectory", new Value("obj/"));
				buildTable.Add("BinaryDirectory", new Value("bin/"));
				buildTable.Add("Source", new Value(new ValueList()
				{
					new Value("TestFile1.cpp"),
					new Value("TestFile2.cpp"),
					new Value("TestFile3.cpp"),
				}));
				buildTable.Add("IncludeDirectories", new Value(new ValueList()
				{
					new Value("Folder"),
					new Value("AnotherFolder/Sub"),
				}));
				buildTable.Add("ModuleDependencies", new Value(new ValueList()
				{
					new Value("../Other/bin/OtherModule1.mock.bmi"),
					new Value("../OtherModule2.mock.bmi"),
				}));
				buildTable.Add("OptimizationLevel", new Value((long)BuildOptimizationLevel.None));

				// Setup parameters table
				var parametersTable = new ValueTable();
				state.Add("Parameters", new Value(parametersTable));
				parametersTable.Add("Architecture", new Value("x64"));
				parametersTable.Add("Compiler", new Value("MOCK"));

				// Register the mock compiler
				var compiler = new Compiler.Mock.Compiler();
				var compilerFactory = new Dictionary<string, Func<IValueTable, ICompiler>>();
				compilerFactory.Add("MOCK", (IValueTable state) => { return compiler; });

				var factory = new ValueFactory();
				var uut = new BuildTask(buildState, factory, compilerFactory);

				uut.Execute();

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
						"INFO: Build Generate Done",
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
				};

				expectedCompileArguments.ImplementationUnits = new List<TranslationUnitCompileArguments>()
				{
					new TranslationUnitCompileArguments()
					{
						SourceFile = new Path("TestFile1.cpp"),
						TargetFile = new Path("obj/TestFile1.mock.obj"),
					},
					new TranslationUnitCompileArguments()
					{
						SourceFile = new Path("TestFile2.cpp"),
						TargetFile = new Path("obj/TestFile2.mock.obj"),
					},
					new TranslationUnitCompileArguments()
					{
						SourceFile = new Path("TestFile3.cpp"),
						TargetFile = new Path("obj/TestFile3.mock.obj"),
					},
				};

				var expectedLinkArguments = new LinkArguments();
				expectedLinkArguments.TargetFile = new Path("bin/Library.mock.lib");
				expectedLinkArguments.TargetType = LinkTarget.StaticLibrary;
				expectedLinkArguments.TargetArchitecture = "x64";
				expectedLinkArguments.TargetRootDirectory = new Path("C:/target/");
				expectedLinkArguments.ObjectFiles = new List<Path>()
				{
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
					buildState.GetBuildOperations());
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
				// Setup the input build state
				var buildState = new MockBuildState();
				var state = buildState.ActiveState;

				// Setup build table
				var buildTable = new ValueTable();
				state.Add("Build", new Value(buildTable));
				buildTable.Add("TargetName", new Value("Library"));
				buildTable.Add("TargetType", new Value((long)BuildTargetType.StaticLibrary));
				buildTable.Add("LanguageStandard", new Value((long)LanguageStandard.CPP20));
				buildTable.Add("SourceRootDirectory", new Value("C:/source/"));
				buildTable.Add("TargetRootDirectory", new Value("C:/target/"));
				buildTable.Add("ObjectDirectory", new Value("obj/"));
				buildTable.Add("BinaryDirectory", new Value("bin/"));
				buildTable.Add("ModuleInterfaceSourceFile", new Value("Public.cpp"));
				buildTable.Add("Source", new Value(new ValueList()
				{
					new Value("TestFile1.cpp"),
					new Value("TestFile2.cpp"),
					new Value("TestFile3.cpp"),
				}));
				buildTable.Add("IncludeDirectories", new Value(new ValueList()
				{
					new Value("Folder"),
					new Value("AnotherFolder/Sub"),
				}));
				buildTable.Add("ModuleDependencies", new Value(new ValueList()
				{
					new Value("../Other/bin/OtherModule1.mock.bmi"),
					new Value("../OtherModule2.mock.bmi"),
				}));
				buildTable.Add("OptimizationLevel", new Value((long)BuildOptimizationLevel.None));
				buildTable.Add("PreprocessorDefinitions", new Value(new ValueList()
				{
					new Value("DEBUG"),
					new Value("AWESOME"),
				}));

				// Setup parameters table
				var parametersTable = new ValueTable();
				state.Add("Parameters", new Value(parametersTable));
				parametersTable.Add("Architecture", new Value("x64"));
				parametersTable.Add("Compiler", new Value("MOCK"));

				// Register the mock compiler
				var compiler = new Compiler.Mock.Compiler();
				var compilerFactory = new Dictionary<string, Func<IValueTable, ICompiler>>();
				compilerFactory.Add("MOCK", (IValueTable state) => { return compiler; });

				var factory = new ValueFactory();
				var uut = new BuildTask(buildState, factory, compilerFactory);

				uut.Execute();

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
						"INFO: Generate Module Unit Compile: ./Public.cpp",
						"INFO: Generate Compile Operation: ./TestFile1.cpp",
						"INFO: Generate Compile Operation: ./TestFile2.cpp",
						"INFO: Generate Compile Operation: ./TestFile3.cpp",
						"INFO: CoreLink",
						"INFO: Linking target",
						"INFO: Generate Link Operation: ./bin/Library.mock.lib",
						"INFO: Build Generate Done",
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
					InterfaceUnit = new InterfaceUnitCompileArguments()
					{
						ModuleInterfaceTarget = new Path("obj/Public.mock.bmi"),
						SourceFile = new Path("Public.cpp"),
						TargetFile = new Path("obj/Public.mock.obj"),
					},
					ImplementationUnits = new List<TranslationUnitCompileArguments>()
					{
						new TranslationUnitCompileArguments()
						{
							SourceFile = new Path("TestFile1.cpp"),
							TargetFile = new Path("obj/TestFile1.mock.obj"),
						},
						new TranslationUnitCompileArguments()
						{
							SourceFile = new Path("TestFile2.cpp"),
							TargetFile = new Path("obj/TestFile2.mock.obj"),
						},
						new TranslationUnitCompileArguments()
						{
							SourceFile = new Path("TestFile3.cpp"),
							TargetFile = new Path("obj/TestFile3.mock.obj"),
						},
					},
				};

				var expectedLinkArguments = new LinkArguments();
				expectedLinkArguments.TargetFile = new Path("bin/Library.mock.lib");
				expectedLinkArguments.TargetType = LinkTarget.StaticLibrary;
				expectedLinkArguments.TargetArchitecture = "x64";
				expectedLinkArguments.TargetRootDirectory = new Path("C:/target/");
				expectedLinkArguments.ObjectFiles = new List<Path>()
				{
					new Path("obj/TestFile1.mock.obj"),
					new Path("obj/TestFile2.mock.obj"),
					new Path("obj/TestFile3.mock.obj"),
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
						"Copy [./obj/Public.mock.bmi] -> [./bin/Library.mock.bmi]",
						new Path("C:/target/"),
						new Path("C:/copy.exe"),
						"\"./obj/Public.mock.bmi\" \"./bin/Library.mock.bmi\"",
						new List<Path>()
						{
							new Path("./obj/Public.mock.bmi"),
						},
						new List<Path>()
						{
							new Path("./bin/Library.mock.bmi"),
						}),
					new BuildOperation(
						"MockCompileModule: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("InputFile.in"),
						},
						new List<Path>()
						{
							new Path("OutputFile.out"),
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
					buildState.GetBuildOperations());
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
				// Setup the input build state
				var buildState = new MockBuildState();
				var state = buildState.ActiveState;

				// Setup build table
				var buildTable = new ValueTable();
				state.Add("Build", new Value(buildTable));
				buildTable.Add("TargetName", new Value("Library"));
				buildTable.Add("TargetType", new Value((long)BuildTargetType.StaticLibrary));
				buildTable.Add("LanguageStandard", new Value((long)LanguageStandard.CPP20));
				buildTable.Add("SourceRootDirectory", new Value("C:/source/"));
				buildTable.Add("TargetRootDirectory", new Value("C:/target/"));
				buildTable.Add("ObjectDirectory", new Value("obj/"));
				buildTable.Add("BinaryDirectory", new Value("bin/"));
				buildTable.Add("ModuleInterfaceSourceFile", new Value("Public.cpp"));
				state.Add("SourceFiles", new Value(new ValueList()));
				buildTable.Add("IncludeDirectories", new Value(new ValueList()
				{
					new Value("Folder"),
					new Value("AnotherFolder/Sub"),
				}));
				buildTable.Add("ModuleDependencies", new Value(new ValueList()
				{
					new Value("../Other/bin/OtherModule1.mock.bmi"),
					new Value("../OtherModule2.mock.bmi"),
				}));
				buildTable.Add("OptimizationLevel", new Value((long)BuildOptimizationLevel.None));
				buildTable.Add("PreprocessorDefinitions", new Value(new ValueList()
				{
					new Value("DEBUG"),
					new Value("AWESOME"),
				}));

				// Setup parameters table
				var parametersTable = new ValueTable();
				state.Add("Parameters", new Value(parametersTable));
				parametersTable.Add("Architecture", new Value("x64"));
				parametersTable.Add("Compiler", new Value("MOCK"));

				// Register the mock compiler
				var compiler = new Compiler.Mock.Compiler();
				var compilerFactory = new Dictionary<string, Func<IValueTable, ICompiler>>();
				compilerFactory.Add("MOCK", (IValueTable state) => { return compiler; });

				var factory = new ValueFactory();
				var uut = new BuildTask(buildState, factory, compilerFactory);

				uut.Execute();

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
					new List<string>
					{
						"INFO: Generate Module Unit Compile: ./Public.cpp",
						"INFO: CoreLink",
						"INFO: Linking target",
						"INFO: Generate Link Operation: ./bin/Library.mock.lib",
						"INFO: Build Generate Done",
					},
					testListener.GetMessages());

				// Setup the shared arguments
				var expectedCompileArguments = new SharedCompileArguments()
				{
					Standard = LanguageStandard.CPP20,
					Optimize = OptimizationLevel.None,
					SourceRootDirectory = new Path("C:/source/"),
					TargetRootDirectory = new Path("C:/target/"),
					ObjectDirectory = new Path("./obj/"),
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
					InterfaceUnit = new InterfaceUnitCompileArguments()
					{
						SourceFile = new Path("./Public.cpp"),
						TargetFile = new Path("./obj/Public.mock.obj"),
						ModuleInterfaceTarget = new Path("./obj/Public.mock.bmi"),
					}
				};

				var expectedLinkArguments = new LinkArguments();
				expectedLinkArguments.TargetFile = new Path("bin/Library.mock.lib");
				expectedLinkArguments.TargetType = LinkTarget.StaticLibrary;
				expectedLinkArguments.TargetArchitecture = "x64";
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
						"Copy [./obj/Public.mock.bmi] -> [./bin/Library.mock.bmi]",
						new Path("C:/target/"),
						new Path("C:/copy.exe"),
						"\"./obj/Public.mock.bmi\" \"./bin/Library.mock.bmi\"",
						new List<Path>()
						{
							new Path("./obj/Public.mock.bmi"),
						},
						new List<Path>()
						{
							new Path("./bin/Library.mock.bmi"),
						}),
					new BuildOperation(
						"MockCompileModule: 1",
						new Path("MockWorkingDirectory"),
						new Path("MockCompiler.exe"),
						"Arguments",
						new List<Path>()
						{
							new Path("InputFile.in"),
						},
						new List<Path>()
						{
							new Path("OutputFile.out"),
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
					buildState.GetBuildOperations());
			}
		}
	}
}
