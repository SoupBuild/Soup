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
            Assert::AreEqual(uut.GetName(), "Clang", "Verify name match expected.");
            Assert::AreEqual(uut.GetObjectFileExtension(), "o", "Verify object file extension match expected.");
            Assert::AreEqual(uut.GetModuleFileExtension(), "pcm", "Verify module file extension match expected.");
            Assert::AreEqual(uut.GetStaticLibraryFileExtension(), "a", "Verify static library file extension match expected.");
        }

        [[Fact]]
        void Compile_Simple()
        {
            // Register the test process manager
            auto processManager = std::make_shared<MockProcessManager>();
            IProcessManager::Register(processManager);

            auto uut = Compiler();

            CompileArguments arguments = {};
            arguments.SourceFiles =
            {
                Path("File.cpp"),
            };

            auto result = uut.Compile(arguments);

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::string>({
                    "D:/Repos/llvm/build/Release/bin/clang++.exe",
                }),
                processManager->GetRequests(),
                "Verify process manager requests match expected.");

        }
    };
}
