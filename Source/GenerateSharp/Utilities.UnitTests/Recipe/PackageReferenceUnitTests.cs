// <copyright file="PackageReferenceUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Xunit;

namespace Soup.Build.Utilities.UnitTests
{
	public class PackageReferenceUnitTests
	{
		[Fact]
		public void DefaultInitializer()
		{
			var uut = new PackageReference();
			Assert.True(uut.IsLocal);
			Assert.Equal(new Path(), uut.Path);
		}

		[Fact]
		public void Parse_Local()
		{
			var uut = PackageReference.Parse("../OtherFolder/");
			Assert.True(uut.IsLocal);
			Assert.Equal(new Path("../OtherFolder/"), uut.Path);
		}

		[Fact]
		public void Parse_Public()
		{
			var uut = PackageReference.Parse("Other@1.0.0");
			Assert.False(uut.IsLocal);
			Assert.Equal(string.Empty, uut.Language);
			Assert.Equal("Other", uut.Name);
			Assert.Equal(new SemanticVersion(1, 0, 0), uut.Version);
		}

		[Fact]
		public void Parse_Public_Language()
		{
			var uut = PackageReference.Parse("C#|Other@1.0.0");
			Assert.False(uut.IsLocal);
			Assert.Equal("C#", uut.Language);
			Assert.Equal("Other", uut.Name);
			Assert.Equal(new SemanticVersion(1, 0, 0), uut.Version);
		}
	}
}
