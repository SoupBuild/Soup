// <copyright file="FileBuildStateManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class FileBuildStateManagerTests
    {
    public:
        [[Fact]]
        void TryLoadFromFile_MissingFile()
        {
            // Register the test listener
            auto testListener = std::make_shared<TestTraceListener>();
            Log::RegisterListener(testListener);

            auto directory = Path("TestFiles/NoFile");
            BuildState actual;
            auto uut = FileBuildStateManager();
            auto result = uut.TryLoadState(directory, actual);

            Assert::IsFalse(result, "Verify result is false.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "BuildState file does not exist.\n",
                }), 
                testListener->GetMessages(),
                "Verify messages match expected.");
        }

        [[Fact]]
        void TryLoadFromFile_GarbageFile()
        {
            // Register the test listener
            auto testListener = std::make_shared<TestTraceListener>();
            Log::RegisterListener(testListener);

            auto directory = Path("TestFiles/GarbageBuildState");
            BuildState actual;
            auto uut = FileBuildStateManager();
            auto result = uut.TryLoadState(directory, actual);

            Assert::IsFalse(result, "Verify result is false.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "Failed to parse BuildState.\n",
                }), 
                testListener->GetMessages(),
                "Verify messages match expected.");
        }

        [[Fact]]
        void TryLoadFromFile_SimpleFile()
        {
            // Register the test listener
            auto testListener = std::make_shared<TestTraceListener>();
            // TODO Log::RegisterListener(testListener);

            auto directory = Path("TestFiles/SimpleBuildState");
            BuildState actual;
            auto uut = FileBuildStateManager();
            auto result = uut.TryLoadState(directory, actual);

            Assert::IsTrue(result, "Verify result is false.");

            auto expected = BuildState({
                    FileInfo("File.h", { "Other.h" }),
                });

            Assert::AreEqual(expected, actual, "Verify matches expected.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                }), 
                testListener->GetMessages(),
                "Verify messages match expected.");
        }
    };
}
