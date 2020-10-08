// <copyright file="BuildEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::CSharp::Compiler::UnitTests
{
	class BuildEngineTests
	{
	public:
		[[Fact]]
		void Initialize_Success()
		{
			auto compiler = std::make_shared<Soup::CSharp::Compiler::Compiler>(
				Path("C:/Tools/"),
				Path("csc.test.exe"));
			auto uut = BuildEngine(compiler);
		}

		[[Fact]]
		void Build_Executable()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Register the compiler
			auto compiler = std::make_shared<Soup::CSharp::Compiler::Compiler>(
				Path("C:/Tools/"),
				Path("csc.test.exe"));

			// Setup the build arguments
			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::Library;
			arguments.WorkingDirectory = Path("C:/root/");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.SourceFiles = std::vector<Path>({
				Path("TestFile1.cs"),
				Path("TestFile2.cs"),
				Path("TestFile3.cs"),
			});
			arguments.EnableOptimization = true;

			auto uut = BuildEngine(compiler);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto result = uut.Execute(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Generate compile source files",
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

			// Verify build state
			auto expectedCompileOperations = std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
				new Build::Utilities::BuildOperation(
					"CoreCompile",
					Path("C:/Tools/csc.test.exe"),
					"/noconfig /unsafe- /checked- /fullpaths /nostdlib+ /errorreport:prompt /warn:4 /errorendlocation /preferreduilang:en-US /highentropyva+ /debug- /debug:portable /filealign:512 /optimize+ /out:./obj/Library /target:library /warnaserror+ /utf8output /deterministic+ /langversion:8.0 ./TestFile1.cs ./TestFile2.cs ./TestFile3.cs",
					Path("C:/root/"),
					std::vector<Path>({
					}),
					std::vector<Path>({
					}),
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
					})),
			});

			auto expectedBuildOperations = std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/obj]",
					Path("C:/testlocation/mkdir.exe"),
					"\"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({
						Path("C:/root/obj"),
					}),
					expectedCompileOperations),
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/bin]",
					Path("C:/testlocation/mkdir.exe"),
					"\"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({
						Path("C:/root/bin"),
					}),
					expectedCompileOperations),
			});

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				result.BuildOperations);

			Assert::AreEqual(
				std::vector<Path>({
				}),
				result.LinkDependencies,
				"Verify Link Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({}),
				result.RuntimeDependencies,
				"Verify Runtime Dependencies Result");
		}

		[[Fact]]
		void Build_Library_NoSource()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			auto scopedTraceListener = ScopedTraceListenerRegister(testListener);

			// Register the compiler
			auto compiler = std::make_shared<Soup::CSharp::Compiler::Compiler>(
				Path("C:/Tools/"),
				Path("csc.test.exe"));

			// Register the test process manager
			auto processManager = std::make_shared<MockProcessManager>();
			auto scopedProcesManager = ScopedProcessManagerRegister(processManager);

			// Setup the build arguments
			auto arguments = BuildArguments();
			arguments.TargetName = "Program";
			arguments.TargetType = BuildTargetType::Executable;
			arguments.WorkingDirectory = Path("C:/root/");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.SourceFiles = std::vector<Path>({
			});
			arguments.EnableOptimization = false;

			auto uut = BuildEngine(compiler);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto result = uut.Execute(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
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

			// Verify build state
			auto expectedBuildOperations = std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/obj]",
					Path("C:/testlocation/mkdir.exe"),
					"\"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({
						Path("C:/root/obj"),
					}),
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
					})),
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/bin]",
					Path("C:/testlocation/mkdir.exe"),
					"\"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({
						Path("C:/root/bin"),
					}),
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				result.BuildOperations);

			Assert::AreEqual(
				std::vector<Path>({}),
				result.LinkDependencies,
				"Verify Link Dependencies Result");

			Assert::AreEqual(
				std::vector<Path>({
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

			// Register the compiler
			auto compiler = std::make_shared<Soup::CSharp::Compiler::Compiler>(
				Path("C:/Tools/"),
				Path("csc.test.exe"));

			// Setup the build arguments
			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::Library;
			arguments.WorkingDirectory = Path("C:/root/");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.SourceFiles = std::vector<Path>({
				Path("File1.cs"),
				Path("File2.cs"),
			});
			arguments.EnableOptimization = true;
			arguments.LibraryPaths = std::vector<Path>({
				Path("../Other/bin/OtherModule1.mock.dll"),
				Path("../OtherModule2.mock.dll"),
			});

			auto uut = BuildEngine(compiler);
			auto buildState = Build::Runtime::BuildState(Build::Runtime::ValueTable());
			auto result = uut.Execute(Build::Utilities::BuildStateWrapper(buildState), arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"INFO: Generate compile source files",
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

			// Verify build state
			auto expectedCompileOperation =
				Memory::Reference<Build::Utilities::BuildOperation>(
					new Build::Utilities::BuildOperation(
						"CoreCompile",
						Path("C:/Tools/csc.test.exe"),
						"/noconfig /unsafe- /checked- /fullpaths /nostdlib+ /errorreport:prompt /warn:4 /errorendlocation /preferreduilang:en-US /highentropyva+ /debug- /debug:portable /filealign:512 /optimize+ /out:./obj/Library /target:library /warnaserror+ /utf8output /deterministic+ /langversion:8.0 ./File1.cs ./File2.cs",
						Path("C:/root/"),
						std::vector<Path>({
						}),
						std::vector<Path>({
						}),
						std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
						})));

			auto expectedBuildOperations = std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/obj]",
					Path("C:/testlocation/mkdir.exe"),
					"\"C:/root/obj\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({
						Path("C:/root/obj"),
					}),
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
						expectedCompileOperation,
					})),
				new Build::Utilities::BuildOperation(
					"MakeDir [C:/root/bin]",
					Path("C:/testlocation/mkdir.exe"),
					"\"C:/root/bin\"",
					Path("./"),
					std::vector<Path>({}),
					std::vector<Path>({
						Path("C:/root/bin"),
					}),
					std::vector<Memory::Reference<Build::Utilities::BuildOperation>>({
						expectedCompileOperation,
					})),
			});

			AssertExtensions::AreEqual(
				expectedBuildOperations,
				result.BuildOperations);

			Assert::AreEqual(
				std::vector<Path>({
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
