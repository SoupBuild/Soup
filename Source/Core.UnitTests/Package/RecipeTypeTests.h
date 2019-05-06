// <copyright file="RecipeTypeTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
    class RecipeTypeTests
    {
    public:
        [[Theory]]
        [[InlineData(Soup::RecipeType::Executable, "Executable")]]
        [[InlineData(Soup::RecipeType::Library, "Library")]]
        void ToStringValues(RecipeType value, std::string expected)
        {
            auto actual = ToString(value);
            Assert::AreEqual(expected, actual, "Verify matches expected.");
        }
    };
}
