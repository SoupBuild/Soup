// <copyright file="BuildEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class BuildEngineTests
	{
	public:
		[[Fact]]
		void InitialzeEngine_Success()
		{
			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildEngine(compiler);
		}

		[[Fact]]
		void InitialzeEngine_NullCompilerThrows()
		{
			auto compiler = nullptr;
			Assert::ThrowsRuntimeError([&compiler]() {
				auto uut = BuildEngine(compiler);
			});
		}

		[[Fact]]
		void Build_Executable_Simple_NotIncremental()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildEngine(compiler);

			auto arguments = BuildArguments();
			arguments.TargetName = "Program";
			arguments.TargetType = BuildTargetType::Executable;
			arguments.WorkingDirectory = Path("root");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.SourceFiles = std::vector<Path>({ 
				Path("TestFile.cpp"),
			});
			arguments.IncludeDirectories = std::vector<Path>({});
			arguments.IncludeModules = std::vector<Path>({});
			arguments.IsIncremental = false;

			uut.Execute(arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"VERB: TargetName = Program",
					"VERB: TargetType = Executable",
					"VERB: WorkingDirectory = root",
					"VERB: ObjectDirectory = obj",
					"VERB: BinaryDirectory = bin",
					"VERB: ModuleInterfaceSourceFile = ./",
					"VERB: IsIncremental = false",
					"VERB: IncludeDirectories = ",
					"VERB: IncludeModules = ",
					"VERB: PreprocessorDefinitions = ",
					"VERB: Task: CoreCompile",
					"VERB: Create Directory: obj",
					"VERB: Create Directory: bin",
					"VERB: Task: CompileSourceFiles",
					"VERB: Compiling source files",
					"VERB: TestFile.cpp",
					"VERB: Saving updated build state",
					"VERB: Create Directory: .soup",
					"VERB: Task: CoreLink",
					"VERB: Linking target",
					"VERB: bin/Program.exe",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::Speed;
			expectedCompileArguments.RootDirectory = Path("root");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");
			expectedCompileArguments.GenerateIncludeTree = true;

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetType = LinkTarget::Executable;
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.RootDirectory = Path("root");
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

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::pair<std::string, FileSystemRequestType>>({
					std::make_pair("root/obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/bin", FileSystemRequestType::Exists),
					std::make_pair("root/bin", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/.soup", FileSystemRequestType::Exists),
					std::make_pair("root/.soup", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenWrite),
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void Build_Executable_Simple_Incremental_MissingBuildState()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildEngine(compiler);

			auto arguments = BuildArguments();
			arguments.TargetName = "Program";
			arguments.TargetType = BuildTargetType::Executable;
			arguments.WorkingDirectory = Path("root");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.SourceFiles = std::vector<Path>({ 
				Path("TestFile.cpp"),
			});
			arguments.IncludeDirectories = std::vector<Path>({});
			arguments.IncludeModules = std::vector<Path>({});
			arguments.IsIncremental = true;

			uut.Execute(arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"VERB: TargetName = Program",
					"VERB: TargetType = Executable",
					"VERB: WorkingDirectory = root",
					"VERB: ObjectDirectory = obj",
					"VERB: BinaryDirectory = bin",
					"VERB: ModuleInterfaceSourceFile = ./",
					"VERB: IsIncremental = true",
					"VERB: IncludeDirectories = ",
					"VERB: IncludeModules = ",
					"VERB: PreprocessorDefinitions = ",
					"VERB: Task: CoreCompile",
					"VERB: Loading previous build state",
					"VERB: BuildState file does not exist",
					"VERB: No previous state found, full rebuild required",
					"VERB: Create Directory: obj",
					"VERB: Create Directory: bin",
					"VERB: Task: CompileSourceFiles",
					"VERB: Compiling source files",
					"VERB: TestFile.cpp",
					"VERB: Saving updated build state",
					"VERB: Create Directory: .soup",
					"VERB: Task: CoreLink",
					"VERB: Linking target",
					"VERB: bin/Program.exe",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::Speed;
			expectedCompileArguments.RootDirectory = Path("root");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");
			expectedCompileArguments.GenerateIncludeTree = true;

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.TargetType = LinkTarget::Executable;
			expectedLinkArguments.RootDirectory = Path("root");
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

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::pair<std::string, FileSystemRequestType>>({
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::Exists),
					std::make_pair("root/obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/bin", FileSystemRequestType::Exists),
					std::make_pair("root/bin", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/.soup", FileSystemRequestType::Exists),
					std::make_pair("root/.soup", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenWrite),
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void Build_Executable_Simple_Incremental_MissingFileInfo()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			// Create the initial build state
			auto initialBuildState = BuildState();
			std::stringstream initialBuildStateJson;
			BuildStateJson::Serialize(initialBuildState, initialBuildStateJson);
			fileSystem->CreateMockFile(
				Path("root/.soup/BuildState.json"),
				MockFileState(std::move(initialBuildStateJson)));

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildEngine(compiler);

			auto arguments = BuildArguments();
			arguments.TargetName = "Program";
			arguments.TargetType = BuildTargetType::Executable;
			arguments.WorkingDirectory = Path("root");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.SourceFiles = std::vector<Path>({ 
				Path("TestFile.cpp"),
			});
			arguments.IncludeDirectories = std::vector<Path>({});
			arguments.IncludeModules = std::vector<Path>({});
			arguments.IsIncremental = true;

			uut.Execute(arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"VERB: TargetName = Program",
					"VERB: TargetType = Executable",
					"VERB: WorkingDirectory = root",
					"VERB: ObjectDirectory = obj",
					"VERB: BinaryDirectory = bin",
					"VERB: ModuleInterfaceSourceFile = ./",
					"VERB: IsIncremental = true",
					"VERB: IncludeDirectories = ",
					"VERB: IncludeModules = ",
					"VERB: PreprocessorDefinitions = ",
					"VERB: Task: CoreCompile",
					"VERB: Loading previous build state",
					"VERB: Create Directory: obj",
					"VERB: Create Directory: bin",
					"VERB: Task: CompileSourceFiles",
					"VERB: Check for updated source",
					"VERB: Missing file info: TestFile.cpp",
					"VERB: Compiling source files",
					"VERB: TestFile.cpp",
					"VERB: Saving updated build state",
					"VERB: Create Directory: .soup",
					"VERB: Task: CoreLink",
					"VERB: Linking target",
					"VERB: bin/Program.exe",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::Speed;
			expectedCompileArguments.RootDirectory = Path("root");
			expectedCompileArguments.SourceFile = Path("TestFile.cpp");
			expectedCompileArguments.TargetFile = Path("obj/TestFile.mock.obj");
			expectedCompileArguments.GenerateIncludeTree = true;

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.TargetType = LinkTarget::Executable;
			expectedLinkArguments.RootDirectory = Path("root");
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

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::pair<std::string, FileSystemRequestType>>({
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::Exists),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenRead),
					std::make_pair("root/obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/bin", FileSystemRequestType::Exists),
					std::make_pair("root/bin", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/.soup", FileSystemRequestType::Exists),
					std::make_pair("root/.soup", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenWrite),
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void Build_Executable_Simple_Incremental_MissingTargetFile()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			// Create the initial build state
			auto initialBuildState = BuildState({
				FileInfo(Path("TestFile.cpp"), { }),
			});
			std::stringstream initialBuildStateJson;
			BuildStateJson::Serialize(initialBuildState, initialBuildStateJson);
			fileSystem->CreateMockFile(
				Path("root/.soup/BuildState.json"),
				MockFileState(std::move(initialBuildStateJson)));

			// Setup the input/output files to be up to date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystem->CreateMockFile(Path("root/obj/TestFile.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/TestFile.cpp"), MockFileState(inputTime));

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildEngine(compiler);

			auto arguments = BuildArguments();
			arguments.TargetName = "Program";
			arguments.TargetType = BuildTargetType::Executable;
			arguments.WorkingDirectory = Path("root");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.SourceFiles = std::vector<Path>({ 
				Path("TestFile.cpp"),
			});
			arguments.IncludeDirectories = std::vector<Path>({});
			arguments.IncludeModules = std::vector<Path>({});
			arguments.IsIncremental = true;

			uut.Execute(arguments);

			auto expectedLinkArguments = LinkArguments();
			expectedLinkArguments.TargetFile = Path("bin/Program.exe");
			expectedLinkArguments.TargetType = LinkTarget::Executable;
			expectedLinkArguments.RootDirectory = Path("root");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile.mock.obj"),
			});

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<CompileArguments>({}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<LinkArguments>({
					expectedLinkArguments,
				}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::pair<std::string, FileSystemRequestType>>({
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::Exists),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenRead),
					std::make_pair("root/obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/bin", FileSystemRequestType::Exists),
					std::make_pair("root/bin", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/obj/TestFile.mock.obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj/TestFile.mock.obj", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/TestFile.cpp", FileSystemRequestType::Exists),
					std::make_pair("root/TestFile.cpp", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/bin/Program.exe", FileSystemRequestType::Exists),
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"VERB: TargetName = Program",
					"VERB: TargetType = Executable",
					"VERB: WorkingDirectory = root",
					"VERB: ObjectDirectory = obj",
					"VERB: BinaryDirectory = bin",
					"VERB: ModuleInterfaceSourceFile = ./",
					"VERB: IsIncremental = true",
					"VERB: IncludeDirectories = ",
					"VERB: IncludeModules = ",
					"VERB: PreprocessorDefinitions = ",
					"VERB: Task: CoreCompile",
					"VERB: Loading previous build state",
					"VERB: Create Directory: obj",
					"VERB: Create Directory: bin",
					"VERB: Task: CompileSourceFiles",
					"VERB: Check for updated source",
					"VERB: File up to date: TestFile.cpp",
					"VERB: Objects up to date",
					"VERB: Task: CoreLink",
					"VERB: Link target does not exist: bin/Program.exe",
					"VERB: Linking target",
					"VERB: bin/Program.exe",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void Build_Executable_Simple_Incremental_UpToDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			// Create the initial build state
			auto initialBuildState = BuildState({
				FileInfo(Path("TestFile.cpp"), { }),
			});
			std::stringstream initialBuildStateJson;
			BuildStateJson::Serialize(initialBuildState, initialBuildStateJson);
			fileSystem->CreateMockFile(
				Path("root/.soup/BuildState.json"),
				MockFileState(std::move(initialBuildStateJson)));

			// Setup the input/output files to be up to date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			fileSystem->CreateMockFile(Path("root/bin/Program.exe"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/obj/TestFile.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/TestFile.cpp"), MockFileState(inputTime));

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildEngine(compiler);

			auto arguments = BuildArguments();
			arguments.TargetName = "Program";
			arguments.TargetType = BuildTargetType::Executable;
			arguments.WorkingDirectory = Path("root");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.SourceFiles = std::vector<Path>({ 
				Path("TestFile.cpp"),
			});
			arguments.IncludeDirectories = std::vector<Path>({});
			arguments.IncludeModules = std::vector<Path>({});
			arguments.IsIncremental = true;

			uut.Execute(arguments);

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<CompileArguments>({}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<LinkArguments>({}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::pair<std::string, FileSystemRequestType>>({
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::Exists),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenRead),
					std::make_pair("root/obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/bin", FileSystemRequestType::Exists),
					std::make_pair("root/bin", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/obj/TestFile.mock.obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj/TestFile.mock.obj", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/TestFile.cpp", FileSystemRequestType::Exists),
					std::make_pair("root/TestFile.cpp", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/bin/Program.exe", FileSystemRequestType::Exists),
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"VERB: TargetName = Program",
					"VERB: TargetType = Executable",
					"VERB: WorkingDirectory = root",
					"VERB: ObjectDirectory = obj",
					"VERB: BinaryDirectory = bin",
					"VERB: ModuleInterfaceSourceFile = ./",
					"VERB: IsIncremental = true",
					"VERB: IncludeDirectories = ",
					"VERB: IncludeModules = ",
					"VERB: PreprocessorDefinitions = ",
					"VERB: Task: CoreCompile",
					"VERB: Loading previous build state",
					"VERB: Create Directory: obj",
					"VERB: Create Directory: bin",
					"VERB: Task: CompileSourceFiles",
					"VERB: Check for updated source",
					"VERB: File up to date: TestFile.cpp",
					"VERB: Objects up to date",
					"VERB: Task: CoreLink",
					"VERB: Final target up to date",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");
		}

		[[Fact]]
		void Build_Library_MultipleFiles_IncludeModuleUpdated()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			// Create the initial build state
			auto initialBuildState = BuildState({
				FileInfo(Path("TestFile1.cpp"), { }),
				FileInfo(Path("TestFile2.cpp"), { }),
				FileInfo(Path("TestFile3.cpp"), { }),
			});
			std::stringstream initialBuildStateJson;
			BuildStateJson::Serialize(initialBuildState, initialBuildStateJson);
			fileSystem->CreateMockFile(
				Path("root/.soup/BuildState.json"),
				MockFileState(std::move(initialBuildStateJson)));

			// Setup the input/output files to be up to date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			auto outdatedInputTime = CreateDateTime(2015, 5, 22, 9, 13);
			fileSystem->CreateMockFile(Path("root/obj/TestFile1.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/obj/TestFile2.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/obj/TestFile3.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/TestFile1.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("root/TestFile2.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("root/TestFile3.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("../Other/bin/OtherModule1.mock.bmi"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("OtherModule2.mock.bmi"), MockFileState(outdatedInputTime));

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildEngine(compiler);

			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::Library;
			arguments.WorkingDirectory = Path("root");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.SourceFiles = std::vector<Path>({ 
				Path("TestFile1.cpp"),
				Path("TestFile2.cpp"),
				Path("TestFile3.cpp"),
			});
			arguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			arguments.IncludeModules = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.bmi"),
				Path("../OtherModule2.mock.bmi"),
			});
			arguments.LinkLibraries = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});
			arguments.IsIncremental = true;

			uut.Execute(arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"VERB: TargetName = Library",
					"VERB: TargetType = Library",
					"VERB: WorkingDirectory = root",
					"VERB: ObjectDirectory = obj",
					"VERB: BinaryDirectory = bin",
					"VERB: ModuleInterfaceSourceFile = ./",
					"VERB: IsIncremental = true",
					"VERB: IncludeDirectories = Folder AnotherFolder/Sub",
					"VERB: IncludeModules = ../../Other/bin/OtherModule1.mock.bmi ../OtherModule2.mock.bmi",
					"VERB: PreprocessorDefinitions = ",
					"VERB: Task: CoreCompile",
					"VERB: Loading previous build state",
					"VERB: Create Directory: obj",
					"VERB: Create Directory: bin",
					"VERB: Task: CompileSourceFiles",
					"VERB: Check for updated source",
					"VERB: Input altered after target [../OtherModule2.mock.bmi] -> [obj/TestFile1.mock.obj]",
					"VERB: Input altered after target [../OtherModule2.mock.bmi] -> [obj/TestFile2.mock.obj]",
					"VERB: Input altered after target [../OtherModule2.mock.bmi] -> [obj/TestFile3.mock.obj]",
					"VERB: Compiling source files",
					"VERB: TestFile1.cpp",
					"VERB: TestFile2.cpp",
					"VERB: TestFile3.cpp",
					"VERB: Saving updated build state",
					"VERB: Create Directory: .soup",
					"VERB: Task: CoreLink",
					"VERB: Linking target",
					"VERB: bin/Library.mock.lib",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Setup the shared arguments
			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::Speed;
			expectedCompileArguments.RootDirectory = Path("root");
			expectedCompileArguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			expectedCompileArguments.IncludeModules = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.bmi"),
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
			expectedLinkArguments.RootDirectory = Path("root");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile1.mock.obj"),
				Path("obj/TestFile2.mock.obj"),
				Path("obj/TestFile3.mock.obj"),
			});
			expectedLinkArguments.LibraryFiles = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.a"),
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

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::pair<std::string, FileSystemRequestType>>({
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::Exists),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenRead),
					std::make_pair("root/obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/bin", FileSystemRequestType::Exists),
					std::make_pair("root/bin", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/obj/TestFile1.mock.obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj/TestFile1.mock.obj", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/TestFile1.cpp", FileSystemRequestType::Exists),
					std::make_pair("root/TestFile1.cpp", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/obj/TestFile2.mock.obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj/TestFile2.mock.obj", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/TestFile2.cpp", FileSystemRequestType::Exists),
					std::make_pair("root/TestFile2.cpp", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/obj/TestFile3.mock.obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj/TestFile3.mock.obj", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/TestFile3.cpp", FileSystemRequestType::Exists),
					std::make_pair("root/TestFile3.cpp", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/.soup", FileSystemRequestType::Exists),
					std::make_pair("root/.soup", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenWrite),
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void Build_Library_Complex_OutOfDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			// Create the initial build state
			auto initialBuildState = BuildState({
				FileInfo(Path("Public.cpp"), { }),
				FileInfo(Path("TestFile1.cpp"), { }),
				FileInfo(Path("TestFile2.cpp"), { }),
				FileInfo(Path("TestFile3.cpp"), { }),
			});
			std::stringstream initialBuildStateJson;
			BuildStateJson::Serialize(initialBuildState, initialBuildStateJson);
			fileSystem->CreateMockFile(
				Path("root/.soup/BuildState.json"),
				MockFileState(std::move(initialBuildStateJson)));

			// Setup the input/output files to be up to date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			auto outdatedInputTime = CreateDateTime(2015, 5, 22, 9, 13);
			fileSystem->CreateMockFile(Path("root/obj/Public.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/obj/TestFile1.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/obj/TestFile2.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/obj/TestFile3.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/Public.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("root/TestFile1.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("root/TestFile2.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("root/TestFile3.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("../Other/bin/OtherModule1.mock.bmi"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("OtherModule2.mock.bmi"), MockFileState(outdatedInputTime));

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildEngine(compiler);

			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::Library;
			arguments.WorkingDirectory = Path("root");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.ModuleInterfaceSourceFile = Path("Public.cpp");
			arguments.SourceFiles = std::vector<Path>({ 
				Path("TestFile1.cpp"),
				Path("TestFile2.cpp"),
				Path("TestFile3.cpp"),
			});
			arguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			arguments.IncludeModules = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.bmi"),
				Path("../OtherModule2.mock.bmi"),
			});
			arguments.LinkLibraries = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});
			arguments.IsIncremental = true;
			arguments.PreprocessorDefinitions = std::vector<std::string>({ 
				"DEBUG",
				"AWESOME",
			});

			uut.Execute(arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"VERB: TargetName = Library",
					"VERB: TargetType = Library",
					"VERB: WorkingDirectory = root",
					"VERB: ObjectDirectory = obj",
					"VERB: BinaryDirectory = bin",
					"VERB: ModuleInterfaceSourceFile = Public.cpp",
					"VERB: IsIncremental = true",
					"VERB: IncludeDirectories = Folder AnotherFolder/Sub",
					"VERB: IncludeModules = ../../Other/bin/OtherModule1.mock.bmi ../OtherModule2.mock.bmi",
					"VERB: PreprocessorDefinitions = DEBUG AWESOME",
					"VERB: Task: CoreCompile",
					"VERB: Loading previous build state",
					"VERB: Create Directory: obj",
					"VERB: Create Directory: bin",
					"VERB: Task: CompileModuleInterfaceUnit",
					"VERB: Check for updated source",
					"VERB: Input altered after target [../OtherModule2.mock.bmi] -> [obj/Public.mock.obj]",
					"VERB: Public.cpp",
					"VERB: Copy: [obj/Public.mock.bmi] -> [bin/Library.mock.bmi]",
					"VERB: Task: CompileSourceFiles",
					"VERB: Compiling source files",
					"VERB: TestFile1.cpp",
					"VERB: TestFile2.cpp",
					"VERB: TestFile3.cpp",
					"VERB: Saving updated build state",
					"VERB: Create Directory: .soup",
					"VERB: Task: CoreLink",
					"VERB: Linking target",
					"VERB: bin/Library.mock.lib",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Setup the shared arguments
			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::Speed;
			expectedCompileArguments.RootDirectory = Path("root");
			expectedCompileArguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			expectedCompileArguments.IncludeModules = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.bmi"),
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
			expectedLinkArguments.RootDirectory = Path("root");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/TestFile1.mock.obj"),
				Path("obj/TestFile2.mock.obj"),
				Path("obj/TestFile3.mock.obj"),
				Path("obj/Public.mock.obj"),
			});
			expectedLinkArguments.LibraryFiles = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.a"),
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

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::pair<std::string, FileSystemRequestType>>({
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::Exists),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenRead),
					std::make_pair("root/obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/bin", FileSystemRequestType::Exists),
					std::make_pair("root/bin", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/obj/Public.mock.obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj/Public.mock.obj", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/Public.cpp", FileSystemRequestType::Exists),
					std::make_pair("root/Public.cpp", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("[root/obj/Public.mock.bmi] -> [root/bin/Library.mock.bmi]", FileSystemRequestType::CopyFile),
					std::make_pair("root/.soup", FileSystemRequestType::Exists),
					std::make_pair("root/.soup", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenWrite),
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void Build_Library_ModuleInterface_UpToDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			// Create the initial build state
			auto initialBuildState = BuildState({
				FileInfo(Path("Public.cpp"), { }),
				FileInfo(Path("TestFile1.cpp"), { }),
				FileInfo(Path("TestFile2.cpp"), { }),
				FileInfo(Path("TestFile3.cpp"), { }),
			});
			std::stringstream initialBuildStateJson;
			BuildStateJson::Serialize(initialBuildState, initialBuildStateJson);
			fileSystem->CreateMockFile(
				Path("root/.soup/BuildState.json"),
				MockFileState(std::move(initialBuildStateJson)));

			// Setup the input/output files to be up to date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			auto outdatedInputTime = CreateDateTime(2015, 5, 22, 9, 13);
			fileSystem->CreateMockFile(Path("root/obj/Public.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/obj/TestFile1.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/obj/TestFile2.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/obj/TestFile3.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/bin/Library.mock.lib"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/Public.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("root/TestFile1.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("root/TestFile2.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("root/TestFile3.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("../Other/bin/OtherModule1.mock.bmi"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("OtherModule2.mock.bmi"), MockFileState(inputTime));

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildEngine(compiler);

			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::Library;
			arguments.WorkingDirectory = Path("root");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.ModuleInterfaceSourceFile = Path("Public.cpp");
			arguments.SourceFiles = std::vector<Path>({ 
				Path("TestFile1.cpp"),
				Path("TestFile2.cpp"),
				Path("TestFile3.cpp"),
			});
			arguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			arguments.IncludeModules = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.bmi"),
				Path("../OtherModule2.mock.bmi"),
			});
			arguments.LinkLibraries = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});
			arguments.IsIncremental = true;

			uut.Execute(arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"VERB: TargetName = Library",
					"VERB: TargetType = Library",
					"VERB: WorkingDirectory = root",
					"VERB: ObjectDirectory = obj",
					"VERB: BinaryDirectory = bin",
					"VERB: ModuleInterfaceSourceFile = Public.cpp",
					"VERB: IsIncremental = true",
					"VERB: IncludeDirectories = Folder AnotherFolder/Sub",
					"VERB: IncludeModules = ../../Other/bin/OtherModule1.mock.bmi ../OtherModule2.mock.bmi",
					"VERB: PreprocessorDefinitions = ",
					"VERB: Task: CoreCompile",
					"VERB: Loading previous build state",
					"VERB: Create Directory: obj",
					"VERB: Create Directory: bin",
					"VERB: Task: CompileModuleInterfaceUnit",
					"VERB: Check for updated source",
					"VERB: File up to date: Public.cpp",
					"VERB: Module up to date",
					"VERB: Task: CompileSourceFiles",
					"VERB: Check for updated source",
					"VERB: File up to date: TestFile1.cpp",
					"VERB: File up to date: TestFile2.cpp",
					"VERB: File up to date: TestFile3.cpp",
					"VERB: Objects up to date",
					"VERB: Task: CoreLink",
					"VERB: Final target up to date",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Verify expected compiler calls
			Assert::AreEqual(
				std::vector<CompileArguments>({}),
				compiler->GetCompileRequests(),
				"Verify compiler requests match expected.");
			Assert::AreEqual(
				std::vector<LinkArguments>({}),
				compiler->GetLinkRequests(),
				"Verify link requests match expected.");

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::pair<std::string, FileSystemRequestType>>({
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::Exists),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenRead),
					std::make_pair("root/obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/bin", FileSystemRequestType::Exists),
					std::make_pair("root/bin", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/obj/Public.mock.obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj/Public.mock.obj", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/Public.cpp", FileSystemRequestType::Exists),
					std::make_pair("root/Public.cpp", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/obj/TestFile1.mock.obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj/TestFile1.mock.obj", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/TestFile1.cpp", FileSystemRequestType::Exists),
					std::make_pair("root/TestFile1.cpp", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/obj/TestFile2.mock.obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj/TestFile2.mock.obj", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/TestFile2.cpp", FileSystemRequestType::Exists),
					std::make_pair("root/TestFile2.cpp", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/obj/TestFile3.mock.obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj/TestFile3.mock.obj", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/TestFile3.cpp", FileSystemRequestType::Exists),
					std::make_pair("root/TestFile3.cpp", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/bin/Library.mock.lib", FileSystemRequestType::Exists),
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}

		[[Fact]]
		void Build_Library_ModuleInterfaceNoSource_OutOfDate()
		{
			// Register the test listener
			auto testListener = std::make_shared<TestTraceListener>();
			Log::RegisterListener(testListener);

			// Register the test file system
			auto fileSystem = std::make_shared<MockFileSystem>();
			IFileSystem::Register(fileSystem);

			// Create the initial build state
			auto initialBuildState = BuildState({
				FileInfo(Path("Public.cpp"), { }),
			});
			std::stringstream initialBuildStateJson;
			BuildStateJson::Serialize(initialBuildState, initialBuildStateJson);
			fileSystem->CreateMockFile(
				Path("root/.soup/BuildState.json"),
				MockFileState(std::move(initialBuildStateJson)));

			// Setup the input/output files to be up to date
			auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
			auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
			auto outdatedInputTime = CreateDateTime(2015, 5, 22, 9, 13);
			fileSystem->CreateMockFile(Path("root/obj/Public.mock.obj"), MockFileState(outputTime));
			fileSystem->CreateMockFile(Path("root/Public.cpp"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("../Other/bin/OtherModule1.mock.bmi"), MockFileState(inputTime));
			fileSystem->CreateMockFile(Path("OtherModule2.mock.bmi"), MockFileState(outdatedInputTime));

			auto compiler = std::make_shared<Compiler::Mock::Compiler>();
			auto uut = BuildEngine(compiler);

			auto arguments = BuildArguments();
			arguments.TargetName = "Library";
			arguments.TargetType = BuildTargetType::Library;
			arguments.WorkingDirectory = Path("root");
			arguments.ObjectDirectory = Path("obj");
			arguments.BinaryDirectory = Path("bin");
			arguments.ModuleInterfaceSourceFile = Path("Public.cpp");
			arguments.SourceFiles = std::vector<Path>({});
			arguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			arguments.IncludeModules = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.bmi"),
				Path("../OtherModule2.mock.bmi"),
			});
			arguments.LinkLibraries = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.a"),
				Path("../OtherModule2.mock.a"),
			});
			arguments.IsIncremental = true;

			uut.Execute(arguments);

			// Verify expected logs
			Assert::AreEqual(
				std::vector<std::string>({
					"VERB: TargetName = Library",
					"VERB: TargetType = Library",
					"VERB: WorkingDirectory = root",
					"VERB: ObjectDirectory = obj",
					"VERB: BinaryDirectory = bin",
					"VERB: ModuleInterfaceSourceFile = Public.cpp",
					"VERB: IsIncremental = true",
					"VERB: IncludeDirectories = Folder AnotherFolder/Sub",
					"VERB: IncludeModules = ../../Other/bin/OtherModule1.mock.bmi ../OtherModule2.mock.bmi",
					"VERB: PreprocessorDefinitions = ",
					"VERB: Task: CoreCompile",
					"VERB: Loading previous build state",
					"VERB: Create Directory: obj",
					"VERB: Create Directory: bin",
					"VERB: Task: CompileModuleInterfaceUnit",
					"VERB: Check for updated source",
					"VERB: Input altered after target [../OtherModule2.mock.bmi] -> [obj/Public.mock.obj]",
					"VERB: Public.cpp",
					"VERB: Copy: [obj/Public.mock.bmi] -> [bin/Library.mock.bmi]",
					"VERB: Saving updated build state",
					"VERB: Create Directory: .soup",
					"VERB: Task: CoreLink",
					"VERB: Linking target",
					"VERB: bin/Library.mock.lib",
				}),
				testListener->GetMessages(),
				"Verify log messages match expected.");

			// Setup the shared arguments
			auto expectedCompileArguments = CompileArguments();
			expectedCompileArguments.Standard = LanguageStandard::CPP20;
			expectedCompileArguments.Optimize = OptimizationLevel::Speed;
			expectedCompileArguments.RootDirectory = Path("root");
			expectedCompileArguments.IncludeDirectories = std::vector<Path>({
				Path("Folder"),
				Path("AnotherFolder/Sub"),
			});
			expectedCompileArguments.IncludeModules = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.bmi"),
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
			expectedLinkArguments.RootDirectory = Path("root");
			expectedLinkArguments.ObjectFiles = std::vector<Path>({
				Path("obj/Public.mock.obj"),
			});
			expectedLinkArguments.LibraryFiles = std::vector<Path>({
				Path("../../Other/bin/OtherModule1.mock.a"),
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

			// Verify expected file system requests
			Assert::AreEqual(
				std::vector<std::pair<std::string, FileSystemRequestType>>({
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::Exists),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenRead),
					std::make_pair("root/obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/bin", FileSystemRequestType::Exists),
					std::make_pair("root/bin", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/obj/Public.mock.obj", FileSystemRequestType::Exists),
					std::make_pair("root/obj/Public.mock.obj", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("root/Public.cpp", FileSystemRequestType::Exists),
					std::make_pair("root/Public.cpp", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("../Other/bin/OtherModule1.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::Exists),
					std::make_pair("OtherModule2.mock.bmi", FileSystemRequestType::GetLastWriteTime),
					std::make_pair("[root/obj/Public.mock.bmi] -> [root/bin/Library.mock.bmi]", FileSystemRequestType::CopyFile),
					std::make_pair("root/.soup", FileSystemRequestType::Exists),
					std::make_pair("root/.soup", FileSystemRequestType::CreateDirectory),
					std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenWrite),
				}),
				fileSystem->GetRequests(),
				"Verify file system requests match expected.");
		}
	};
}
