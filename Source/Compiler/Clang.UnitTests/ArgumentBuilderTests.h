// <copyright file="ArgumentBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
using namespace SoupTest;

namespace Soup::Compiler::Clang::UnitTests
{
    class ArgumentBuilderTests
    {
    public:
        [[Fact]]
        void NoParameters()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);

            auto expected = std::vector<std::string>({
                "-c",
                "File.cpp",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }

        [[Fact]]
        void SingleArgument_GenerateIncludeTree()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            arguments.GenerateIncludeTree = true;
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);

            auto expected = std::vector<std::string>({
                "-H",
                "-c",
                "File.cpp",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }

        [[Fact]]
        void SingleArgument_ExportModule_ThrowsZeroSource()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            arguments.ExportModule = true;
            Assert::ThrowsRuntimeError([&sourceFile, &arguments]() {
                auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);
            });
        }

        [[Fact]]
        void SingleArgument_ExportModule_ThrowsMoreThanOneSource()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            arguments.ExportModule = true;
            arguments.SourceFiles = std::vector<std::string>({
                "module1.cpp",
                "module2.cpp",
            });
            Assert::ThrowsRuntimeError([&sourceFile, &arguments]() {
                auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);
            });
        }

        [[Fact]]
        void SingleArgument_ExportModule_SingleSource()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            arguments.SourceFiles = std::vector<std::string>({
                "module.cpp",
            });
            arguments.ExportModule = true;
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);

            auto expected = std::vector<std::string>({
                "--precompile",
                "File.cpp",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }
    };
}
