// <copyright file="RecipeExtensionsTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class RecipeExtensionsTests
    {
    public:
        [[Fact]]
        void TryLoadFromFile_MissingFile()
        {
            // Register the test listener
            auto testListener = std::make_shared<TestTraceListener>();
            Log::RegisterListener(testListener);

            auto directory = Path("TestFiles/NoFile/Recipe.json");
            Recipe actual;
            auto result = RecipeExtensions::TryLoadFromFile(directory, actual);

            Assert::IsFalse(result, "Verify result is false.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "Recipe file does not exist.\n",
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

            auto directory = Path("TestFiles/GarbageRecipe/Recipe.json");
            Recipe actual;
            auto result = RecipeExtensions::TryLoadFromFile(directory, actual);

            Assert::IsFalse(result, "Verify result is false.");

            // Verify expected logs
            Assert::AreEqual(
                std::vector<std::string>({
                    "Failed to parse Recipe.\n",
                }), 
                testListener->GetMessages(),
                "Verify messages match expected.");
        }

        [[Fact]]
        void TryLoadFromFile_SimpleFile()
        {
            // Register the test listener
            auto testListener = std::make_shared<TestTraceListener>();
            Log::RegisterListener(testListener);

            auto directory = Path("TestFiles/SimpleRecipe/ReciPe.json");
            Recipe actual;
            auto result = RecipeExtensions::TryLoadFromFile(directory, actual);

            Assert::IsTrue(result, "Verify result is false.");

            auto expected = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3));

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
