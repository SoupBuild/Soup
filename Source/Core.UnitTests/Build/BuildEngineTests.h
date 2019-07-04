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
        void Build_Executable_SingleFile_NotIncremental()
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
            arguments.Target = BuildTargetType::Executable;
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

            auto expectedCompileArguments = CompileArguments();
            expectedCompileArguments.Standard = LanguageStandard::CPP20;
            expectedCompileArguments.RootDirectory = Path("root");
            expectedCompileArguments.OutputDirectory = Path("obj");
            expectedCompileArguments.SourceFile = Path("TestFile.cpp");
            expectedCompileArguments.GenerateIncludeTree = true;

            // Verify expected compiler calls
            Assert::AreEqual(
                std::vector<CompileArguments>({
                    expectedCompileArguments,
                }),
                compiler->GetCompileRequests(),
                "Verify compiler requests match expected.");
            Assert::AreEqual(
                std::vector<LinkerArguments>({
                }),
                compiler->GetLinkLibraryRequests(),
                "Verify link library requests match expected.");
            Assert::AreEqual(
                std::vector<LinkerArguments>({
                }),
                compiler->GetLinkExecutableRequests(),
                "Verify link executable requests match expected.");

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::pair<std::string, FileSystemRequestType>>({
                    std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenWrite),
                }),
                fileSystem->GetRequests(),
                "Verify file system requests match expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "VERB: Target = Executable",
                    "VERB: WorkingDirectory = root",
                    "VERB: ObjectDirectory = obj",
                    "VERB: BinaryDirectory = bin",
                    "VERB: ModuleSourceFile = ",
                    "VERB: IsIncremental = false",
                    "VERB: Task: CoreCompile",
                    "VERB: Compiling source files.",
                    "VERB: TestFile.cpp",
                    "VERB: Saving updated build state",
                    "VERB: Task: LinkExecutable",
                }),
                testListener->GetMessages(),
                "Verify log messages match expected.");
        }

        [[Fact]]
        void Build_Executable_SingleFile_Incremental_MissingBuildState()
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
            arguments.Target = BuildTargetType::Executable;
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

            auto expectedCompileArguments = CompileArguments();
            expectedCompileArguments.Standard = LanguageStandard::CPP20;
            expectedCompileArguments.RootDirectory = Path("root");
            expectedCompileArguments.OutputDirectory = Path("obj");
            expectedCompileArguments.SourceFile = Path("TestFile.cpp");
            expectedCompileArguments.GenerateIncludeTree = true;

            // Verify expected compiler calls
            Assert::AreEqual(
                std::vector<CompileArguments>({
                    expectedCompileArguments,
                }),
                compiler->GetCompileRequests(),
                "Verify compiler requests match expected.");
            Assert::AreEqual(
                std::vector<LinkerArguments>({
                }),
                compiler->GetLinkLibraryRequests(),
                "Verify link library requests match expected.");
            Assert::AreEqual(
                std::vector<LinkerArguments>({
                }),
                compiler->GetLinkExecutableRequests(),
                "Verify link executable requests match expected.");

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::pair<std::string, FileSystemRequestType>>({
                    std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::Exists),
                    std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenWrite),
                }),
                fileSystem->GetRequests(),
                "Verify file system requests match expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "VERB: Target = Executable",
                    "VERB: WorkingDirectory = root",
                    "VERB: ObjectDirectory = obj",
                    "VERB: BinaryDirectory = bin",
                    "VERB: ModuleSourceFile = ",
                    "VERB: IsIncremental = true",
                    "VERB: Task: CoreCompile",
                    "VERB: Loading previous build state.",
                    "VERB: BuildState file does not exist.",
                    "VERB: No previous state found, full rebuild required.",
                    "VERB: Compiling source files.",
                    "VERB: TestFile.cpp",
                    "VERB: Saving updated build state",
                    "VERB: Task: LinkExecutable",
                }),
                testListener->GetMessages(),
                "Verify log messages match expected.");
        }

        [[Fact]]
        void Build_Executable_SingleFile_Incremental_MissingFileInfo()
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
            arguments.Target = BuildTargetType::Executable;
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

            auto expectedCompileArguments = CompileArguments();
            expectedCompileArguments.Standard = LanguageStandard::CPP20;
            expectedCompileArguments.RootDirectory = Path("root");
            expectedCompileArguments.OutputDirectory = Path("obj");
            expectedCompileArguments.SourceFile = Path("TestFile.cpp");
            expectedCompileArguments.GenerateIncludeTree = true;

            // Verify expected compiler calls
            Assert::AreEqual(
                std::vector<CompileArguments>({
                    expectedCompileArguments,
                }),
                compiler->GetCompileRequests(),
                "Verify compiler requests match expected.");
            Assert::AreEqual(
                std::vector<LinkerArguments>({
                }),
                compiler->GetLinkLibraryRequests(),
                "Verify link library requests match expected.");
            Assert::AreEqual(
                std::vector<LinkerArguments>({
                }),
                compiler->GetLinkExecutableRequests(),
                "Verify link executable requests match expected.");

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::pair<std::string, FileSystemRequestType>>({
                    std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::Exists),
                    std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenRead),
                    std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenWrite),
                }),
                fileSystem->GetRequests(),
                "Verify file system requests match expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "VERB: Target = Executable",
                    "VERB: WorkingDirectory = root",
                    "VERB: ObjectDirectory = obj",
                    "VERB: BinaryDirectory = bin",
                    "VERB: ModuleSourceFile = ",
                    "VERB: IsIncremental = true",
                    "VERB: Task: CoreCompile",
                    "VERB: Loading previous build state.",
                    "VERB: Check for updated source.",
                    "VERB: Missing file info: TestFile.cpp",
                    "VERB: Compiling source files.",
                    "VERB: TestFile.cpp",
                    "VERB: Saving updated build state",
                    "VERB: Task: LinkExecutable",
                }),
                testListener->GetMessages(),
                "Verify log messages match expected.");
        }

        [[Fact]]
        void Build_Executable_SingleFile_Incremental_UpToDate()
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
            arguments.Target = BuildTargetType::Executable;
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
                std::vector<LinkerArguments>({
                }),
                compiler->GetLinkLibraryRequests(),
                "Verify link library requests match expected.");
            Assert::AreEqual(
                std::vector<LinkerArguments>({
                }),
                compiler->GetLinkExecutableRequests(),
                "Verify link executable requests match expected.");

            // Verify expected file system requests
            // Assert::AreEqual(
            //     std::vector<std::pair<std::string, FileSystemRequestType>>({
            //         std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::Exists),
            //         std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenRead),
            //         std::make_pair("root/TestFile.cpp", FileSystemRequestType::Exists),
            //         std::make_pair("root/.soup/BuildState.json", FileSystemRequestType::OpenWrite),
            //     }),
            //     fileSystem->GetRequests(),
            //     "Verify file system requests match expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "VERB: Target = Executable",
                    "VERB: WorkingDirectory = root",
                    "VERB: ObjectDirectory = obj",
                    "VERB: BinaryDirectory = bin",
                    "VERB: ModuleSourceFile = ",
                    "VERB: IsIncremental = true",
                    "VERB: Task: CoreCompile",
                    "VERB: Loading previous build state.",
                    "VERB: Check for updated source.",
                    "INFO: Up to date",
                }),
                testListener->GetMessages(),
                "Verify log messages match expected.");
        }
    };
}
