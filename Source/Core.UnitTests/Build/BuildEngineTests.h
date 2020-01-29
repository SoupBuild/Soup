// <copyright file="BuildEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
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

			// Setup the build arguments
			auto arguments = BuildArguments();
			arguments.TargetName = "Program";
			arguments.TargetType = BuildTargetType::Executable;
			arguments.LanguageStandard = LanguageStandard::CPP20;
			arguments.WorkingDirectory = Path("C:/root/");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.SourceFiles = std::vector<Path>({
				Path("TestFile.cpp"),
			});
			arguments.OptimizationLevel = BuildOptimizationLevel::None;
			arguments.LinkDependencies = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});

			auto uut = BuildEngine(compiler);
			auto buildState = BuildState();
			auto result = uut.Execute(BuildStateWrapper(buildState), arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Compiling source files",
					"INFO: Generate Compile Node: TestFile.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Node: bin/Program.exe",
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

			// Verify build state
			auto expectedLinkNode =
				Memory::Reference<BuildGraphNode>(
					new BuildGraphNode(
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
				Memory::Reference<BuildGraphNode>(
					new BuildGraphNode(
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
						std::vector<Memory::Reference<BuildGraphNode>>({
							expectedLinkNode,
						})));

			auto expectedBuildNodes = std::vector<Memory::Reference<BuildGraphNode>>({
				new BuildGraphNode(
					"MakeDir [C:/root/obj]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({
						"C:/root/obj",
					}),
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedCompileNode,
					})),
				new BuildGraphNode(
					"MakeDir [C:/root/bin]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({
						"C:/root/bin",
					}),
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedCompileNode,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildNodes,
				result.BuildNodes);

			Assert::AreEqual(
				std::vector<Path>({}),
				result.ModuleDependencies,
				"Verify Module Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({}),
				result.LinkDependencies,
				"Verify Link Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({}),
				result.RuntimeDependencies,
				"Verify Runtime Dependencies Result");
		}

		[[Fact]]
		void Build_Library_MultipleFiles()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the mock compiler
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();

			// Setup the build arguments
			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::StaticLibrary;
			arguments.LanguageStandard = LanguageStandard::CPP20;
			arguments.WorkingDirectory = Path("C:/root/");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
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
			auto buildState = BuildState();
			auto result = uut.Execute(BuildStateWrapper(buildState), arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Compiling source files",
					"INFO: Generate Compile Node: TestFile1.cpp",
					"INFO: Generate Compile Node: TestFile2.cpp",
					"INFO: Generate Compile Node: TestFile3.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Node: bin/Library.mock.lib",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

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
			auto expectedLinkNode =
				Memory::Reference<BuildGraphNode>(
					new BuildGraphNode(
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

			auto expectedCompileNodes = std::vector<Memory::Reference<BuildGraphNode>>({
				new BuildGraphNode(
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
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedLinkNode,
					})),
				new BuildGraphNode(
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
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedLinkNode,
					})),
				new BuildGraphNode(
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
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedLinkNode,
					})),
			});

			auto expectedBuildNodes = std::vector<Memory::Reference<BuildGraphNode>>({
				new BuildGraphNode(
					"MakeDir [C:/root/obj]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({
						"C:/root/obj",
					}),
					expectedCompileNodes),
				new BuildGraphNode(
					"MakeDir [C:/root/bin]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({
						"C:/root/bin",
					}),
					expectedCompileNodes),
			});

			AssertExtensions::AreEqual(
				expectedBuildNodes,
				result.BuildNodes);

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

			// Register the mock compiler
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();

			// Setup the build arguments
			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::StaticLibrary;
			arguments.LanguageStandard = LanguageStandard::CPP20;
			arguments.WorkingDirectory = Path("C:/root/");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
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
			auto buildState = BuildState();
			auto result = uut.Execute(BuildStateWrapper(buildState), arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: CompileModuleInterfaceUnit",
					"INFO: Generate Compile Node: Public.cpp",
					"INFO: Compiling source files",
					"INFO: Generate Compile Node: TestFile1.cpp",
					"INFO: Generate Compile Node: TestFile2.cpp",
					"INFO: Generate Compile Node: TestFile3.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Node: bin/Library.mock.lib",
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
			auto expectedLinkNode =
				Memory::Reference<BuildGraphNode>(
					new BuildGraphNode(
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

			auto expectedCompileSourceNodes = std::vector<Memory::Reference<BuildGraphNode>>({
				new BuildGraphNode(
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
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedLinkNode,
					})),
				new BuildGraphNode(
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
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedLinkNode,
					})),
				new BuildGraphNode(
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
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedLinkNode,
					})),
			});

			auto expectedCopyModuleInterfaceNode =
				Memory::Reference<BuildGraphNode>(
					new BuildGraphNode(
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
				Memory::Reference<BuildGraphNode>(
					new BuildGraphNode(
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
						std::vector<Memory::Reference<BuildGraphNode>>({
							expectedCopyModuleInterfaceNode,
						})));

			auto expectedBuildNodes = std::vector<Memory::Reference<BuildGraphNode>>({
				new BuildGraphNode(
					"MakeDir [C:/root/obj]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({
						"C:/root/obj",
					}),
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
				new BuildGraphNode(
					"MakeDir [C:/root/bin]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({
						"C:/root/bin",
					}),
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildNodes,
				result.BuildNodes);

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

			// Register the mock compiler
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();

			// Setup the build arguments
			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::StaticLibrary;
			arguments.LanguageStandard = LanguageStandard::CPP20;
			arguments.WorkingDirectory = Path("C:/root/");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
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
			auto buildState = BuildState();
			auto result = uut.Execute(BuildStateWrapper(buildState), arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: CompileModuleInterfaceUnit",
					"INFO: Generate Compile Node: Public.cpp",
					"INFO: CoreLink",
					"INFO: Linking target",
					"INFO: Generate Link Node: bin/Library.mock.lib",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

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
			auto expectedLinkNode =
				Memory::Reference<BuildGraphNode>(
					new BuildGraphNode(
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
				Memory::Reference<BuildGraphNode>(
					new BuildGraphNode(
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
						std::vector<Memory::Reference<BuildGraphNode>>({
							expectedLinkNode,
						})));

			auto expectedCompileModuleNode =
				Memory::Reference<BuildGraphNode>(
					new BuildGraphNode(
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
						std::vector<Memory::Reference<BuildGraphNode>>({
							expectedCopyModuleInterfaceNode,
						})));

			auto expectedBuildNodes = std::vector<Memory::Reference<BuildGraphNode>>({
				new BuildGraphNode(
					"MakeDir [C:/root/obj]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({
						"C:/root/obj",
					}),
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
				new BuildGraphNode(
					"MakeDir [C:/root/bin]",
					"C:/Windows/System32/cmd.exe",
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					"./",
					std::vector<std::string>({}),
					std::vector<std::string>({
						"C:/root/bin",
					}),
					std::vector<Memory::Reference<BuildGraphNode>>({
						expectedCompileModuleNode,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildNodes,
				result.BuildNodes);

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
