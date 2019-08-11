// <copyright file="RecipeBuildGeneratorTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class RecipeBuildGeneratorTests
    {
    public:
        [[Fact]]
        void Initialize_Success()
        {
            auto uut = RecipeBuildGenerator();
        }

        [[Fact]]
        void Execute()
        {
            // Register the test listener
            auto testListener = std::make_shared<TestTraceListener>();
            Log::RegisterListener(testListener);

            // Register the test file system
            auto fileSystem = std::make_shared<MockFileSystem>();
            IFileSystem::Register(fileSystem);

            auto uut = RecipeBuildGenerator();

            auto workingDirectory = Path("Root");
            auto recipe = Recipe();
            uut.Execute(workingDirectory, recipe);

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "VERB: Create Directory: Root/.soup/Build"
                }),
                testListener->GetMessages(),
                "Verify log messages match expected.");

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::pair<std::string, FileSystemRequestType>>({
                    std::make_pair("Root/.soup/Build", FileSystemRequestType::Exists),
                    std::make_pair("Root/.soup/Build", FileSystemRequestType::CreateDirectory),
                }),
                fileSystem->GetRequests(),
                "Verify file system requests match expected.");
        }

        // TODO: Way more of this
    };
}
