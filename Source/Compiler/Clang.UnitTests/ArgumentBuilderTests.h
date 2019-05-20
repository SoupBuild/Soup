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
            std::string sourceFile = "";
            CompileArguments arguments = {};
            std::string workingDirectory = "";
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments, workingDirectory);

            auto expected = std::vector<std::string>({
                "-c",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }

        [[Fact]]
        void SingleArgument_GenerateIncludeTree()
        {
            std::string sourceFile = "";
            CompileArguments arguments = {};
            arguments.GenerateIncludeTree = true;
            std::string workingDirectory = "";
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments, workingDirectory);

            auto expected = std::vector<std::string>({
                "-H",
                "-c",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }

        [[Fact]]
        void SingleArgument_ExportModule_ThrowsZeroSource()
        {
            std::string sourceFile = "";
            CompileArguments arguments = {};
            arguments.ExportModule = true;
            std::string workingDirectory = "";
            auto test = [&sourceFile, &arguments, &workingDirectory]() {
                auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments, workingDirectory);
            };

            Assert::ThrowsRuntimeError(test);
        }

        [[Fact]]
        void SingleArgument_ExportModule_ThrowsMoreThanOneSource()
        {
            std::string sourceFile = "";
            CompileArguments arguments = {};
            arguments.ExportModule = true;
            arguments.SourceFiles = std::vector<std::string>({
                "module1.cpp",
                "module2.cpp",
            });
            std::string workingDirectory = "";
            Assert::ThrowsRuntimeError([&sourceFile, &arguments, &workingDirectory]() {
                auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments, workingDirectory);
            });
        }

        [[Fact]]
        void SingleArgument_ExportModule_SingleSource()
        {
            std::string sourceFile = "";
            CompileArguments arguments = {};
            arguments.SourceFiles = std::vector<std::string>({
                "module.cpp",
            });
            arguments.ExportModule = true;
            std::string workingDirectory = "";
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments, workingDirectory);

            auto expected = std::vector<std::string>({
                "--precompile",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }
    };
}
