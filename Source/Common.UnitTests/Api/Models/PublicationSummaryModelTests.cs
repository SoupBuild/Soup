// <copyright company="Soup" file="PublicationSummaryModelTests.cs">
//   Copyright (c) Soup.  All rights reserved.
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
	}
}
