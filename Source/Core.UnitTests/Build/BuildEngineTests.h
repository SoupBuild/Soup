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

        // [[Fact]]
        // void Compile_Executable_SingleFile()
        // {
        //     auto mockCompiler = std::make_shared<MockCompiler>();
        //     auto uut = BuildEngine(mockCompiler);

        //     auto arguments = BuildArguments();
        //     argumes.Source = 
        // }
    };
}
