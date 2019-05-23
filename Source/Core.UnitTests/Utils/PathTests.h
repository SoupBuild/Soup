// <copyright file="PathTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class PathTests
    {
    public:
        [[Fact]]
        void DefaultInitializer()
        {
            auto uut = Path();
            Assert::AreEqual("", uut.GetFileName(), "Verify file name matches.");
        }
    };
}
