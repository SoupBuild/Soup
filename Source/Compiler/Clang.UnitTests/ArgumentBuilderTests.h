// <copyright file="ArgumentBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

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
                "-fno-ms-compatibility",
                "-Xclang",
                "-flto-visibility-public-std",
                "-std=c++11",
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
                "-fno-ms-compatibility",
                "-Xclang",
                "-flto-visibility-public-std",
                "-H",
                "-std=c++11",
                "-c",
                "File.cpp",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }

        [[Fact]]
        void SingleArgument_LanguageStandard_CPP11()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            arguments.Standard = LanguageStandard::CPP11;
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);

            auto expected = std::vector<std::string>({
                "-fno-ms-compatibility",
                "-Xclang",
                "-flto-visibility-public-std",
                "-std=c++11",
                "-c",
                "File.cpp",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }

        [[Fact]]
        void SingleArgument_LanguageStandard_CPP14()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            arguments.Standard = LanguageStandard::CPP14;
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);

            auto expected = std::vector<std::string>({
                "-fno-ms-compatibility",
                "-Xclang",
                "-flto-visibility-public-std",
                "-std=c++14",
                "-c",
                "File.cpp",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }

        [[Fact]]
        void SingleArgument_LanguageStandard_CPP17()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            arguments.Standard = LanguageStandard::CPP17;
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);

            auto expected = std::vector<std::string>({
                "-fno-ms-compatibility",
                "-Xclang",
                "-flto-visibility-public-std",
                "-std=c++17",
                "-c",
                "File.cpp",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }

        [[Fact]]
        void SingleArgument_LanguageStandard_CPP20()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            arguments.Standard = LanguageStandard::CPP20;
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);

            auto expected = std::vector<std::string>({
                "-fno-ms-compatibility",
                "-Xclang",
                "-flto-visibility-public-std",
                "-std=c++17",
                "-fmodules-ts",
                "-c",
                "File.cpp",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }

        [[Fact]]
        void SingleArgument_IncludePaths()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            arguments.IncludeDirectories = std::vector<Path>({
                Path("C:/Files/SDK/"),
                Path("my files/")
            });
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);

            auto expected = std::vector<std::string>({
                "-fno-ms-compatibility",
                "-Xclang",
                "-flto-visibility-public-std",
                "-std=c++11",
                "-I\"C:/Files/SDK/\"",
                "-I\"my files/\"",
                "-c",
                "File.cpp",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }

        [[Fact]]
        void SingleArgument_PreprocessorDefinitions()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            arguments.PreprocessorDefinitions = std::vector<std::string>({
                "DEBUG",
                "VERSION=1"
            });
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);

            auto expected = std::vector<std::string>({
                "-fno-ms-compatibility",
                "-Xclang",
                "-flto-visibility-public-std",
                "-std=c++11",
                "-DDEBUG",
                "-DVERSION=1",
                "-c",
                "File.cpp",
            });

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }

        [[Fact]]
        void SingleArgument_Modules()
        {
            std::string sourceFile = "File.cpp";
            CompileArguments arguments = {};
            arguments.IncludeModules = std::vector<Path>({
                Path("Module.pcm"),
                Path("Std.pcm")
            });
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);

            auto expected = std::vector<std::string>({
                "-fno-ms-compatibility",
                "-Xclang",
                "-flto-visibility-public-std",
                "-std=c++11",
                "-fmodule-file=\"Module.pcm\"",
                "-fmodule-file=\"Std.pcm\"",
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
            arguments.SourceFiles = std::vector<Path>({
                Path("module1.cpp"),
                Path("module2.cpp"),
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
            arguments.SourceFiles = std::vector<Path>({
                Path("module.cpp"),
            });
            arguments.ExportModule = true;
            auto actual = ArgumentBuilder::BuildCompilerArguments(sourceFile, arguments);

            auto expected = std::vector<std::string>({
                "-fno-ms-compatibility",
                "-Xclang",
                "-flto-visibility-public-std",
                "-std=c++11",
                "--precompile",
                "File.cpp",
            });
            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }
    };
}
