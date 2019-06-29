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
            arguments.WorkingDirectory = Path("");
            arguments.ObjectDirectory = Path("obj");
            arguments.BinaryDirectory = Path("bin");
            arguments.SourceFiles = std::vector<Path>({ 
                Path("TestFile.cpp")
            });
            arguments.IncludeDirectories = std::vector<Path>({});
            arguments.IncludeModules = std::vector<Path>({});
            arguments.IsIncremental = false;

            uut.Execute(arguments);

            auto expectedCompileArguments = CompileArguments();
            expectedCompileArguments.Standard = LanguageStandard::CPP20;
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
                    // TODO: Save std::make_pair(".soup/BuildState.json", FileSystemRequestType::OpenWrite),
                }),
                fileSystem->GetRequests(),
                "Verify file system requests match expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "VERB: Target = Executable",
                    "VERB: WorkingDirectory = ",
                    "VERB: ObjectDirectory = obj",
                    "VERB: BinaryDirectory = bin",
                    "VERB: ModuleSourceFile = ",
                    "VERB: IsIncremental = false",
                    "INFO: Task: CoreCompile",
                    "INFO: TestFile.cpp",
                    "INFO: Link Executable",
                }),
                testListener->GetMessages(),
                "Verify log messages match expected.");
        }

        [[Fact]]
        void Build_Executable_SingleFile_Incremental()
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
            arguments.WorkingDirectory = Path("");
            arguments.ObjectDirectory = Path("obj");
            arguments.BinaryDirectory = Path("bin");
            arguments.SourceFiles = std::vector<Path>({ 
                Path("TestFile.cpp")
            });
            arguments.IncludeDirectories = std::vector<Path>({});
            arguments.IncludeModules = std::vector<Path>({});
            arguments.IsIncremental = true;

            uut.Execute(arguments);

            auto expectedCompileArguments = CompileArguments();
            expectedCompileArguments.Standard = LanguageStandard::CPP20;
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
                    std::make_pair(".soup/BuildState.json", FileSystemRequestType::Exists),
                    // TODO: Save std::make_pair(".soup/BuildState.json", FileSystemRequestType::OpenWrite),
                }),
                fileSystem->GetRequests(),
                "Verify file system requests match expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "VERB: Target = Executable",
                    "VERB: WorkingDirectory = ",
                    "VERB: ObjectDirectory = obj",
                    "VERB: BinaryDirectory = bin",
                    "VERB: ModuleSourceFile = ",
                    "VERB: IsIncremental = true",
                    "INFO: Task: CoreCompile",
                    "VERB: Loading previous build state.",
                    "VERB: BuildState file does not exist.",
                    "VERB: No previous state found.",
                    "INFO: TestFile.cpp",
                    "INFO: Link Executable",
                }),
                testListener->GetMessages(),
                "Verify log messages match expected.");
        }
    };
}
