// <copyright file="PackageReferenceUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using Xunit;

namespace Soup.Build.Utilities.UnitTests;

public class PackageReferenceUnitTests
{
	[Fact]
	public void DefaultInitializer()
	{
		var uut = new PackageReference();
		Assert.True(uut.IsLocal);
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Path; });
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Language; });
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Owner; });
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Name; });
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Version; });
	}

	[Fact]
	public void Parse_Local()
	{
		var uut = PackageReference.Parse("../OtherFolder/");
		Assert.True(uut.IsLocal);
		Assert.Equal(new Path("../OtherFolder/"), uut.Path);
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Language; });
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Owner; });
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Name; });
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Version; });
	}

	[Fact]
	public void Parse_Public()
	{
		var uut = PackageReference.Parse("Other.Package@1.0.0");
		Assert.False(uut.IsLocal);
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Path; });
		Assert.Null(uut.Language);
		Assert.Null(uut.Owner);
		Assert.Equal("Other.Package", uut.Name);
		Assert.Equal(new SemanticVersion(1, 0, 0), uut.Version);
	}

	[Fact]
	public void Parse_Public_Owner()
	{
		var uut = PackageReference.Parse("User1|Other@1.0.0");
		Assert.False(uut.IsLocal);
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Path; });
		Assert.Null(uut.Language);
		Assert.Equal("User1", uut.Owner);
		Assert.Equal("Other", uut.Name);
		Assert.Equal(new SemanticVersion(1, 0, 0), uut.Version);
	}

	[Fact]
	public void Parse_Public_Owner_Language()
	{
		var uut = PackageReference.Parse("C#:User1|Other@1.0.0");
		Assert.False(uut.IsLocal);
		_ = Assert.Throws<InvalidOperationException>(() => { _ = uut.Path; });
		Assert.Equal("C#", uut.Language);
		Assert.Equal("User1", uut.Owner);
		Assert.Equal("Other", uut.Name);
		Assert.Equal(new SemanticVersion(1, 0, 0), uut.Version);
	}
}