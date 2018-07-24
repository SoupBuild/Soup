// <copyright company="Soup" file="PackageSummaryModelTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api.UnitTests
{
	using Xunit;

	public class PackageSummaryModelTests
	{
		[Fact]
		public void DefaultValues()
		{
			var uut = new PackageSummaryModel();

			Assert.Null(uut.Name);
			Assert.Null(uut.Description);
		}

		[Fact]
		public void InitializeValues()
		{
			string name = "TestName";
			string description = "TestDescrition";

			var uut = new PackageSummaryModel()
			{
				Name = name,
				Description = description,
			};

			Assert.Equal(name, uut.Name);
			Assert.Equal(description, uut.Description);
		}
	}
}
