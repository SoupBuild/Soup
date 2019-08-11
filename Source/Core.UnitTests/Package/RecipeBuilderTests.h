// <copyright file="RecipeBuilderTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class RecipeBuilderTests
    {
    public:
        [[Fact]]
        void Initialize_Success()
        {
            auto compiler = std::make_shared<Compiler::Mock::Compiler>();
            auto uut = RecipeBuilder(compiler);
        }

        [[Fact]]
        void Initialize_NullCompilerThrows()
        {
            Assert::ThrowsRuntimeError([]() {
                auto uut = RecipeBuilder(nullptr);
            });
        }

        // TODO: Way more of this
    };
}
