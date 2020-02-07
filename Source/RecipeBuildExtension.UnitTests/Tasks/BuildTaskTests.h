// <copyright file="BuildTaskTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace RecipeBuild::UnitTests
{
	class BuildTaskTests
	{
	public:
		[[Fact]]
		void Initialize_Success()
		{
			auto compilerFactory = CompilerFactory();
			auto uut = BuildTask(compilerFactory);
			
			Assert::AreEqual("Build", uut.GetName(), "Verify name matches expected.");
		}

		[[Fact]]
		void Build_Executable()
		{
			// Register the test listener
			auto testListener = std::make_shared<Soup::TestTraceListener>();
			auto scopedTraceListener = Soup::ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Soup::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Soup::Build::PropertyBagWrapper& state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto buildState = Soup::Build::BuildState();
			auto state = Soup::Build::PropertyBagWrapper(buildState.GetActiveState());
			state.SetPropertyStringValue("TargetName", "Program");
			state.SetPropertyIntegerValue(
				"TargetType",
				static_cast<int64_t>(BuildTargetType::Executable));
			state.SetPropertyIntegerValue(
				"LanguageStandard",
				static_cast<int64_t>(Soup::LanguageStandard::CPP20));
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

			auto expectedCompileArguments = Soup::CompileArguments();
			expectedCompileArguments.Standard = Soup::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Soup::OptimizationLevel::None;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");
			expectedCompileArguments.GenerateIncludeTree = true;

			auto expectedLinkArguments = Soup::LinkArguments();
			expectedLinkArguments.TargetType = Soup::LinkTarget::Executable;
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile.mock.obj"),
			});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<Soup::CompileArguments>({
					expectedCompileArguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Soup::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedLinkNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"MockLink: 1",
						"MockLinker.exe",
						"Arguments",
						"MockWorkingDirectory",
						std::vector<std::string>({
							"InputFile.in",
						}),
						std::vector<std::string>({
							"OutputFile.out",
						})));

			auto expectedCompileNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"MockCompile: 1",
						"MockCompiler.exe",
						"Arguments",
						"MockWorkingDirectory",
						std::vector<std::string>({
							"InputFile.in",
						}),
						std::vector<std::string>({
							"OutputFile.out",
						}),
						std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
							expectedLinkNode,
						})));

			auto expectedBuildNodes = std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/obj]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedCompileNode,
					})),
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/bin]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedCompileNode,
					})),
			});

			Soup::AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}

		[[Fact]]
		void Build_Executable_OptimizeSpeed()
		{
			// Register the test listener
			auto testListener = std::make_shared<Soup::TestTraceListener>();
			auto scopedTraceListener = Soup::ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Soup::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Soup::Build::PropertyBagWrapper& state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto buildState = Soup::Build::BuildState();
			auto state = Soup::Build::PropertyBagWrapper(buildState.GetActiveState());
			state.SetPropertyStringValue("TargetName", "Program");
			state.SetPropertyIntegerValue(
				"TargetType",
				static_cast<int64_t>(BuildTargetType::Executable));
			state.SetPropertyIntegerValue(
				"LanguageStandard",
				static_cast<int64_t>(Soup::LanguageStandard::CPP20));
			state.SetPropertyStringValue("WorkingDirectory", "C:/root/");
			state.SetPropertyStringValue("ObjectDirectory", "obj");
			state.SetPropertyStringValue("BinaryDirectory", "bin");
			state.SetPropertyStringList(
				"SourceFiles",
				 std::vector<std::string>({
					"TestFile.cpp",
				}));
			state.SetPropertyStringList("IncludeDirectories", std::vector<std::string>());
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

			auto expectedCompileArguments = Soup::CompileArguments();
			expectedCompileArguments.Standard = Soup::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Soup::OptimizationLevel::Speed;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");
			expectedCompileArguments.GenerateIncludeTree = true;

			auto expectedLinkArguments = Soup::LinkArguments();
			expectedLinkArguments.TargetType = Soup::LinkTarget::Executable;
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile.mock.obj"),
			});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<Soup::CompileArguments>({
					expectedCompileArguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Soup::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedLinkNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"MockLink: 1",
						"MockLinker.exe",
						"Arguments",
						"MockWorkingDirectory",
						std::vector<std::string>({
							"InputFile.in",
						}),
						std::vector<std::string>({
							"OutputFile.out",
						})));

			auto expectedCompileNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"MockCompile: 1",
						"MockCompiler.exe",
						"Arguments",
						"MockWorkingDirectory",
						std::vector<std::string>({
							"InputFile.in",
						}),
						std::vector<std::string>({
							"OutputFile.out",
						}),
						std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
							expectedLinkNode,
						})));

			auto expectedBuildNodes = std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/obj]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedCompileNode,
					})),
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/bin]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedCompileNode,
					})),
			});

			Soup::AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}

		[[Fact]]
		void Build_Executable_OptimizeSize()
		{
			// Register the test listener
			auto testListener = std::make_shared<Soup::TestTraceListener>();
			auto scopedTraceListener = Soup::ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Soup::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Soup::Build::PropertyBagWrapper& state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto buildState = Soup::Build::BuildState();
			auto state = Soup::Build::PropertyBagWrapper(buildState.GetActiveState());
			state.SetPropertyStringValue("TargetName", "Program");
			state.SetPropertyIntegerValue(
				"TargetType",
				static_cast<int64_t>(BuildTargetType::Executable));
			state.SetPropertyIntegerValue(
				"LanguageStandard",
				static_cast<int64_t>(Soup::LanguageStandard::CPP20));
			state.SetPropertyStringValue("WorkingDirectory", "C:/root/");
			state.SetPropertyStringValue("ObjectDirectory", "obj");
			state.SetPropertyStringValue("BinaryDirectory", "bin");
			state.SetPropertyStringList(
				"SourceFiles",
				 std::vector<std::string>({
					"TestFile.cpp",
				}));
			state.SetPropertyStringList("IncludeDirectories", std::vector<std::string>());
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

			auto expectedCompileArguments = Soup::CompileArguments();
			expectedCompileArguments.Standard = Soup::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Soup::OptimizationLevel::Size;
			expectedCompileArguments.RootDirectory = Path("C:/root/");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");
			expectedCompileArguments.GenerateIncludeTree = true;

			auto expectedLinkArguments = Soup::LinkArguments();
			expectedLinkArguments.TargetType = Soup::LinkTarget::Executable;
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.RootDirectory = Path("C:/root/");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile.mock.obj"),
			});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<Soup::CompileArguments>({
					expectedCompileArguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Soup::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedLinkNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"MockLink: 1",
						"MockLinker.exe",
						"Arguments",
						"MockWorkingDirectory",
						std::vector<std::string>({
							"InputFile.in",
						}),
						std::vector<std::string>({
							"OutputFile.out",
						})));

			auto expectedCompileNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"MockCompile: 1",
						"MockCompiler.exe",
						"Arguments",
						"MockWorkingDirectory",
						std::vector<std::string>({
							"InputFile.in",
						}),
						std::vector<std::string>({
							"OutputFile.out",
						}),
						std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
							expectedLinkNode,
						})));

			auto expectedBuildNodes = std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/obj]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedCompileNode,
					})),
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/bin]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedCompileNode,
					})),
			});

			Soup::AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}

		[[Fact]]
		void Build_Library_MultipleFiles()
		{
			// Register the test listener
			auto testListener = std::make_shared<Soup::TestTraceListener>();
			auto scopedTraceListener = Soup::ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Soup::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Soup::Build::PropertyBagWrapper& state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto buildState = Soup::Build::BuildState();
			auto state = Soup::Build::PropertyBagWrapper(buildState.GetActiveState());
			state.SetPropertyStringValue("TargetName", "Library");
			state.SetPropertyIntegerValue(
				"TargetType",
				static_cast<int64_t>(BuildTargetType::StaticLibrary));
			state.SetPropertyIntegerValue(
				"LanguageStandard",
				static_cast<int64_t>(Soup::LanguageStandard::CPP20));
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
				"ModuleDependencies",
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.bmi",
					"../OtherModule2.mock.bmi",
				}));
			state.SetPropertyIntegerValue(
				"OptimizationLevel", 
				static_cast<int64_t>(BuildOptimizationLevel::None));
			state.SetPropertyStringList(
				"LinkDependencies",
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
			auto expectedCompileArguments = Soup::CompileArguments();
			expectedCompileArguments.Standard = Soup::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Soup::OptimizationLevel::None;
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

			auto expectedLinkArguments = Soup::LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Library.mock.lib");
			expectedLinkArguments.TargetType = Soup::LinkTarget::StaticLibrary;
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
				std::vector<Soup::CompileArguments>({
					expectedCompile1Arguments,
					expectedCompile2Arguments,
					expectedCompile3Arguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Soup::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedLinkNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"MockLink: 1",
						"MockLinker.exe",
						"Arguments",
						"MockWorkingDirectory",
						std::vector<std::string>({
							"InputFile.in",
						}),
						std::vector<std::string>({
							"OutputFile.out",
						})));

			auto expectedCompileNodes = std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
				new Soup::Build::BuildGraphNode(
					"MockCompile: 1",
					"MockCompiler.exe",
					"Arguments",
					"MockWorkingDirectory",
					std::vector<std::string>({
						"InputFile.in",
					}),
					std::vector<std::string>({
						"OutputFile.out",
					}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedLinkNode,
					})),
				new Soup::Build::BuildGraphNode(
					"MockCompile: 2",
					"MockCompiler.exe",
					"Arguments",
					"MockWorkingDirectory",
					std::vector<std::string>({
						"InputFile.in",
					}),
					std::vector<std::string>({
						"OutputFile.out",
					}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedLinkNode,
					})),
				new Soup::Build::BuildGraphNode(
					"MockCompile: 3",
					"MockCompiler.exe",
					"Arguments",
					"MockWorkingDirectory",
					std::vector<std::string>({
						"InputFile.in",
					}),
					std::vector<std::string>({
						"OutputFile.out",
					}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedLinkNode,
					})),
			});

			auto expectedBuildNodes = std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/obj]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					expectedCompileNodes),
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/bin]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					expectedCompileNodes),
			});

			Soup::AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}

		[[Fact]]
		void Build_Library_ModuleInterface()
		{
			// Register the test listener
			auto testListener = std::make_shared<Soup::TestTraceListener>();
			auto scopedTraceListener = Soup::ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Soup::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Soup::Build::PropertyBagWrapper& state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto buildState = Soup::Build::BuildState();
			auto state = Soup::Build::PropertyBagWrapper(buildState.GetActiveState());
			state.SetPropertyStringValue("TargetName", "Library");
			state.SetPropertyIntegerValue(
				"TargetType",
				static_cast<int64_t>(BuildTargetType::StaticLibrary));
			state.SetPropertyIntegerValue(
				"LanguageStandard",
				static_cast<int64_t>(Soup::LanguageStandard::CPP20));
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
				"ModuleDependencies",
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.bmi",
					"../OtherModule2.mock.bmi",
				}));
			state.SetPropertyIntegerValue(
				"OptimizationLevel", 
				static_cast<int64_t>(BuildOptimizationLevel::None));
			state.SetPropertyStringList(
				"LinkDependencies",
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
			auto expectedCompileArguments = Soup::CompileArguments();
			expectedCompileArguments.Standard = Soup::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Soup::OptimizationLevel::None;
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

			auto expectedLinkArguments = Soup::LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Library.mock.lib");
			expectedLinkArguments.TargetType = Soup::LinkTarget::StaticLibrary;
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
				std::vector<Soup::CompileArguments>({
					expectedCompileModuleArguments,
					expectedCompile1Arguments,
					expectedCompile2Arguments,
					expectedCompile3Arguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Soup::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedLinkNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"MockLink: 1",
						"MockLinker.exe",
						"Arguments",
						"MockWorkingDirectory",
						std::vector<std::string>({
							"InputFile.in",
						}),
						std::vector<std::string>({
							"OutputFile.out",
						})));

			auto expectedCompileSourceNodes = std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
				new Soup::Build::BuildGraphNode(
					"MockCompile: 2",
					"MockCompiler.exe",
					"Arguments",
					"MockWorkingDirectory",
					std::vector<std::string>({
						"InputFile.in",
					}),
					std::vector<std::string>({
						"OutputFile.out",
					}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedLinkNode,
					})),
				new Soup::Build::BuildGraphNode(
					"MockCompile: 3",
					"MockCompiler.exe",
					"Arguments",
					"MockWorkingDirectory",
					std::vector<std::string>({
						"InputFile.in",
					}),
					std::vector<std::string>({
						"OutputFile.out",
					}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedLinkNode,
					})),
				new Soup::Build::BuildGraphNode(
					"MockCompile: 4",
					"MockCompiler.exe",
					"Arguments",
					"MockWorkingDirectory",
					std::vector<std::string>({
						"InputFile.in",
					}),
					std::vector<std::string>({
						"OutputFile.out",
					}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedLinkNode,
					})),
			});

			auto expectedCopyModuleInterfaceNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"Copy [C:/root/obj/Public.mock.bmi] -> [C:/root/bin/Library.mock.bmi]",
						"C:/Windows/System32/cmd.exe",
						"/C copy /Y \"C:\\root\\obj\\Public.mock.bmi\" \"C:\\root\\bin\\Library.mock.bmi\"",
						"./",
						std::vector<std::string>({
							"C:/root/obj/Public.mock.bmi",
						}),
						std::vector<std::string>({
							"C:/root/bin/Library.mock.bmi",
						}),
						expectedCompileSourceNodes));

			auto expectedCompileModuleNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"MockCompile: 1",
						"MockCompiler.exe",
						"Arguments",
						"MockWorkingDirectory",
						std::vector<std::string>({
							"InputFile.in",
						}),
						std::vector<std::string>({
							"OutputFile.out",
						}),
						std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
							expectedCopyModuleInterfaceNode,
						})));

			auto expectedBuildNodes = std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/obj]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/bin]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
			});

			Soup::AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}

		[[Fact]]
		void Build_Library_ModuleInterfaceNoSource()
		{
			// Register the test listener
			auto testListener = std::make_shared<Soup::TestTraceListener>();
			auto scopedTraceListener = Soup::ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Soup::Compiler::Mock::Compiler>();
			auto compilerFactory = CompilerFactory();
			compilerFactory.emplace("MOCK", [compiler](Soup::Build::PropertyBagWrapper& state) { return compiler; });

			auto uut = BuildTask(compilerFactory);

			// Setup the input build state
			auto buildState = Soup::Build::BuildState();
			auto state = Soup::Build::PropertyBagWrapper(buildState.GetActiveState());
			state.SetPropertyStringValue("TargetName", "Library");
			state.SetPropertyIntegerValue(
				"TargetType", 
				static_cast<int64_t>(BuildTargetType::StaticLibrary));
			state.SetPropertyIntegerValue(
				"LanguageStandard",
				static_cast<int64_t>(Soup::LanguageStandard::CPP20));
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
				"ModuleDependencies",
				std::vector<std::string>({
					"../Other/bin/OtherModule1.mock.bmi",
					"../OtherModule2.mock.bmi",
				}));
			state.SetPropertyIntegerValue(
				"OptimizationLevel",
				static_cast<int64_t>(BuildOptimizationLevel::None));
			state.SetPropertyStringList(
				"LinkDependencies",
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
			auto expectedCompileArguments = Soup::CompileArguments();
			expectedCompileArguments.Standard = Soup::LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = Soup::OptimizationLevel::None;
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

			auto expectedLinkArguments = Soup::LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Library.mock.lib");
			expectedLinkArguments.TargetType = Soup::LinkTarget::StaticLibrary;
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
				std::vector<Soup::CompileArguments>({
					expectedCompileModuleArguments,
				}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<Soup::LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify build state
			auto expectedLinkNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"MockLink: 1",
						"MockLinker.exe",
						"Arguments",
						"MockWorkingDirectory",
						std::vector<std::string>({
							"InputFile.in",
						}),
						std::vector<std::string>({
							"OutputFile.out",
						})));

			auto expectedCopyModuleInterfaceNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"Copy [C:/root/obj/Public.mock.bmi] -> [C:/root/bin/Library.mock.bmi]",
						"C:/Windows/System32/cmd.exe",
						"/C copy /Y \"C:\\root\\obj\\Public.mock.bmi\" \"C:\\root\\bin\\Library.mock.bmi\"",
						"./",
						std::vector<std::string>({
							"C:/root/obj/Public.mock.bmi",
						}),
						std::vector<std::string>({
							"C:/root/bin/Library.mock.bmi",
						}),
						std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
							expectedLinkNode,
						})));

			auto expectedCompileModuleNode =
				Memory::Reference<Soup::Build::BuildGraphNode>(
					new Soup::Build::BuildGraphNode(
						"MockCompile: 1",
						"MockCompiler.exe",
						"Arguments",
						"MockWorkingDirectory",
						std::vector<std::string>({
							"InputFile.in",
						}),
						std::vector<std::string>({
							"OutputFile.out",
						}),
						std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
							expectedCopyModuleInterfaceNode,
						})));

			auto expectedBuildNodes = std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/obj]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
				new Soup::Build::BuildGraphNode(
					"MakeDir [C:/root/bin]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({}),
					std::vector<Memory::Reference<Soup::Build::BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
			});

			Soup::AssertExtensions::AreEqual(
				expectedBuildNodes,
				buildState.GetBuildNodes());
		}
	};
}
