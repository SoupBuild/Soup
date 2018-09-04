// <copyright file="PublicationSummaryModelTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api.UnitTests
{
    using Xunit;

    public class PublicationSummaryModelTests
    {
        [Fact]
        public void DefaultValues()
        {
            var uut = new PublicationSummaryModel();

            Assert.Null(uut.Version);
        }

        [Fact]
        public void InitializeValues()
        {
            SemanticVersion version = new SemanticVersion(1, 2, 3);

            var uut = new PublicationSummaryModel()
            {
                Version = version,
            };

            Assert.Equal(version, uut.Version);
        }

        [Fact]
        public void Serialize_Json()
        {
            SemanticVersion version = new SemanticVersion(1, 2, 3);

            var uut = new PublicationSummaryModel()
            {
                Version = version,
            };

            var json = TestUtils.JsonSerialize(uut);

            var expected =
                "{\"version\":\"1.2.3\"}";

            Assert.Equal(expected, json);
        }

        [Fact]
        public void Deserialize_Json()
        {
            SemanticVersion version = new SemanticVersion(1, 2, 3);

            var json =
                "{\"version\":\"1.2.3\"}";

            var value = TestUtils.JsonDeserialize<PublicationSummaryModel>(json);

            Assert.Equal(version, value.Version);
        }
    }
}
