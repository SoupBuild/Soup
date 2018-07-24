// <copyright company="Soup" file="PackageCreateModelTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api.UnitTests
{
	using Xunit;

	public class PackageCreateModelTests
	{
		[Fact]
		public void DefaultValues()
		{
			var uut = new PackageCreateModel();

			Assert.Null(uut.Name);
			Assert.Null(uut.Description);
		}

		[Fact]
		public void InitializeValues()
		{
			string name = "TestName";
			string description = "TestDescrition";

			var uut = new PackageCreateModel()
			{
				Name = name,
				Description = description,
			};

			Assert.Equal(name, uut.Name);
			Assert.Equal(description, uut.Description);
		}
	}
}
