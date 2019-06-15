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
        }
    };
}
