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
            CompileArguments arguments = {};
            arguments.SourceFile = Path("File.cpp");
            auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

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
            CompileArguments arguments = {};
            arguments.SourceFile = Path("File.cpp");
            arguments.GenerateIncludeTree = true;
            auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

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
            CompileArguments arguments = {};
            arguments.SourceFile = Path("File.cpp");
            arguments.Standard = LanguageStandard::CPP11;
            auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

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
            CompileArguments arguments = {};
            arguments.SourceFile = Path("File.cpp");
            arguments.Standard = LanguageStandard::CPP14;
            auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

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
            CompileArguments arguments = {};
            arguments.SourceFile = Path("File.cpp");
            arguments.Standard = LanguageStandard::CPP17;
            auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

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
            CompileArguments arguments = {};
            arguments.SourceFile = Path("File.cpp");
            arguments.Standard = LanguageStandard::CPP20;
            auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

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
            CompileArguments arguments = {};
            arguments.SourceFile = Path("File.cpp");
            arguments.IncludeDirectories = std::vector<Path>({
                Path("C:/Files/SDK/"),
                Path("my files/")
            });
            auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

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
            CompileArguments arguments = {};
            arguments.SourceFile = Path("File.cpp");
            arguments.PreprocessorDefinitions = std::vector<std::string>({
                "DEBUG",
                "VERSION=1"
            });
            auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

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
            CompileArguments arguments = {};
            arguments.SourceFile = Path("File.cpp");
            arguments.IncludeModules = std::vector<Path>({
                Path("Module.pcm"),
                Path("Std.pcm")
            });
            auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

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
            CompileArguments arguments = {};
            arguments.SourceFile = Path("");
            arguments.ExportModule = true;
            Assert::ThrowsRuntimeError([&arguments]() {
                auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);
            });
        }

        [[Fact]]
        void SingleArgument_ExportModule_SingleSource()
        {
            CompileArguments arguments = {};
            arguments.SourceFile = Path("module.cpp");
            arguments.ExportModule = true;
            auto actual = ArgumentBuilder::BuildCompilerArguments(arguments);

            auto expected = std::vector<std::string>({
                "-fno-ms-compatibility",
                "-Xclang",
                "-flto-visibility-public-std",
                "-std=c++11",
                "--precompile",
                "module.cpp",
            });
            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }
    };
}
