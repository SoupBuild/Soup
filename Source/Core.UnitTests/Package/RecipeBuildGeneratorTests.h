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
        void Initialze()
        {
            auto uut = RecipeBuildGenerator(nullptr);
        }

        // TODO: Way more of this
    };
}
