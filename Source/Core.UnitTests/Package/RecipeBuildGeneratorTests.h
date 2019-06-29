// <copyright file="RecipeBuildGeneratorTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class RecipeBuildGeneratorTests
    {
    public:
        [[Fact]]
        void Initialze_Success()
        {
            auto compiler = std::make_shared<Compiler::Mock::Compiler>();
            auto uut = RecipeBuildGenerator(compiler);
        }

        [[Fact]]
        void Initialze_NullCompilerThrows()
        {
            Assert::ThrowsRuntimeError([]() {
                auto uut = RecipeBuildGenerator(nullptr);
            });
        }

        // TODO: Way more of this
    };
}
