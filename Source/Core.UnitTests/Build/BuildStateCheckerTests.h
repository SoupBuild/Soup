// <copyright file="BuildStateCheckerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class BuildStateCheckerTests
    {
    public:
        [[Fact]]
        void IsOutdated_SingleInput_MissingTarget()
        {
            // Register the test listener
            auto testListener = std::make_shared<TestTraceListener>();
            Log::RegisterListener(testListener);

            // Register the test file system
            auto fileSystem = std::make_shared<MockFileSystem>();
            IFileSystem::Register(fileSystem);

            // Setup the input parameters
            auto targetFile = Path("Output.bin");
            auto inputFiles = std::vector<Path>({
                Path("Input.cpp")
            });
            auto rootPath = Path("Root");

            // Perform the check
            bool result = BuildStateChecker::IsOutdated(targetFile, inputFiles, rootPath);

            // Verify the results
            Assert::IsTrue(result, "Verify the result is true.");

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::pair<std::string, FileSystemRequestType>>({
                    std::make_pair("Root/Output.bin", FileSystemRequestType::Exists),
                }),
                fileSystem->GetRequests(),
                "Verify file system requests match expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "VERB: Output target does not exist: Output.bin",
                }),
                testListener->GetMessages(),
                "Verify log messages match expected.");
        }

        [[Fact]]
        void IsOutdated_SingleInput_TargetExists_MissingInput()
        {
            // Register the test listener
            auto testListener = std::make_shared<TestTraceListener>();
            Log::RegisterListener(testListener);

            // Register the test file system
            auto fileSystem = std::make_shared<MockFileSystem>();
            IFileSystem::Register(fileSystem);

            // Create the file state
            auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
            fileSystem->CreateFile(Path("Root/Output.bin"), MockFileState(outputTime));

            // Setup the input parameters
            auto targetFile = Path("Output.bin");
            auto inputFiles = std::vector<Path>({
                Path("Input.cpp")
            });
            auto rootPath = Path("Root");

            // Perform the check
            bool result = BuildStateChecker::IsOutdated(targetFile, inputFiles, rootPath);

            // Verify the results
            Assert::IsTrue(result, "Verify the result is true.");

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::pair<std::string, FileSystemRequestType>>({
                    std::make_pair("Root/Output.bin", FileSystemRequestType::Exists),
                    std::make_pair("Root/Output.bin", FileSystemRequestType::GetLastWriteTime),
                    std::make_pair("Root/Input.cpp", FileSystemRequestType::Exists),
                }),
                fileSystem->GetRequests(),
                "Verify file system requests match expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "VERB: Input file missing, rebuild required.",
                    "VERB: [Input.cpp] -> [Output.bin].",
                }),
                testListener->GetMessages(),
                "Verify log messages match expected.");
        }

        [[Fact]]
        void IsOutdated_SingleInput_ExistsOut_Outdated()
        {
            // Register the test listener
            auto testListener = std::make_shared<TestTraceListener>();
            Log::RegisterListener(testListener);

            // Register the test file system
            auto fileSystem = std::make_shared<MockFileSystem>();
            IFileSystem::Register(fileSystem);

            // Create the file state
            auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
            auto inputTime = CreateDateTime(2015, 5, 22, 9, 13);
            fileSystem->CreateFile(Path("Root/Output.bin"), MockFileState(outputTime));
            fileSystem->CreateFile(Path("Root/Input.cpp"), MockFileState(inputTime));

            // Setup the input parameters
            auto targetFile = Path("Output.bin");
            auto inputFiles = std::vector<Path>({
                Path("Input.cpp")
            });
            auto rootPath = Path("Root");

            // Perform the check
            bool result = BuildStateChecker::IsOutdated(targetFile, inputFiles, rootPath);

            // Verify the results
            Assert::IsTrue(result, "Verify the result is true.");

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::pair<std::string, FileSystemRequestType>>({
                    std::make_pair("Root/Output.bin", FileSystemRequestType::Exists),
                    std::make_pair("Root/Output.bin", FileSystemRequestType::GetLastWriteTime),
                    std::make_pair("Root/Input.cpp", FileSystemRequestType::Exists),
                    std::make_pair("Root/Input.cpp", FileSystemRequestType::GetLastWriteTime),
                }),
                fileSystem->GetRequests(),
                "Verify file system requests match expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "VERB: Input file altered after target, rebuild required.",
                    "VERB: [Input.cpp] -> [Output.bin].",
                }),
                testListener->GetMessages(),
                "Verify log messages match expected.");
        }

        [[Fact]]
        void IsOutdated_SingleInput_ExistsOut_UpToDate()
        {
            // Register the test listener
            auto testListener = std::make_shared<TestTraceListener>();
            Log::RegisterListener(testListener);

            // Register the test file system
            auto fileSystem = std::make_shared<MockFileSystem>();
            IFileSystem::Register(fileSystem);

            // Create the file state
            auto outputTime = CreateDateTime(2015, 5, 22, 9, 12);
            auto inputTime = CreateDateTime(2015, 5, 22, 9, 11);
            fileSystem->CreateFile(Path("Root/Output.bin"), MockFileState(outputTime));
            fileSystem->CreateFile(Path("Root/Input.cpp"), MockFileState(inputTime));

            // Setup the input parameters
            auto targetFile = Path("Output.bin");
            auto inputFiles = std::vector<Path>({
                Path("Input.cpp")
            });
            auto rootPath = Path("Root");

            // Perform the check
            bool result = BuildStateChecker::IsOutdated(targetFile, inputFiles, rootPath);

            // Verify the results
            Assert::IsFalse(result, "Verify the result is false.");

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::pair<std::string, FileSystemRequestType>>({
                    std::make_pair("Root/Output.bin", FileSystemRequestType::Exists),
                    std::make_pair("Root/Output.bin", FileSystemRequestType::GetLastWriteTime),
                    std::make_pair("Root/Input.cpp", FileSystemRequestType::Exists),
                    std::make_pair("Root/Input.cpp", FileSystemRequestType::GetLastWriteTime),
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
    };
}