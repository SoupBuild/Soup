// <copyright file="PackageUpdateModelTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api.UnitTests
{
    using Xunit;

    public class PackageUpdateModelTests
    {
        [Fact]
        public void DefaultValues()
        {
            var uut = new PackageUpdateModel();

            Assert.Null(uut.Description);
        }

        [Fact]
        public void InitializeValues()
        {
            string description = "TestDescription";

            var uut = new PackageUpdateModel()
            {
                Description = description,
            };

            Assert.Equal(description, uut.Description);
        }

        [Fact]
        public void Serialize_Json()
        {
            string description = "Test Description";

            var uut = new PackageUpdateModel()
            {
                Description = description,
            };

            var json = TestUtils.JsonSerialize(uut);

            var expected =
                "{\"description\":\"Test Description\"}";

            Assert.Equal(expected, json);
        }

        [Fact]
        public void Deserialize_Json()
        {
            string description = "Test Description";

            var json =
                "{\"description\":\"Test Description\"}";

            var value = TestUtils.JsonDeserialize<PackageUpdateModel>(json);

            Assert.Equal(description, value.Description);
        }
    }
}
