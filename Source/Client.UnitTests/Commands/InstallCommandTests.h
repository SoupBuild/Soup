// <copyright file="InstallCommandTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Client::UnitTests
{
    /// <summary>
    /// Install Command Tests
    /// </summary>
    class InstallCommandTests
    {
    public:
        [[Fact]]
        void Initialize()
        {
            auto uut = PackCommand();
        }
    };
}
