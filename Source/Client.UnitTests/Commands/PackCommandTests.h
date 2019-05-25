// <copyright file="PackCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Client::UnitTests
{
    /// <summary>
    /// Pack Command Tests
    /// </summary>
    class PackCommandTests
    {
    public:
        [[Fact]]
        void Initialize()
        {
            auto uut = PackCommand();
        }
    };
}
