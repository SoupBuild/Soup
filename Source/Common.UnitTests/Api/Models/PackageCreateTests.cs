// <copyright company="Soup" file="PackageCreateTests.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api.UnitTests
{
	using Xunit;

	public class PackageCreateTests
	{
		[Fact]
		public void DefaultValues()
		{
			var uut = new PackageCreate();

			Assert.Null(uut.Name);
			Assert.Null(uut.Description);
		}

		[Fact]
		public void InitializeValues()
		{
			string name = "TestName";
			string description = "TestDescrition";

			var uut = new PackageCreate()
			{
				Name = name,
				Description = description,
			};

			Assert.Equal(name, uut.Name);
			Assert.Equal(description, uut.Description);
		}
	}
}
