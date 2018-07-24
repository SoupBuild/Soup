// <copyright company="Soup" file="PublicationSummaryTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api.UnitTests
{
	using Xunit;

	public class PublicationSummaryTests
	{
		[Fact]
		public void DefaultValues()
		{
			var uut = new PublicationSummary();

			Assert.Null(uut.Version);
		}

		[Fact]
		public void InitializeValues()
		{
			SemanticVersion version = new SemanticVersion(1, 2, 3);

			var uut = new PublicationSummary()
			{
				Version = version,
			};

			Assert.Equal(version, uut.Version);
		}
	}
}
