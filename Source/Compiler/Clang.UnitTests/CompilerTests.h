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
            arguments.TargetFile = Path("obj/File.o");

            auto result = uut.Compile(arguments);

            // Verify expected file system requests
            Assert::AreEqual(
                std::vector<std::string>({
                    ": D:/Repos/llvm/build/Release/bin/clang++.exe -Wno-unknown-attributes -Xclang -flto-visibility-public-std -std=c++11 -c File.cpp -o obj/File.o",
                }),
                processManager->GetRequests(),
                "Verify process manager requests match expected.");
        }
    };
}
