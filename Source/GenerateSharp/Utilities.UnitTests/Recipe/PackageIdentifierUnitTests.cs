// <copyright file="PackageIdentifierUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Xunit;

namespace Soup.Build.Utilities.UnitTests;

public class PackageIdentifierUnitTests
{
	[Fact]
	public void DefaultInitializer()
	{
		var uut = new PackageIdentifier();
		Assert.Null(uut.Language);
		Assert.Null(uut.Owner);
		Assert.Equal(string.Empty, uut.Name);
	}

	[Fact]
	public void Parse_Public()
	{
		var uut = PackageIdentifier.Parse("Other");
		Assert.Null(uut.Language);
		Assert.Null(uut.Owner);
		Assert.Equal("Other", uut.Name);
	}

	[Fact]
	public void Parse_Public_Owner()
	{
		var uut = PackageIdentifier.Parse("User1|Other");
		Assert.Null(uut.Language);
		Assert.Equal("User1", uut.Owner);
		Assert.Equal("Other", uut.Name);
	}

	[Fact]
	public void Parse_Public_Owner_Language()
	{
		var uut = PackageIdentifier.Parse("[C#]User1|Other");
		Assert.Equal("C#", uut.Language);
		Assert.Equal("User1", uut.Owner);
		Assert.Equal("Other", uut.Name);
	}
}