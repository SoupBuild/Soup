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
            auto compiler = std::make_shared<MockCompiler>();
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
            auto test = std::fstream("missingfile.txt");
            
            // Register the test listener
            auto testListener = std::make_shared<TestTraceListener>();
            Log::RegisterListener(testListener);

            // Register the test file system
            auto fileSystem = std::make_shared<MockFileSystem>();
            IFileSystem::Register(fileSystem);

            auto compiler = std::make_shared<MockCompiler>();
            auto uut = BuildEngine(compiler);

            auto arguments = BuildArguments();
            arguments.Target = TargetType::Executable;
            arguments.WorkingDirectory = Path("");
            arguments.ObjectDirectory = Path("obj");
            arguments.BinaryDirectory = Path("bin");
            arguments.SourceFiles = std::vector<Path>({});
            arguments.IncludeDirectories = std::vector<Path>({});
            arguments.ModuleFiles = std::vector<Path>({});
            arguments.IsIncremental = false;

            uut.Execute(arguments);

            // Verify expected compiler calls
            Assert::AreEqual(
                std::vector<std::string>({
                }),
                compiler->GetMessages(),
                "Verify compiler requests match expected.");

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::pair<std::string, FileSystemRequestType>>({
                    std::make_pair(".soup/BuildState.json", FileSystemRequestType::OpenWrite),
                }),
                fileSystem->GetRequests(),
                "Verify file system requests match expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
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

            auto compiler = std::make_shared<MockCompiler>();
            auto uut = BuildEngine(compiler);

            auto arguments = BuildArguments();
            arguments.Target = TargetType::Executable;
            arguments.WorkingDirectory = Path("");
            arguments.ObjectDirectory = Path("obj");
            arguments.BinaryDirectory = Path("bin");
            arguments.SourceFiles = std::vector<Path>({});
            arguments.IncludeDirectories = std::vector<Path>({});
            arguments.ModuleFiles = std::vector<Path>({});
            arguments.IsIncremental = true;

            uut.Execute(arguments);

            // Verify expected compiler calls
            Assert::AreEqual(
                std::vector<std::string>({
                }),
                compiler->GetMessages(),
                "Verify compiler requests match expected.");

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::pair<std::string, FileSystemRequestType>>({
                    std::make_pair(".soup/BuildState.json", FileSystemRequestType::Exists),
                    std::make_pair(".soup/BuildState.json", FileSystemRequestType::OpenWrite),
                }),
                fileSystem->GetRequests(),
                "Verify file system requests match expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "Loading previous build state.\n",
                    "BuildState file does not exist.\n",
                    "No previous state found.\n",
                }),
                testListener->GetMessages(),
                "Verify log messages match expected.");
        }
    };
}
