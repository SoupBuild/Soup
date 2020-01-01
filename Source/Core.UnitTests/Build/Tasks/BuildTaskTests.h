// <copyright file="BuildTaskTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class BuildTaskTests
	{
	public:
		[[Fact]]
		void Initialze_Success()
		{
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildTask(compiler);
			
			Assert::AreEqual("Build", uut.GetName(), "Verify name matches expected.");
		}

		[[Fact]]
		void Initialze_NullCompilerThrows()
		{
			auto compiler = nullptr;
			Assert::ThrowsRuntimeError([&compiler]() {
				auto uut = BuildTask(compiler);
			});
		}

		[[Fact]]
		void Build_Executable()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildTask(compiler);

			// Setup the input build state
			auto buildState = BuildState();
			auto state = BuildStateWrapper(buildState);
			state.SetPropertyStringValue("TargetName", "Program");
			state.SetPropertyIntegerValue(
				"TargetType",
				static_cast<int64_t>(BuildTargetType::Executable));
			state.SetPropertyIntegerValue(
				"LanguageStandard",
				static_cast<int64_t>(LanguageStandard::CPP20));
			state.SetPropertyStringValue("WorkingDirectory", "C:/root/");
			state.SetPropertyStringValue("ObjectDirectory", "obj");
			state.SetPropertyStringValue("BinaryDirectory", "bin");
			state.SetPropertyStringList(
				"SourceFiles", 
				std::vector<std::string>({
					"TestFile.cpp",
				}));
			state.SetPropertyStringList("IncludeDirectories", std::vector<std::string>());
			state.SetPropertyStringList("IncludeModules", std::vector<std::string>());
			state.SetPropertyIntegerValue(
				"OptimizationLevel", 
				static_cast<int64_t>(BuildOptimizationLevel::None));

			auto result = uut.Execute(buildState);
			Assert::AreEqual<int64_t>(0, result, "Verify Execute returned success.");

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
					"DIAG: IncludeModules = ",
					"DIAG: PreprocessorDefinitions = ",
					"INFO: Compiling source files",
					"INFO: Generate Compile Node: TestFile.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Node: bin/Program.exe",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::None;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");
			expectedCompileArguments.GenerateIncludeTree = true;

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetType = LinkTarget::Executable;
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile.mock.obj"),
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

			// Verify build state
			auto expectedLinkNode =
				std::make_shared<BuildGraphNode>(
					"MockLink: 1",
					Path("MockLinker.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}));

			auto expectedCompileNode =
				std::make_shared<BuildGraphNode>(
					"MockCompile: 1",
					Path("MockCompiler.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedLinkNode,
					}));

			auto expectedBuildNodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCompileNode,
					})),
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCompileNode,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}

		[[Fact]]
		void Build_Executable_OptimizeSpeed()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildTask(compiler);

			// Setup the input build state
			auto buildState = BuildState();
			auto state = BuildStateWrapper(buildState);
			state.SetPropertyStringValue("TargetName", "Program");
			state.SetPropertyIntegerValue(
				"TargetType",
				static_cast<int64_t>(BuildTargetType::Executable));
			state.SetPropertyIntegerValue(
				"LanguageStandard",
				static_cast<int64_t>(LanguageStandard::CPP20));
			state.SetPropertyStringValue("WorkingDirectory", "C:/root/");
			state.SetPropertyStringValue("ObjectDirectory", "obj");
			state.SetPropertyStringValue("BinaryDirectory", "bin");
			state.SetPropertyStringList(
				"SourceFiles",
				 std::vector<std::string>({
					"TestFile.cpp",
				}));
			state.SetPropertyStringList("IncludeDirectories", std::vector<std::string>());
			state.SetPropertyStringList("IncludeModules", std::vector<std::string>());
			state.SetPropertyIntegerValue(
				"OptimizationLevel",
				static_cast<int64_t>(BuildOptimizationLevel::Speed));

			auto result = uut.Execute(buildState);
			Assert::AreEqual<int64_t>(0, result, "Verify Execute returned success.");

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
					"DIAG: IncludeModules = ",
					"DIAG: PreprocessorDefinitions = ",
					"INFO: Compiling source files",
					"INFO: Generate Compile Node: TestFile.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Node: bin/Program.exe",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::Speed;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");
			expectedCompileArguments.GenerateIncludeTree = true;

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetType = LinkTarget::Executable;
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile.mock.obj"),
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

			// Verify build state
			auto expectedLinkNode =
				std::make_shared<BuildGraphNode>(
					"MockLink: 1",
					Path("MockLinker.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}));

			auto expectedCompileNode =
				std::make_shared<BuildGraphNode>(
					"MockCompile: 1",
					Path("MockCompiler.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedLinkNode,
					}));

			auto expectedBuildNodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCompileNode,
					})),
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCompileNode,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}

		[[Fact]]
		void Build_Executable_OptimizeSize()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildTask(compiler);

			// Setup the input build state
			auto buildState = BuildState();
			auto state = BuildStateWrapper(buildState);
			state.SetPropertyStringValue("TargetName", "Program");
			state.SetPropertyIntegerValue(
				"TargetType",
				static_cast<int64_t>(BuildTargetType::Executable));
			state.SetPropertyIntegerValue(
				"LanguageStandard",
				static_cast<int64_t>(LanguageStandard::CPP20));
			state.SetPropertyStringValue("WorkingDirectory", "C:/root/");
			state.SetPropertyStringValue("ObjectDirectory", "obj");
			state.SetPropertyStringValue("BinaryDirectory", "bin");
			state.SetPropertyStringList(
				"SourceFiles",
				 std::vector<std::string>({
					"TestFile.cpp",
				}));
			state.SetPropertyStringList("IncludeDirectories", std::vector<std::string>());
			state.SetPropertyStringList("IncludeModules", std::vector<std::string>());
			state.SetPropertyIntegerValue(
				"OptimizationLevel", 
				static_cast<int64_t>(BuildOptimizationLevel::Size));

			auto result = uut.Execute(buildState);
			Assert::AreEqual<int64_t>(0, result, "Verify Execute returned success.");

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
					"DIAG: IncludeModules = ",
					"DIAG: PreprocessorDefinitions = ",
					"INFO: Compiling source files",
					"INFO: Generate Compile Node: TestFile.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Node: bin/Program.exe",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::Size;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");
			expectedCompileArguments.GenerateIncludeTree = true;

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetType = LinkTarget::Executable;
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile.mock.obj"),
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

			// Verify build state
			auto expectedLinkNode =
				std::make_shared<BuildGraphNode>(
					"MockLink: 1",
					Path("MockLinker.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}));

			auto expectedCompileNode =
				std::make_shared<BuildGraphNode>(
					"MockCompile: 1",
					Path("MockCompiler.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedLinkNode,
					}));

			auto expectedBuildNodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCompileNode,
					})),
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCompileNode,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}

		[[Fact]]
		void Build_Library_MultipleFiles()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildTask(compiler);

			// Setup the input build state
			auto buildState = BuildState();
			auto state = BuildStateWrapper(buildState);
			state.SetPropertyStringValue("TargetName", "Library");
			state.SetPropertyIntegerValue(
				"TargetType",
				static_cast<int64_t>(BuildTargetType::StaticLibrary));
			state.SetPropertyIntegerValue(
				"LanguageStandard",
				static_cast<int64_t>(LanguageStandard::CPP20));
			state.SetPropertyStringValue("WorkingDirectory", "C:/root/");
			state.SetPropertyStringValue("ObjectDirectory", "obj");
			state.SetPropertyStringValue("BinaryDirectory", "bin");
			state.SetPropertyStringList(
				"SourceFiles",
				std::vector<std::string>({ 
					"TestFile1.cpp",
					"TestFile2.cpp",
					"TestFile3.cpp",
				}));
			state.SetPropertyStringList(
				"IncludeDirectories", 
				std::vector<std::string>({
					"Folder",
					"AnotherFolder/Sub",
				}));
			state.SetPropertyStringList(
				"IncludeModules",
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.bmi",
					"../OtherModule2.mock.bmi",
				}));
			state.SetPropertyIntegerValue(
				"OptimizationLevel", 
				static_cast<int64_t>(BuildOptimizationLevel::None));
			state.SetPropertyStringList(
				"LinkLibraries",
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.a",
					"../OtherModule2.mock.a",
				}));

			auto result = uut.Execute(buildState);
			Assert::AreEqual<int64_t>(0, result, "Verify Execute returned success.");

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
					"DIAG: IncludeModules = ../Other/bin/OtherModule1.mock.bmi ../OtherModule2.mock.bmi",
					"DIAG: PreprocessorDefinitions = ",
					"INFO: Compiling source files",
					"INFO: Generate Compile Node: TestFile1.cpp",
					"INFO: Generate Compile Node: TestFile2.cpp",
					"INFO: Generate Compile Node: TestFile3.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Node: bin/Library.mock.lib",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

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
			expectedCompileArguments.GenerateIncludeTree = true;

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
			expectedLinkArguments.LibraryFiles = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});

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
			auto expectedLinkNode =
				std::make_shared<BuildGraphNode>(
					"MockLink: 1",
					Path("MockLinker.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}));

			auto expectedCompileNodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
					"MockCompile: 1",
					Path("MockCompiler.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedLinkNode,
					})),
				std::make_shared<BuildGraphNode>(
					"MockCompile: 2",
					Path("MockCompiler.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedLinkNode,
					})),
				std::make_shared<BuildGraphNode>(
					"MockCompile: 3",
					Path("MockCompiler.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedLinkNode,
					})),
			});

			auto expectedBuildNodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					expectedCompileNodes),
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					expectedCompileNodes),
			});

			AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}

		[[Fact]]
		void Build_Library_ModuleInterface()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildTask(compiler);

			// Setup the input build state
			auto buildState = BuildState();
			auto state = BuildStateWrapper(buildState);
			state.SetPropertyStringValue("TargetName", "Library");
			state.SetPropertyIntegerValue(
				"TargetType",
				static_cast<int64_t>(BuildTargetType::StaticLibrary));
			state.SetPropertyIntegerValue(
				"LanguageStandard",
				static_cast<int64_t>(LanguageStandard::CPP20));
			state.SetPropertyStringValue("WorkingDirectory", "C:/root/");
			state.SetPropertyStringValue("ObjectDirectory", "obj");
			state.SetPropertyStringValue("BinaryDirectory", "bin");
			state.SetPropertyStringValue("ModuleInterfaceSourceFile", "Public.cpp");
			state.SetPropertyStringList(
				"SourceFiles",
				std::vector<std::string>({ 
					"TestFile1.cpp",
					"TestFile2.cpp",
					"TestFile3.cpp",
				}));
			state.SetPropertyStringList(
				"IncludeDirectories",
				std::vector<std::string>({
					"Folder",
					"AnotherFolder/Sub",
				}));
			state.SetPropertyStringList(
				"IncludeModules",
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.bmi",
					"../OtherModule2.mock.bmi",
				}));
			state.SetPropertyIntegerValue(
				"OptimizationLevel", 
				static_cast<int64_t>(BuildOptimizationLevel::None));
			state.SetPropertyStringList(
				"LinkLibraries",
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.a",
					"../OtherModule2.mock.a",
				}));
			state.SetPropertyStringList(
				"PreprocessorDefinitions",
				std::vector<std::string>({
					"DEBUG",
					"AWESOME",
				}));

			auto result = uut.Execute(buildState);
			Assert::AreEqual<int64_t>(0, result, "Verify Execute returned success.");

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
					"DIAG: IncludeModules = ../Other/bin/OtherModule1.mock.bmi ../OtherModule2.mock.bmi",
					"DIAG: PreprocessorDefinitions = DEBUG AWESOME",
					"INFO: CompileModuleInterfaceUnit",
					"INFO: Generate Compile Node: Public.cpp",
					"INFO: Compiling source files",
					"INFO: Generate Compile Node: TestFile1.cpp",
					"INFO: Generate Compile Node: TestFile2.cpp",
					"INFO: Generate Compile Node: TestFile3.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Node: bin/Library.mock.lib",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

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
			expectedCompileArguments.GenerateIncludeTree = true;
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
			expectedLinkArguments.LibraryFiles = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});

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
			auto expectedLinkNode =
				std::make_shared<BuildGraphNode>(
					"MockLink: 1",
					Path("MockLinker.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}));

			auto expectedCompileSourceNodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
					"MockCompile: 2",
					Path("MockCompiler.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedLinkNode,
					})),
				std::make_shared<BuildGraphNode>(
					"MockCompile: 3",
					Path("MockCompiler.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedLinkNode,
					})),
				std::make_shared<BuildGraphNode>(
					"MockCompile: 4",
					Path("MockCompiler.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedLinkNode,
					})),
			});

			auto expectedCopyModuleInterfaceNode =
				std::make_shared<BuildGraphNode>(
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
					expectedCompileSourceNodes);

			auto expectedCompileModuleNode =
				std::make_shared<BuildGraphNode>(
					"MockCompile: 1",
					Path("MockCompiler.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCopyModuleInterfaceNode,
					}));

			auto expectedBuildNodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}

		[[Fact]]
		void Build_Library_ModuleInterfaceNoSource()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildTask(compiler);

			// Setup the input build state
			auto buildState = BuildState();
			auto state = BuildStateWrapper(buildState);
			state.SetPropertyStringValue("TargetName", "Library");
			state.SetPropertyIntegerValue(
				"TargetType", 
				static_cast<int64_t>(BuildTargetType::StaticLibrary));
			state.SetPropertyIntegerValue(
				"LanguageStandard",  
				static_cast<int64_t>(LanguageStandard::CPP20));
			state.SetPropertyStringValue("WorkingDirectory", "C:/root/");
			state.SetPropertyStringValue("ObjectDirectory", "obj");
			state.SetPropertyStringValue("BinaryDirectory", "bin");
			state.SetPropertyStringValue("ModuleInterfaceSourceFile", "Public.cpp");
			state.SetPropertyStringList("SourceFiles", std::vector<std::string>({}));
			state.SetPropertyStringList(
				"IncludeDirectories", 
				std::vector<std::string>({
					"Folder",
					"AnotherFolder/Sub",
				}));
			state.SetPropertyStringList(
				"IncludeModules",
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.bmi",
					"../OtherModule2.mock.bmi",
				}));
			state.SetPropertyIntegerValue(
				"OptimizationLevel",
				static_cast<int64_t>(BuildOptimizationLevel::None));
			state.SetPropertyStringList(
				"LinkLibraries",
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.a",
					"../OtherModule2.mock.a",
				}));

			auto result = uut.Execute(buildState);
			Assert::AreEqual<int64_t>(0, result, "Verify Execute returned success.");

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
					"DIAG: IncludeModules = ../Other/bin/OtherModule1.mock.bmi ../OtherModule2.mock.bmi",
					"DIAG: PreprocessorDefinitions = ",
					"INFO: CompileModuleInterfaceUnit",
					"INFO: Generate Compile Node: Public.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Node: bin/Library.mock.lib",
					"INFO: Build Generate Done",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

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
			expectedCompileArguments.GenerateIncludeTree = true;

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
			expectedLinkArguments.LibraryFiles = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});

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
			auto expectedLinkNode =
				std::make_shared<BuildGraphNode>(
					"MockLink: 1",
					Path("MockLinker.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}));

			auto expectedCopyModuleInterfaceNode =
				std::make_shared<BuildGraphNode>(
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
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedLinkNode,
					}));

			auto expectedCompileModuleNode =
				std::make_shared<BuildGraphNode>(
					"MockCompile: 1",
					Path("MockCompiler.exe"),
					"Arguments",
					Path("MockWorkingDirectory"),
					std::vector<Path>({
						Path("InputFile.in"),
					}),
					std::vector<Path>({
						Path("OutputFile.out"),
					}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCopyModuleInterfaceNode,
					}));

			auto expectedBuildNodes = std::vector<std::shared_ptr<BuildGraphNode>>({
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
				std::make_shared<BuildGraphNode>(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<std::shared_ptr<BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}
	};
}
