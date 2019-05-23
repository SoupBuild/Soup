// <copyright file="SemanticVersionTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.UnitTests
{
    using Xunit;

    public class SemanticVersionTests
    {
        [Theory]
        [InlineData(1, 2, 3)]
        [InlineData(3, 2, 1)]
        [InlineData(1, 1, 1)]
        [InlineData(100, 200, 300)]
        [InlineData(1, 2, 333)]
        [InlineData(1, 222, 3)]
        [InlineData(111, 2, 3)]
        public void InitializeValues(short major, short minor, short patch)
        {
            var uut = new SemanticVersion(major, minor, patch);
            Assert.Equal(major, uut.Major);
            Assert.Equal(minor, uut.Minor);
            Assert.Equal(patch, uut.Patch);
        }
    }
}
