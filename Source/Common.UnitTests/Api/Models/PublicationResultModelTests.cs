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
	}
}
