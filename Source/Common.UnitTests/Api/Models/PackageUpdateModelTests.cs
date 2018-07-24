// <copyright company="Soup" file="PackageUpdateModelTests.cs">
//   Copyright (c) Soup.  All rights reserved.
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
			string description = "TestDescrition";

			var uut = new PackageUpdateModel()
			{
				Description = description,
			};
			
			Assert.Equal(description, uut.Description);
		}
	}
}
