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
            auto expected = "";

            auto actual = ArgumentBuilder::BuildCompilerArguments("", {}, "");

            Assert::AreEqual(expected, actual, "Verify generated arguments match expected.");
        }
    };
}
