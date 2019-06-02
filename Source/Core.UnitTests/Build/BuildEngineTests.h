// <copyright file="BuildEngineTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class BuildEngineTests
    {
    public:
        [[Fact]]
        void InitialzeEngine()
        {
            auto mockCompiler = std::make_shared<MockCompiler>();
            auto uut = BuildEngine(mockCompiler);
        }
    };
}
