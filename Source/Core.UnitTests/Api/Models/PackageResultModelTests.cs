// <copyright company="Soup" file="PackageResultModelTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api.UnitTests
{
	using System.Collections.Generic;
	using Xunit;

	public class PackageResultModelTests
	{
		[Fact]
		public void DefaultValues()
		{
			var uut = new PackageResultModel();

			Assert.Null(uut.Name);
			Assert.Null(uut.Description);
			Assert.Null(uut.Latest);
			Assert.Null(uut.Publications);
		}

		[Fact]
		public void InitializeValues()
		{
			string name = "TestName";
			string description = "TestDescription";
			SemanticVersion latest = new SemanticVersion(1, 0, 0);
			List<PublicationSummaryModel> publications = new List<PublicationSummaryModel>()
			{
			};

			var uut = new PackageResultModel()
			{
				Name = name,
				Description = description,
				Latest = latest,
				Publications = publications,
			};

			Assert.Equal(name, uut.Name);
			Assert.Equal(description, uut.Description);
			Assert.Equal(latest, uut.Latest);
			Assert.Equal(publications, uut.Publications);
		}

		[Fact]
		public void Serialize_Json()
		{
			string name = "TestName";
			string description = "Test Description";
			SemanticVersion latest = new SemanticVersion(1, 2, 3);
			List<PublicationSummaryModel> publications = new List<PublicationSummaryModel>()
			{
			};

			var uut = new PackageResultModel()
			{
				Name = name,
				Description = description,
				Latest = latest,
				Publications = publications,
			};

			var json = TestUtils.JsonSerialize(uut);

			var expected = 
				"{\"name\":\"TestName\"," +
				"\"description\":\"Test Description\"," +
				"\"latest\":\"1.2.3\"," +
				"\"publications\":[]}";

			Assert.Equal(expected, json);
		}

		[Fact]
		public void Deserialize_Json()
		{
			string name = "TestName";
			string description = "Test Description";
			SemanticVersion latest = new SemanticVersion(1, 0, 0);
			List<PublicationSummaryModel> publications = new List<PublicationSummaryModel>()
			{
			};

			var json =
				"{\"name\":\"TestName\"," +
				"\"description\":\"Test Description\"," +
				"\"latest\":\"1.0.0\"," +
				"\"publications\":[]}";

			var value = TestUtils.JsonDeserialize<PackageResultModel>(json);

			Assert.Equal(name, value.Name);
			Assert.Equal(description, value.Description);
			Assert.Equal(latest, value.Latest);
			Assert.Equal(publications, value.Publications);
		}
	}
}
