// <copyright file="BuildEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp::Compiler::UnitTests
{
	class BuildEngineTests
	{
	public:
		[[Fact]]
		void Initialize_Success()
		{
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildEngine(compiler);
		}

		[[Fact]]
		void Build_Executable()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Setup the build arguments
			auto arguments = BuildArguments();
			arguments.TargetName = "Program";
			arguments.TargetType = BuildTargetType::Executable;
			arguments.LanguageStandard = LanguageStandard::CPP20;
			arguments.WorkingDirectory = Path("C:/root/");
			arguments.ObjectDirectory = Path("obj/");
			arguments.BinaryDirectory = Path("bin/");
			arguments.SourceFiles = std::vector<Path>({
				Path("TestFile.cpp"),
			});
			arguments.OptimizationLevel = BuildOptimizationLevel::None;
			arguments.LinkDependencies = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});

			auto uut = BuildEngine(compiler);
			auto fileSystemState = Build::Runtime::FileSystemState(1234);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable(), fileSystemState);
			auto result = uut.Execute(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Compiling source files",
					"INFO: Generate Compile Operation: ./TestFile.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Operation: ./bin/Program.exe",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::None;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetType = LinkTarget::Executable;
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile.mock.obj"),
			});
			expectedLinkArguments.LibraryFiles = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<CompileArguments>({
					expectedCompileArguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			auto expectedBuildOperations = std::vector<Build::Utilities::BuildOperation>({
				Build::Utilities::BuildOperation(
					"MakeDir [./obj/]",
					Path("C:/root/"),
					Path("C:/testlocation/mkdir.exe"),
					"\"./obj/\"",
					std::vector<Path>({}),
					std::vector<Path>({
						Path("./obj/"),
					})),
				Build::Utilities::BuildOperation(
					"MakeDir [./bin/]",
					Path("C:/root/"),
					Path("C:/testlocation/mkdir.exe"),
					"\"./bin/\"",
					std::vector<Path>({}),
					std::vector<Path>({
						Path("./bin/"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 1",
					Path("MockWorkingDirectory"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"MockLink: 1",
					Path("MockWorkingDirectory"),
					Path("MockLinker.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
			});

			Assert::AreEqual(
				expectedBuildOperations,
				result.BuildOperations,
				"Verify Build Operations Result");

			Assert::AreEqual(
				std::vector<Path>({}),
				result.ModuleDependencies,
				"Verify Module Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({}),
				result.LinkDependencies,
				"Verify Link Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({
					Path("C:/root/bin/Program.exe"),
				}),
				result.RuntimeDependencies,
				"Verify Runtime Dependencies Result");
		}

		[[Fact]]
		void Build_Library_MultipleFiles()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Register the mock compiler
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();

			// Setup the build arguments
			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::StaticLibrary;
			arguments.LanguageStandard = LanguageStandard::CPP20;
			arguments.WorkingDirectory = Path("C:/root/");
			arguments.ObjectDirectory = Path("obj/");
			arguments.BinaryDirectory = Path("bin/");
			arguments.SourceFiles = std::vector<Path>({
				Path("TestFile1.cpp"),
				Path("TestFile2.cpp"),
				Path("TestFile3.cpp"),
			});
			arguments.OptimizationLevel = BuildOptimizationLevel::Size;
			arguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			arguments.ModuleDependencies = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.bmi"),
				Path("../OtherModule2.mock.bmi"),
			});
			arguments.LinkDependencies = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});

			auto uut = BuildEngine(compiler);
			auto fileSystemState = Build::Runtime::FileSystemState(1234);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable(), fileSystemState);
			auto result = uut.Execute(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Compiling source files",
					"INFO: Generate Compile Operation: ./TestFile1.cpp",
					"INFO: Generate Compile Operation: ./TestFile2.cpp",
					"INFO: Generate Compile Operation: ./TestFile3.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Operation: ./bin/Library.mock.lib",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Setup the shared arguments
			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::Size;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			expectedCompileArguments.IncludeModules = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.bmi"),
				Path("../OtherModule2.mock.bmi"),
			});

			auto expectedCompile1Arguments = expectedCompileArguments;
			expectedCompile1Arguments.SourceFile = Path("TestFile1.cpp");
			expectedCompile1Arguments.TargetFile = Path("obj/TestFile1.mock.obj");
			auto expectedCompile2Arguments = expectedCompileArguments;
			expectedCompile2Arguments.SourceFile = Path("TestFile2.cpp");
			expectedCompile2Arguments.TargetFile = Path("obj/TestFile2.mock.obj");
			auto expectedCompile3Arguments = expectedCompileArguments;
			expectedCompile3Arguments.SourceFile = Path("TestFile3.cpp");
			expectedCompile3Arguments.TargetFile = Path("obj/TestFile3.mock.obj");

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Library.mock.lib");
			expectedLinkArguments.TargetType = LinkTarget::StaticLibrary;
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile1.mock.obj"),
				Path("obj/TestFile2.mock.obj"),
				Path("obj/TestFile3.mock.obj"),
			});

			// Note: There is no need to send along the static libraries for a static library linking
			expectedLinkArguments.LibraryFiles = std::vector<Path>({});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<CompileArguments>({
					expectedCompile1Arguments,
					expectedCompile2Arguments,
					expectedCompile3Arguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedBuildOperations = std::vector<Build::Utilities::BuildOperation>({
				Build::Utilities::BuildOperation(
					"MakeDir [./obj/]",
					Path("C:/root/"),
					Path("C:/testlocation/mkdir.exe"),
					"\"./obj/\"",
					std::vector<Path>({}),
					std::vector<Path>({
						Path("./obj/"),
					})),
				Build::Utilities::BuildOperation(
					"MakeDir [./bin/]",
					Path("C:/root/"),
					Path("C:/testlocation/mkdir.exe"),
					"\"./bin/\"",
					std::vector<Path>({}),
					std::vector<Path>({
						Path("./bin/"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 1",
					Path("MockWorkingDirectory"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 2",
					Path("MockWorkingDirectory"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 3",
					Path("MockWorkingDirectory"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"MockLink: 1",
					Path("MockWorkingDirectory"),
					Path("MockLinker.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
			});

			Assert::AreEqual(
				expectedBuildOperations,
				result.BuildOperations,
				"Verify Build Operations Result");

			Assert::AreEqual(
				std::vector<Path>({}),
				result.ModuleDependencies,
				"Verify Module Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({
					Path("../Other/bin/OtherModule1.mock.a"),
					Path("../OtherModule2.mock.a"),
					Path("C:/root/bin/Library.mock.lib"),
				}),
				result.LinkDependencies,
				"Verify Link Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({}),
				result.RuntimeDependencies,
				"Verify Runtime Dependencies Result");
		}

		[[Fact]]
		void Build_Library_ModuleInterface()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Register the mock compiler
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();

			// Setup the build arguments
			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::StaticLibrary;
			arguments.LanguageStandard = LanguageStandard::CPP20;
			arguments.WorkingDirectory = Path("C:/root/");
			arguments.ObjectDirectory = Path("obj/");
			arguments.BinaryDirectory = Path("bin/");
			arguments.ModuleInterfaceSourceFile = Path("Public.cpp");
			arguments.SourceFiles = std::vector<Path>({
				Path("TestFile1.cpp"),
				Path("TestFile2.cpp"),
				Path("TestFile3.cpp"),
			});
			arguments.OptimizationLevel = BuildOptimizationLevel::None;
			arguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			arguments.ModuleDependencies = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.bmi"),
				Path("../OtherModule2.mock.bmi"),
			});
			arguments.LinkDependencies = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});
			arguments.PreprocessorDefinitions = std::vector<std::string>({
				"DEBUG",
				"AWESOME",
			});

			auto uut = BuildEngine(compiler);
			auto fileSystemState = Build::Runtime::FileSystemState(1234);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable(), fileSystemState);
			auto result = uut.Execute(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: CompileModuleInterfaceUnit",
					"INFO: Generate Compile Operation: ./Public.cpp",
					"INFO: Compiling source files",
					"INFO: Generate Compile Operation: ./TestFile1.cpp",
					"INFO: Generate Compile Operation: ./TestFile2.cpp",
					"INFO: Generate Compile Operation: ./TestFile3.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Operation: ./bin/Library.mock.lib",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Setup the shared arguments
			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::None;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			expectedCompileArguments.IncludeModules = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.bmi"),
				Path("../OtherModule2.mock.bmi"),
			});
			expectedCompileArguments.PreprocessorDefinitions = std::vector<std::string>({ 
				"DEBUG",
				"AWESOME",
			});

			auto expectedCompileModuleArguments = expectedCompileArguments;
			expectedCompileModuleArguments.SourceFile = Path("Public.cpp");
			expectedCompileModuleArguments.TargetFile = Path("obj/Public.mock.obj");
			expectedCompileModuleArguments.ExportModule = true;

			// Remaining source files should reference module interface
			expectedCompileArguments.IncludeModules.push_back(Path("C:/root/bin/Library.mock.bmi"));

			auto expectedCompile1Arguments = expectedCompileArguments;
			expectedCompile1Arguments.SourceFile = Path("TestFile1.cpp");
			expectedCompile1Arguments.TargetFile = Path("obj/TestFile1.mock.obj");
			auto expectedCompile2Arguments = expectedCompileArguments;
			expectedCompile2Arguments.SourceFile = Path("TestFile2.cpp");
			expectedCompile2Arguments.TargetFile = Path("obj/TestFile2.mock.obj");
			auto expectedCompile3Arguments = expectedCompileArguments;
			expectedCompile3Arguments.SourceFile = Path("TestFile3.cpp");
			expectedCompile3Arguments.TargetFile = Path("obj/TestFile3.mock.obj");

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Library.mock.lib");
			expectedLinkArguments.TargetType = LinkTarget::StaticLibrary;
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile1.mock.obj"),
				Path("obj/TestFile2.mock.obj"),
				Path("obj/TestFile3.mock.obj"),
				Path("obj/Public.mock.obj"),
			});
			expectedLinkArguments.LibraryFiles = std::vector<Path>({});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<CompileArguments>({
					expectedCompileModuleArguments,
					expectedCompile1Arguments,
					expectedCompile2Arguments,
					expectedCompile3Arguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedBuildOperations = std::vector<Build::Utilities::BuildOperation>({
				Build::Utilities::BuildOperation(
					"MakeDir [./obj/]",
					Path("C:/root/"),
					Path("C:/testlocation/mkdir.exe"),
					"\"./obj/\"",
					std::vector<Path>({}),
					std::vector<Path>({
						Path("./obj/"),
					})),
				Build::Utilities::BuildOperation(
					"MakeDir [./bin/]",
					Path("C:/root/"),
					Path("C:/testlocation/mkdir.exe"),
					"\"./bin/\"",
					std::vector<Path>({}),
					std::vector<Path>({
						Path("./bin/"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 1",
					Path("MockWorkingDirectory"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"Copy [./obj/Public.mock.bmi] -> [./bin/Library.mock.bmi]",
					Path("C:/root/"),
					Path("C:/testlocation/copy.exe"),
					"\"./obj/Public.mock.bmi\" \"./bin/Library.mock.bmi\"",
					std::vector<Path>({
						Path("./obj/Public.mock.bmi"),
					}),
					std::vector<Path>({
						Path("./bin/Library.mock.bmi"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 2",
					Path("MockWorkingDirectory"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 3",
					Path("MockWorkingDirectory"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 4",
					Path("MockWorkingDirectory"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"MockLink: 1",
					Path("MockWorkingDirectory"),
					Path("MockLinker.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
			});

			Assert::AreEqual(
				expectedBuildOperations,
				result.BuildOperations,
				"Verify Build Operations Result");

			Assert::AreEqual(
				std::vector<Path>({
					Path("C:/root/bin/Library.mock.bmi"),
				}),
				result.ModuleDependencies,
				"Verify Module Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({
					Path("../Other/bin/OtherModule1.mock.a"),
					Path("../OtherModule2.mock.a"),
					Path("C:/root/bin/Library.mock.lib"),
				}),
				result.LinkDependencies,
				"Verify Link Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({}),
				result.RuntimeDependencies,
				"Verify Runtime Dependencies Result");
		}

		[[Fact]]
		void Build_Library_ModuleInterfaceNoSource()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Register the mock compiler
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();

			// Setup the build arguments
			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::StaticLibrary;
			arguments.LanguageStandard = LanguageStandard::CPP20;
			arguments.WorkingDirectory = Path("C:/root/");
			arguments.ObjectDirectory = Path("obj/");
			arguments.BinaryDirectory = Path("bin/");
			arguments.ModuleInterfaceSourceFile = Path("Public.cpp");
			arguments.SourceFiles = std::vector<Path>({});
			arguments.OptimizationLevel = BuildOptimizationLevel::Size;
			arguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			arguments.ModuleDependencies = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.bmi"),
				Path("../OtherModule2.mock.bmi"),
			});
			arguments.LinkDependencies = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});

			auto uut = BuildEngine(compiler);
			auto fileSystemState = Build::Runtime::FileSystemState(1234);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable(), fileSystemState);
			auto result = uut.Execute(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: CompileModuleInterfaceUnit",
					"INFO: Generate Compile Operation: ./Public.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Operation: ./bin/Library.mock.lib",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected process requests
			Assert::AreEqual(
				std::vector<std::string>({
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
					"GetCurrentProcessFileName",
				}),
				processManager->GetRequests(),
				"Verify process manager requests match expected.");

			// Setup the shared arguments
			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::Size;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			expectedCompileArguments.IncludeModules = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.bmi"),
				Path("../OtherModule2.mock.bmi"),
			});

			auto expectedCompileModuleArguments = expectedCompileArguments;
			expectedCompileModuleArguments.SourceFile = Path("Public.cpp");
			expectedCompileModuleArguments.TargetFile = Path("obj/Public.mock.obj");
			expectedCompileModuleArguments.ExportModule = true;

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Library.mock.lib");
			expectedLinkArguments.TargetType = LinkTarget::StaticLibrary;
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/Public.mock.obj"),
			});
			expectedLinkArguments.LibraryFiles = std::vector<Path>({});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<CompileArguments>({
					expectedCompileModuleArguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedBuildOperations = std::vector<Build::Utilities::BuildOperation>({
				Build::Utilities::BuildOperation(
					"MakeDir [./obj/]",
					Path("C:/root/"),
					Path("C:/testlocation/mkdir.exe"),
					"\"./obj/\"",
					std::vector<Path>({}),
					std::vector<Path>({
						Path("./obj/"),
					})),
				Build::Utilities::BuildOperation(
					"MakeDir [./bin/]",
					Path("C:/root/"),
					Path("C:/testlocation/mkdir.exe"),
					"\"./bin/\"",
					std::vector<Path>({}),
					std::vector<Path>({
						Path("./bin/"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 1",
					Path("MockWorkingDirectory"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"Copy [./obj/Public.mock.bmi] -> [./bin/Library.mock.bmi]",
					Path("C:/root/"),
					Path("C:/testlocation/copy.exe"),
					"\"./obj/Public.mock.bmi\" \"./bin/Library.mock.bmi\"",
					std::vector<Path>({
						Path("./obj/Public.mock.bmi"),
					}),
					std::vector<Path>({
						Path("./bin/Library.mock.bmi"),
					})),
				Build::Utilities::BuildOperation(
					"MockLink: 1",
					Path("MockWorkingDirectory"),
					Path("MockLinker.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
			});

			Assert::AreEqual(
				expectedBuildOperations,
				result.BuildOperations,
				"Verify Build Operations Result");

			Assert::AreEqual(
				std::vector<Path>({
					Path("C:/root/bin/Library.mock.bmi"),
				}),
				result.ModuleDependencies,
				"Verify Module Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({
					Path("../Other/bin/OtherModule1.mock.a"),
					Path("../OtherModule2.mock.a"),
					Path("C:/root/bin/Library.mock.lib"),
				}),
				result.LinkDependencies,
				"Verify Link Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({}),
				result.RuntimeDependencies,
				"Verify Runtime Dependencies Result");
		}
	};
}
