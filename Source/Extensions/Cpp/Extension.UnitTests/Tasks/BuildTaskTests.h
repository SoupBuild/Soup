// <copyright file="BuildTaskTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Cpp::UnitTests
{
	class BuildTaskTests
	{
	public:
		// [[Fact]]
		void Initialize_Success()
		{
			auto compilerFactory = CompilerFactory();
			auto uut = BuildTask(compilerFactory);
			
			Assert::AreEqual("Build", uut.GetName(), "Verify name matches expected.");
		}

		// [[Fact]]
		void Build_Executable()
		{
			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = Opal::ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Cpp::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Build::Utilities::ValueTableWrapper state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto fileSystemState = Build::Runtime::FileSystemState(1234);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable(), fileSystemState);
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());
			state.CreateValue("TargetName").SetValueString("Program");
			state.CreateValue("TargetType").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildTargetType::Executable));
			state.CreateValue("LanguageStandard").SetValueInteger(
				static_cast<int64_t>(Compiler::LanguageStandard::CPP20));
			state.CreateValue("WorkingDirectory").SetValueString("C:/root/");
			state.CreateValue("ObjectDirectory").SetValueString("obj");
			state.CreateValue("BinaryDirectory").SetValueString("bin");
			state.CreateValue("SourceFiles").SetValueStringList(
				std::vector<std::string>({
					"TestFile.cpp",
				}));
			state.CreateValue("IncludeDirectories").SetValueStringList(std::vector<std::string>());
			state.CreateValue("IncludeModules").SetValueStringList(std::vector<std::string>());
			state.CreateValue("OptimizationLevel").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildOptimizationLevel::None));

			auto result = uut.TryExecute(buildState);
			Assert::AreEqual(Build::ApiCallResult::Success, result, "Verify TryExecute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: TargetName = Program",
					"DIAG: TargetType = Executable",
					"DIAG: LanguageStandard = C++20",
					"DIAG: WorkingDirectory = C:/root/",
					"DIAG: ObjectDirectory = obj",
					"DIAG: BinaryDirectory = bin",
					"DIAG: ModuleInterfaceSourceFile = ./",
					"DIAG: OptimizationLevel = None",
					"DIAG: GenerateSourceDebugInfo = false",
					"DIAG: IncludeDirectories = ",
					"DIAG: PreprocessorDefinitions = ",
					"INFO: Compiling source files",
					"INFO: Generate Compile Operation: ./TestFile.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Operation: ./bin/Program.exe",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			auto expectedCompileArguments = Compiler::SharedCompileArguments();
			expectedCompileArguments.Standard = Compiler::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Compiler::OptimizationLevel::None;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			
			auto expectedTranslationUnitArguments = Compiler::TranslationUnitCompileArguments();
			expectedTranslationUnitArguments.SourceFile = Path("TestFile.cpp");
			expectedTranslationUnitArguments.TargetFile = Path("obj/TestFile.mock.obj");
			expectedCompileArguments.ImplementationUnits.push_back(expectedTranslationUnitArguments);

			auto expectedLinkArguments = Compiler::LinkArguments();
			expectedLinkArguments.TargetType = Compiler::LinkTarget::Executable;
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
				std::vector<Compiler::SharedCompileArguments>({
					expectedCompileArguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Compiler::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedBuildOperations = std::vector<Build::Utilities::BuildOperation>({
				Build::Utilities::BuildOperation(
					"MakeDir [C:/root/obj]",
					Path("./"),
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					std::vector<Path>({}),
					std::vector<Path>({})),
				Build::Utilities::BuildOperation(
					"MakeDir [C:/root/bin]",
					Path("./"),
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					std::vector<Path>({}),
					std::vector<Path>({})),
				Build::Utilities::BuildOperation(
					"MockLink: 1",
					Path("MockWorkingDirectory/"),
					Path("MockLinker.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 1",
					Path("MockWorkingDirectory/"),
					Path("MockCompiler.exe"),
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
		}

		// [[Fact]]
		void Build_Executable_OptimizeSpeed()
		{
			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Cpp::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Build::Utilities::ValueTableWrapper& state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto fileSystemState = Build::Runtime::FileSystemState(1234);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable(), fileSystemState);
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());
			state.CreateValue("TargetName").SetValueString("Program");
			state.CreateValue("TargetType").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildTargetType::Executable));
			state.CreateValue("LanguageStandard").SetValueInteger(
				static_cast<int64_t>(Compiler::LanguageStandard::CPP20));
			state.CreateValue("WorkingDirectory").SetValueString("C:/root/");
			state.CreateValue("ObjectDirectory").SetValueString("obj");
			state.CreateValue("BinaryDirectory").SetValueString("bin");
			state.CreateValue("SourceFiles").SetValueStringList(
				std::vector<std::string>({
					"TestFile.cpp",
				}));
			state.CreateValue("IncludeDirectories").SetValueStringList(std::vector<std::string>());
			state.CreateValue("IncludeModules").SetValueStringList(std::vector<std::string>());
			state.CreateValue("OptimizationLevel").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildOptimizationLevel::Speed));

			auto result = uut.TryExecute(buildState);
			Assert::AreEqual(Build::ApiCallResult::Success, result, "Verify TryExecute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: TargetName = Program",
					"DIAG: TargetType = Executable",
					"DIAG: LanguageStandard = C++20",
					"DIAG: WorkingDirectory = C:/root/",
					"DIAG: ObjectDirectory = obj",
					"DIAG: BinaryDirectory = bin",
					"DIAG: ModuleInterfaceSourceFile = ./",
					"DIAG: OptimizationLevel = Speed",
					"DIAG: GenerateSourceDebugInfo = false",
					"DIAG: IncludeDirectories = ",
					"DIAG: PreprocessorDefinitions = ",
					"INFO: Compiling source files",
					"INFO: Generate Compile Operation: ./TestFile.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Operation: ./bin/Program.exe",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			auto expectedCompileArguments = Compiler::CompileArguments();
			expectedCompileArguments.Standard = Compiler::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Compiler::OptimizationLevel::Speed;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");

			auto expectedLinkArguments = Compiler::LinkArguments();
			expectedLinkArguments.TargetType = Compiler::LinkTarget::Executable;
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile.mock.obj"),
			});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<Compiler::CompileArguments>({
					expectedCompileArguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Compiler::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedBuildOperations = std::vector<Build::Utilities::BuildOperation>({
				Build::Utilities::BuildOperation(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({})),
				Build::Utilities::BuildOperation(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({})),
				Build::Utilities::BuildOperation(
					"MockLink: 1",
					Path("MockWorkingDirectory/"),
					Path("MockLinker.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 1",
					Path("MockWorkingDirectory/"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				Build::Utilities::BuildOperationListWrapper(buildState.GetRootOperationList()));
		}

		// [[Fact]]
		void Build_Executable_OptimizeSize()
		{
			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Cpp::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Build::Utilities::ValueTableWrapper& state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto fileSystemState = Build::Runtime::FileSystemState(1234);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable(), fileSystemState);
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());
			state.CreateValue("TargetName").SetValueString("Program");
			state.CreateValue("TargetType").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildTargetType::Executable));
			state.CreateValue("LanguageStandard").SetValueInteger(
				static_cast<int64_t>(Compiler::LanguageStandard::CPP20));
			state.CreateValue("WorkingDirectory").SetValueString("C:/root/");
			state.CreateValue("ObjectDirectory").SetValueString("obj");
			state.CreateValue("BinaryDirectory").SetValueString("bin");
			state.CreateValue("SourceFiles").SetValueStringList(
				std::vector<std::string>({
					"TestFile.cpp",
				}));
			state.CreateValue("IncludeDirectories").SetValueStringList(std::vector<std::string>());
			state.CreateValue("IncludeModules").SetValueStringList(std::vector<std::string>());
			state.CreateValue("OptimizationLevel").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildOptimizationLevel::Size));

			auto result = uut.TryExecute(buildState);
			Assert::AreEqual(Build::ApiCallResult::Success, result, "Verify TryExecute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: TargetName = Program",
					"DIAG: TargetType = Executable",
					"DIAG: LanguageStandard = C++20",
					"DIAG: WorkingDirectory = C:/root/",
					"DIAG: ObjectDirectory = obj",
					"DIAG: BinaryDirectory = bin",
					"DIAG: ModuleInterfaceSourceFile = ./",
					"DIAG: OptimizationLevel = Size",
					"DIAG: GenerateSourceDebugInfo = false",
					"DIAG: IncludeDirectories = ",
					"DIAG: PreprocessorDefinitions = ",
					"INFO: Compiling source files",
					"INFO: Generate Compile Operation: ./TestFile.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Operation: ./bin/Program.exe",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			auto expectedCompileArguments = Compiler::CompileArguments();
			expectedCompileArguments.Standard = Compiler::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Compiler::OptimizationLevel::Size;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");

			auto expectedLinkArguments = Compiler::LinkArguments();
			expectedLinkArguments.TargetType = Compiler::LinkTarget::Executable;
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile.mock.obj"),
			});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<Compiler::CompileArguments>({
					expectedCompileArguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Compiler::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedBuildOperations = std::vector<Build::Utilities::BuildOperation>({
				Build::Utilities::BuildOperation(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({})),
				Build::Utilities::BuildOperation(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({})),
				Build::Utilities::BuildOperation(
					"MockLink: 1",
					Path("MockWorkingDirectory/"),
					Path("MockLinker.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
				Build::Utilities::BuildOperation(
					"MockCompile: 1",
					Path("MockWorkingDirectory/"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				Build::Utilities::BuildOperationListWrapper(buildState.GetRootOperationList()));
		}

		// [[Fact]]
		void Build_Library_MultipleFiles()
		{
			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Cpp::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Build::Utilities::ValueTableWrapper& state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto fileSystemState = Build::Runtime::FileSystemState(1234);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable(), fileSystemState);
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());
			state.CreateValue("TargetName").SetValueString("Library");
			state.CreateValue("TargetType").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildTargetType::StaticLibrary));
			state.CreateValue("LanguageStandard").SetValueInteger(
				static_cast<int64_t>(Compiler::LanguageStandard::CPP20));
			state.CreateValue("WorkingDirectory").SetValueString("C:/root/");
			state.CreateValue("ObjectDirectory").SetValueString("obj");
			state.CreateValue("BinaryDirectory").SetValueString("bin");
			state.CreateValue("SourceFiles").SetValueStringList(
				std::vector<std::string>({
					"TestFile1.cpp",
					"TestFile2.cpp",
					"TestFile3.cpp",
				}));
			state.CreateValue("IncludeDirectories").SetValueStringList(
				std::vector<std::string>({
					"Folder",
					"AnotherFolder/Sub",
				}));
			state.CreateValue("IncludeModules").SetValueStringList(
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.bmi",
					"../OtherModule2.mock.bmi",
				}));
			state.CreateValue("OptimizationLevel").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildOptimizationLevel::None));
			state.CreateValue("LinkDependencies").SetValueStringList(
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.a",
					"../OtherModule2.mock.a",
				}));

			auto result = uut.TryExecute(buildState);
			Assert::AreEqual(Build::ApiCallResult::Success, result, "Verify TryExecute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: TargetName = Library",
					"DIAG: TargetType = StaticLibrary",
					"DIAG: LanguageStandard = C++20",
					"DIAG: WorkingDirectory = C:/root/",
					"DIAG: ObjectDirectory = obj",
					"DIAG: BinaryDirectory = bin",
					"DIAG: ModuleInterfaceSourceFile = ./",
					"DIAG: OptimizationLevel = None",
					"DIAG: GenerateSourceDebugInfo = false",
					"DIAG: IncludeDirectories = Folder AnotherFolder/Sub",
					"DIAG: PreprocessorDefinitions = ",
					"INFO: Compiling source files",
					"INFO: Generate Compile Operation: ./TestFile1.cpp",
					"INFO: Generate Compile Operation: ./TestFile2.cpp",
					"INFO: Generate Compile Operation: ./TestFile3.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Operation: ./bin/Library.mock.lib",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Setup the shared arguments
			auto expectedCompileArguments = Compiler::CompileArguments();
			expectedCompileArguments.Standard = Compiler::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Compiler::OptimizationLevel::None;
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

			auto expectedLinkArguments = Compiler::LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Library.mock.lib");
			expectedLinkArguments.TargetType = Compiler::LinkTarget::StaticLibrary;
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile1.mock.obj"),
				Path("obj/TestFile2.mock.obj"),
				Path("obj/TestFile3.mock.obj"),
			});
			expectedLinkArguments.LibraryFiles = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<Compiler::CompileArguments>({
					expectedCompile1Arguments,
					expectedCompile2Arguments,
					expectedCompile3Arguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Compiler::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedCompileOperations = std::vector<Build::Utilities::BuildOperation>({
				Build::Utilities::BuildOperation(
					"MockCompile: 1",
						Path("MockWorkingDirectory/"),
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
					Path("MockWorkingDirectory/"),
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
					Path("MockWorkingDirectory/"),
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
					Path("MockWorkingDirectory/"),
					Path("MockLinker.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					})),
			});

			auto expectedBuildOperations = std::vector<Build::Utilities::BuildOperation>({
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					expectedCompileOperations),
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					expectedCompileOperations),
			});

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				Build::Utilities::BuildOperationListWrapper(buildState.GetRootOperationList()));
		}

		// [[Fact]]
		void Build_Library_ModuleInterface()
		{
			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Cpp::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Build::Utilities::ValueTableWrapper& state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto fileSystemState = Build::Runtime::FileSystemState(1234);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable(), fileSystemState);
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());
			state.CreateValue("TargetName").SetValueString("Library");
			state.CreateValue("TargetType").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildTargetType::StaticLibrary));
			state.CreateValue("LanguageStandard").SetValueInteger(
				static_cast<int64_t>(Compiler::LanguageStandard::CPP20));
			state.CreateValue("WorkingDirectory").SetValueString("C:/root/");
			state.CreateValue("ObjectDirectory").SetValueString("obj");
			state.CreateValue("BinaryDirectory").SetValueString("bin");
			state.CreateValue("ModuleInterfaceSourceFile").SetValueString("Public.cpp");
			state.CreateValue("SourceFiles").SetValueStringList(
				std::vector<std::string>({
					"TestFile1.cpp",
					"TestFile2.cpp",
					"TestFile3.cpp",
				}));
			state.CreateValue("IncludeDirectories").SetValueStringList(
				std::vector<std::string>({
					"Folder",
					"AnotherFolder/Sub",
				}));
			state.CreateValue("IncludeModules").SetValueStringList(
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.bmi",
					"../OtherModule2.mock.bmi",
				}));
			state.CreateValue("OptimizationLevel").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildOptimizationLevel::None));
			state.CreateValue("LinkDependencies").SetValueStringList(
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.a",
					"../OtherModule2.mock.a",
				}));
			state.CreateValue("PreprocessorDefinitions").SetValueStringList(
				std::vector<std::string>({
					"DEBUG",
					"AWESOME",
				}));

			auto result = uut.TryExecute(buildState);
			Assert::AreEqual(Build::ApiCallResult::Success, result, "Verify TryExecute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: TargetName = Library",
					"DIAG: TargetType = StaticLibrary",
					"DIAG: LanguageStandard = C++20",
					"DIAG: WorkingDirectory = C:/root/",
					"DIAG: ObjectDirectory = obj",
					"DIAG: BinaryDirectory = bin",
					"DIAG: ModuleInterfaceSourceFile = Public.cpp",
					"DIAG: OptimizationLevel = None",
					"DIAG: GenerateSourceDebugInfo = false",
					"DIAG: IncludeDirectories = Folder AnotherFolder/Sub",
					"DIAG: PreprocessorDefinitions = DEBUG AWESOME",
					"INFO: CompileModuleInterfaceUnit",
					"INFO: Generate Compile Operation: ./Public.cpp",
					"INFO: Compiling source files",
					"INFO: Generate Compile Operation: ./TestFile1.cpp",
					"INFO: Generate Compile Operation: ./TestFile2.cpp",
					"INFO: Generate Compile Operation: ./TestFile3.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Operation: ./bin/Library.mock.lib",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Setup the shared arguments
			auto expectedCompileArguments = Compiler::CompileArguments();
			expectedCompileArguments.Standard = Compiler::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Compiler::OptimizationLevel::None;
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
			expectedCompileArguments.IncludeModules.push_back(Path("obj/Public.mock.bmi"));

			auto expectedCompile1Arguments = expectedCompileArguments;
			expectedCompile1Arguments.SourceFile = Path("TestFile1.cpp");
			expectedCompile1Arguments.TargetFile = Path("obj/TestFile1.mock.obj");
			auto expectedCompile2Arguments = expectedCompileArguments;
			expectedCompile2Arguments.SourceFile = Path("TestFile2.cpp");
			expectedCompile2Arguments.TargetFile = Path("obj/TestFile2.mock.obj");
			auto expectedCompile3Arguments = expectedCompileArguments;
			expectedCompile3Arguments.SourceFile = Path("TestFile3.cpp");
			expectedCompile3Arguments.TargetFile = Path("obj/TestFile3.mock.obj");

			auto expectedLinkArguments = Compiler::LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Library.mock.lib");
			expectedLinkArguments.TargetType = Compiler::LinkTarget::StaticLibrary;
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile1.mock.obj"),
				Path("obj/TestFile2.mock.obj"),
				Path("obj/TestFile3.mock.obj"),
				Path("obj/Public.mock.obj"),
			});
			expectedLinkArguments.LibraryFiles = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<Compiler::CompileArguments>({
					expectedCompileModuleArguments,
					expectedCompile1Arguments,
					expectedCompile2Arguments,
					expectedCompile3Arguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Compiler::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedLinkOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
						"MockLink: 1",
						Path("MockWorkingDirectory/"),
						Path("MockLinker.exe"),
						"Arguments",
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						})));

			auto expectedCompileSourceOperations = std::vector<Build::Utilities::BuildOperation>({
				new Build::Utilities::BuildOperation(
					"MockCompile: 2",
					Path("MockWorkingDirectory/"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<Build::Utilities::BuildOperation>({
						expectedLinkOperation,
					})),
				new Build::Utilities::BuildOperation(
					"MockCompile: 3",
					Path("MockWorkingDirectory/"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<Build::Utilities::BuildOperation>({
						expectedLinkOperation,
					})),
				new Build::Utilities::BuildOperation(
					"MockCompile: 4",
					Path("MockWorkingDirectory/"),
					Path("MockCompiler.exe"),
					"Arguments",
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<Build::Utilities::BuildOperation>({
						expectedLinkOperation,
					})),
			});

			auto expectedCopyModuleInterfaceOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
						"Copy [C:/root/obj/Public.mock.bmi] -> [C:/root/bin/Library.mock.bmi]",
						Path("C:/Windows/System32/cmd.exe"),
						"/C copy /Y \"C:\\root\\obj\\Public.mock.bmi\" \"C:\\root\\bin\\Library.mock.bmi\"",
						Path("./"),
						std::vector<Path>({
							Path("C:/root/obj/Public.mock.bmi"),
						}),
						std::vector<Path>({
							Path("C:/root/bin/Library.mock.bmi"),
						}),
						expectedCompileSourceOperations));

			auto expectedCompileModuleOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
						"MockCompile: 1",
						Path("MockWorkingDirectory/"),
						Path("MockCompiler.exe"),
						"Arguments",
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						}),
						std::vector<Build::Utilities::BuildOperation>({
							expectedCopyModuleInterfaceOperation,
						})));

			auto expectedBuildOperations = std::vector<Build::Utilities::BuildOperation>({
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<Build::Utilities::BuildOperation>({
						expectedCompileModuleOperation,
					})),
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<Build::Utilities::BuildOperation>({
						expectedCompileModuleOperation,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				Build::Utilities::BuildOperationListWrapper(buildState.GetRootOperationList()));
		}

		// [[Fact]]
		void Build_Library_ModuleInterfaceNoSource()
		{
			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Cpp::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Build::Utilities::ValueTableWrapper& state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto fileSystemState = Build::Runtime::FileSystemState(1234);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable(), fileSystemState);
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());
			state.CreateValue("TargetName").SetValueString("Library");
			state.CreateValue("TargetType").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildTargetType::StaticLibrary));
			state.CreateValue("LanguageStandard").SetValueInteger(
				static_cast<int64_t>(Compiler::LanguageStandard::CPP20));
			state.CreateValue("WorkingDirectory").SetValueString("C:/root/");
			state.CreateValue("ObjectDirectory").SetValueString("obj");
			state.CreateValue("BinaryDirectory").SetValueString("bin");
			state.CreateValue("ModuleInterfaceSourceFile").SetValueString("Public.cpp");
			state.CreateValue("SourceFiles").SetValueStringList(std::vector<std::string>());
			state.CreateValue("IncludeDirectories").SetValueStringList(
				std::vector<std::string>({
					"Folder",
					"AnotherFolder/Sub",
				}));
			state.CreateValue("IncludeModules").SetValueStringList(
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.bmi",
					"../OtherModule2.mock.bmi",
				}));
			state.CreateValue("OptimizationLevel").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildOptimizationLevel::None));
			state.CreateValue("LinkDependencies").SetValueStringList(
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.a",
					"../OtherModule2.mock.a",
				}));

			auto result = uut.TryExecute(buildState);
			Assert::AreEqual(Build::ApiCallResult::Success, result, "Verify TryExecute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: TargetName = Library",
					"DIAG: TargetType = StaticLibrary",
					"DIAG: LanguageStandard = C++20",
					"DIAG: WorkingDirectory = C:/root/",
					"DIAG: ObjectDirectory = obj",
					"DIAG: BinaryDirectory = bin",
					"DIAG: ModuleInterfaceSourceFile = Public.cpp",
					"DIAG: OptimizationLevel = None",
					"DIAG: GenerateSourceDebugInfo = false",
					"DIAG: IncludeDirectories = Folder AnotherFolder/Sub",
					"DIAG: PreprocessorDefinitions = ",
					"INFO: CompileModuleInterfaceUnit",
					"INFO: Generate Compile Operation: ./Public.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Operation: ./bin/Library.mock.lib",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Setup the shared arguments
			auto expectedCompileArguments = Compiler::CompileArguments();
			expectedCompileArguments.Standard = Compiler::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Compiler::OptimizationLevel::None;
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

			auto expectedLinkArguments = Compiler::LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Library.mock.lib");
			expectedLinkArguments.TargetType = Compiler::LinkTarget::StaticLibrary;
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/Public.mock.obj"),
			});
			expectedLinkArguments.LibraryFiles = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<Compiler::CompileArguments>({
					expectedCompileModuleArguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Compiler::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedLinkOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
						"MockLink: 1",
						Path("MockWorkingDirectory/"),
						Path("MockLinker.exe"),
						"Arguments",
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						})));

			auto expectedCopyModuleInterfaceOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
						"Copy [C:/root/obj/Public.mock.bmi] -> [C:/root/bin/Library.mock.bmi]",
						Path("C:/Windows/System32/cmd.exe"),
						"/C copy /Y \"C:\\root\\obj\\Public.mock.bmi\" \"C:\\root\\bin\\Library.mock.bmi\"",
						Path("./"),
						std::vector<Path>({
							Path("C:/root/obj/Public.mock.bmi"),
						}),
						std::vector<Path>({
							Path("C:/root/bin/Library.mock.bmi"),
						}),
						std::vector<Build::Utilities::BuildOperation>({
							expectedLinkOperation,
						})));

			auto expectedCompileModuleOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
						"MockCompile: 1",
						Path("MockWorkingDirectory/"),
						Path("MockCompiler.exe"),
						"Arguments",
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						}),
						std::vector<Build::Utilities::BuildOperation>({
							expectedCopyModuleInterfaceOperation,
						})));

			auto expectedBuildOperations = std::vector<Build::Utilities::BuildOperation>({
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<Build::Utilities::BuildOperation>({
						expectedCompileModuleOperation,
					})),
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<Build::Utilities::BuildOperation>({
						expectedCompileModuleOperation,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				Build::Utilities::BuildOperationListWrapper(buildState.GetRootOperationList()));
		}
	};
}
