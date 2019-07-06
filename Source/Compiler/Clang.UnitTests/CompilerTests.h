// <copyright file="CompilerTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Compiler::Clang::UnitTests
{
    class CompilerTests
    {
    public:
        [[Fact]]
        void Initialize()
        {
            auto uut = Compiler();
            Assert::AreEqual(uut.GetName(), std::string_view("Clang"), "Verify name match expected.");
            Assert::AreEqual(uut.GetObjectFileExtension(), std::string_view("o"), "Verify object file extension match expected.");
            Assert::AreEqual(uut.GetModuleFileExtension(), std::string_view("pcm"), "Verify module file extension match expected.");
            Assert::AreEqual(uut.GetStaticLibraryFileExtension(), std::string_view("a"), "Verify static library file extension match expected.");
        }

        [[Fact]]
        void Compile_Simple()
        {
            // Register the test process manager
            auto processManager = std::make_shared<MockProcessManager>();
            IProcessManager::Register(processManager);

            auto uut = Compiler();

            CompileArguments arguments = {};
            arguments.SourceFile = Path("File.cpp");

            auto result = uut.Compile(arguments);

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::pair<std::string, std::vector<std::string>>>({
                    std::make_pair(
                        "D:/Repos/llvm/build/Release/bin/clang++.exe",
                        std::vector<std::string>({
                            "-fno-ms-compatibility",
                            "-Xclang",
                            "-flto-visibility-public-std",
                            "-std=c++11",
                            "-c",
                            "File.cpp",
                        })),
                }),
                processManager->GetRequests(),
                "Verify process manager requests match expected.");
        }
    };
}
