// <copyright file="RecipeManagerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class RecipeManagerTests
    {
    public:
        [[Fact]]
        void Parse_GarbageThrows()
        {
            auto recipe = std::stringstream("garbage");
            Assert::ThrowsRuntimeError([&recipe]() {
                auto actual = RecipeManager::LoadFromStream(recipe);
            });
        }

        [[Fact]]
        void Parse_MissingNameThrows()
        {
            auto recipe = std::stringstream(
                R"({
                    "version": "1.2.3"
                })");

            Assert::ThrowsRuntimeError([&recipe]() {
                auto actual = RecipeManager::LoadFromStream(recipe);
            });
        }

        [[Fact]]
        void Parse_MissingVersionThrows()
        {
            auto recipe = std::stringstream(
                R"({
                   "name": "MyPackage"
                })");

            Assert::ThrowsRuntimeError([&recipe]() {
                auto actual = RecipeManager::LoadFromStream(recipe);
            });
        }

        [[Fact]]
        void ParseSimple()
        {
            auto recipe = std::stringstream(
                R"({
                   "name": "MyPackage",
                   "version": "1.2.3"
                })");
            auto actual = RecipeManager::LoadFromStream(recipe);

            auto expected = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3));

            Assert::AreEqual(expected, actual, "Verify matches expected.");
        }

        [[Fact]]
        void Parse_AllProperties()
        {
            auto recipe = std::stringstream(
                R"({
                   "name": "MyPackage",
                   "version": "1.2.3",
                   "type": "Executable",
                   "dependencies": [],
                   "public": "Public.cpp",
                   "source": []
                })");
            auto actual = RecipeManager::LoadFromStream(recipe);

            auto expected = Recipe(
                "MyPackage",
                SemanticVersion(1, 2, 3),
                RecipeType::Executable,
                std::vector<PackageReference>(),
                "Public.cpp",
                std::vector<std::string>());

            Assert::AreEqual(expected, actual, "Verify matches expected.");
        }
    };
}
