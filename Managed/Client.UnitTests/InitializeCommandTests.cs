// <copyright file="InitializeCommandTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Client.UnitTests
{
    using Xunit;

    /// <summary>
    /// Initialize Command Tests
    /// </summary>
    public class InitializeCommandTests
    {
        [Fact]
        public void NameIsCorrect()
        {
            var uut = new InitializeCommand();
        }
    }
}
