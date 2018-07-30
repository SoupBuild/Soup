// <copyright company="Soup" file="PublicationGetResultTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api.UnitTests
{
	using System;
	using Xunit;

	public class PublicationResultModelTests
	{
		[Fact]
		public void DefaultValues()
		{
			var uut = new PublicationResultModel();

			Assert.Null(uut.Version);
			Assert.Equal(new DateTime(), uut.DatePublished);
			Assert.Equal(0, uut.TotalDownloads);
		}

		[Fact]
		public void InitializeValues()
		{
			SemanticVersion version = new SemanticVersion(1, 2, 3);
			DateTime datePublished = new DateTime(1);
			int totalDownloads = 1;

			var uut = new PublicationResultModel()
			{
				Version = version,
				DatePublished = datePublished,
				TotalDownloads = totalDownloads,
			};

			Assert.Equal(version, uut.Version);
			Assert.Equal(datePublished, uut.DatePublished);
			Assert.Equal(totalDownloads, uut.TotalDownloads);
		}

		[Fact]
		public void Serialize_Json()
		{
			SemanticVersion version = new SemanticVersion(1, 2, 3);
			DateTime datePublished = new DateTime(1);
			int totalDownloads = 1;

			var uut = new PublicationResultModel()
			{
				Version = version,
				DatePublished = datePublished,
				TotalDownloads = totalDownloads,
			};

			var json = TestUtils.JsonSerialize(uut);

			var expected =
				"{\"version\":\"1.2.3\"," +
				"\"datePublished\":\"0001-01-01T00:00:00.0000001\"," +
				"\"totalDownloads\":1}";

			Assert.Equal(expected, json);
		}

		[Fact]
		public void Deserialize_Json()
		{
			SemanticVersion version = new SemanticVersion(1, 2, 3);
			DateTime datePublished = new DateTime(1);
			int totalDownloads = 1;

			var json = 
				"{\"version\":\"1.2.3\"," +
				"\"datePublished\":\"0001-01-01T00:00:00.0000001\"," +
				"\"totalDownloads\":1}";

			var value = TestUtils.JsonDeserialize<PublicationResultModel>(json);

			Assert.Equal(version, value.Version);
			Assert.Equal(datePublished, value.DatePublished);
			Assert.Equal(totalDownloads, value.TotalDownloads);
		}
	}
}
