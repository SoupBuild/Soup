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
			string description = "TestDescrition";
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
	}
}
