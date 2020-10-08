// <copyright file="BuildTaskTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp::UnitTests
{
	class BuildTaskTests
	{
	public:
		[[Fact]]
		void Initialize_Success()
		{
			auto uut = BuildTask();
			
			Assert::AreEqual("Build", uut.GetName(), "Verify name matches expected.");
		}

		[[Fact]]
		void Build_Executable()
		{
			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = Opal::ScopedTraceListenerRegister(testListener);

			auto uut = BuildTask();

			// Setup the input build state
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());
			state.CreateValue("TargetName").SetValueString("Program");
			state.CreateValue("TargetType").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildTargetType::Executable));
			state.CreateValue("WorkingDirectory").SetValueString("C:/root/");
			state.CreateValue("ObjectDirectory").SetValueString("obj");
			state.CreateValue("BinaryDirectory").SetValueString("bin");
			state.CreateValue("SourceFiles").SetValueStringList(
				std::vector<std::string>({
					"TestFile.cpp",
				}));
			state.CreateValue("IncludeDirectories").SetValueStringList(std::vector<std::string>());
			state.CreateValue("IncludeModules").SetValueStringList(std::vector<std::string>());
			state.CreateValue("EnableOptimization").SetValueBoolean(false);

			auto result = uut.TryExecute(buildState);
			Assert::AreEqual(Build::ApiCallResult::Success, result, "Verify TryExecute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: TargetName = Program",
					"DIAG: TargetType = Executable",
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

			// Verify build state
			auto expectedLinkOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
						"MockLink: 1",
						Path("MockLinker.exe"),
						"Arguments",
						Path("MockWorkingDirectory"),
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						})));

			auto expectedCompileOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
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
						std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
							expectedLinkOperation,
						})));

			auto expectedBuildOperations = std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
						expectedCompileOperation,
					})),
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
						expectedCompileOperation,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				Build::Utilities::BuildOperationListWrapper(buildState.GetRootOperationList()));
		}

		[[Fact]]
		void Build_Executable_Optimize()
		{
			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto uut = BuildTask();

			// Setup the input build state
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());
			state.CreateValue("TargetName").SetValueString("Program");
			state.CreateValue("TargetType").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildTargetType::Executable));
			state.CreateValue("WorkingDirectory").SetValueString("C:/root/");
			state.CreateValue("ObjectDirectory").SetValueString("obj");
			state.CreateValue("BinaryDirectory").SetValueString("bin");
			state.CreateValue("SourceFiles").SetValueStringList(
				std::vector<std::string>({
					"TestFile.cpp",
				}));
			state.CreateValue("IncludeDirectories").SetValueStringList(std::vector<std::string>());
			state.CreateValue("IncludeModules").SetValueStringList(std::vector<std::string>());
			state.CreateValue("EnableOptimization").SetValueBoolean(true);

			auto result = uut.TryExecute(buildState);
			Assert::AreEqual(Build::ApiCallResult::Success, result, "Verify TryExecute returned success.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"DIAG: TargetName = Program",
					"DIAG: TargetType = Executable",
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

			// Verify build state
			auto expectedLinkOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
						"MockLink: 1",
						Path("MockLinker.exe"),
						"Arguments",
						Path("MockWorkingDirectory"),
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						})));

			auto expectedCompileOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
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
						std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
							expectedLinkOperation,
						})));

			auto expectedBuildOperations = std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/obj]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/obj\" mkdir \"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
						expectedCompileOperation,
					})),
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/bin]",
					Path("C:/Windows/System32/cmd.exe"),
					"/C if not exist \"C:/root/bin\" mkdir \"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({}),
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
						expectedCompileOperation,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				Build::Utilities::BuildOperationListWrapper(buildState.GetRootOperationList()));
		}

		[[Fact]]
		void Build_Library_MultipleFiles()
		{
			// Register the test listener
			auto testListener = std::make_shared<Opal::TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			auto uut = BuildTask();

			// Setup the input build state
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto state = Build::Utilities::ValueTableWrapper(buildState.GetActiveState());
			state.CreateValue("TargetName").SetValueString("Library");
			state.CreateValue("TargetType").SetValueInteger(
				static_cast<int64_t>(Compiler::BuildTargetType::Library));
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
			state.CreateValue("EnableOptimization").SetValueInteger(false);
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

			// Verify build state
			auto expectedLinkOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
						"MockLink: 1",
						Path("MockLinker.exe"),
						"Arguments",
						Path("MockWorkingDirectory"),
						std::vector<Path>({
							Path("InputFile.in"),
						}),
						std::vector<Path>({
							Path("OutputFile.out"),
						})));

			auto expectedCompileOperations = std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
				new Build::Utilities::BuildOperation(
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
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
						expectedLinkOperation,
					})),
				new Build::Utilities::BuildOperation(
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
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
						expectedLinkOperation,
					})),
				new Build::Utilities::BuildOperation(
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
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
						expectedLinkOperation,
					})),
			});

			auto expectedBuildOperations = std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
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
	};
}
