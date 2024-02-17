// <copyright file="PackageNameUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Xunit;

namespace Soup.Build.Utilities.UnitTests;

public class PackageNameUnitTests
{
	[Fact]
	public void DefaultInitializer()
	{
		var uut = new PackageName(null, string.Empty);
		Assert.Null(uut.Owner);
		Assert.Equal("", uut.Name);
	}

	[Fact]
	public void Parse_Public()
	{
		var uut = PackageName.Parse("Other");
		Assert.Null(uut.Owner);
		Assert.Equal("Other", uut.Name);
	}

	[Fact]
	public void Parse_Public_Owner()
	{
		var uut = PackageName.Parse("User1|Other");
		Assert.Equal("User1", uut.Owner);
		Assert.Equal("Other", uut.Name);
	}
}