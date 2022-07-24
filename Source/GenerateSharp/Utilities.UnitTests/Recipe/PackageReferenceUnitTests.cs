// <copyright file="PackageReferenceUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
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
			Assert.Throws<InvalidOperationException>(() => { _ = uut.Path; });
			Assert.Throws<InvalidOperationException>(() => { _ = uut.Language; });
			Assert.Throws<InvalidOperationException>(() => { _ = uut.Name; });
			Assert.Throws<InvalidOperationException>(() => { _ = uut.Version; });
		}

		[Fact]
		public void Parse_Local()
		{
			var uut = PackageReference.Parse("../OtherFolder/");
			Assert.True(uut.IsLocal);
			Assert.Equal(new Path("../OtherFolder/"), uut.Path);
			Assert.Throws<InvalidOperationException>(() => { _ = uut.Language; });
			Assert.Throws<InvalidOperationException>(() => { _ = uut.Name; });
			Assert.Throws<InvalidOperationException>(() => { _ = uut.Version; });
		}

		[Fact]
		public void Parse_Public()
		{
			var uut = PackageReference.Parse("Other@1.0.0");
			Assert.False(uut.IsLocal);
			Assert.Throws<InvalidOperationException>(() => { _ = uut.Path; });
			Assert.Null(uut.Language);
			Assert.Equal("Other", uut.Name);
			Assert.Equal(new SemanticVersion(1, 0, 0), uut.Version);
		}

		[Fact]
		public void Parse_Public_Language()
		{
			var uut = PackageReference.Parse("C#|Other@1.0.0");
			Assert.False(uut.IsLocal);
			Assert.Throws<InvalidOperationException>(() => { _ = uut.Path; });
			Assert.Equal("C#", uut.Language);
			Assert.Equal("Other", uut.Name);
			Assert.Equal(new SemanticVersion(1, 0, 0), uut.Version);
		}
	}
}
