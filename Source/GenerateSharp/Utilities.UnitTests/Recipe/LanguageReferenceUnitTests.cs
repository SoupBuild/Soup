// <copyright file="LanguageReferenceUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Xunit;

namespace Soup.Build.Utilities.UnitTests
{
	public class LanguageReferenceUnitTests
	{
		[Fact]
		public void DefaultInitializer()
		{
			var uut = new LanguageReference();
			Assert.Equal(string.Empty, uut.Name);
			Assert.Equal(new SemanticVersion(), uut.Version);
		}

		[Fact]
		public void Parse_Simple()
		{
			var uut = LanguageReference.Parse("Language|1.2.3");
			Assert.Equal("Language", uut.Name);
			Assert.Equal(new SemanticVersion(1, 2, 3), uut.Version);
		}

		[Fact]
		public void Parse_CSharp()
		{
			var uut = LanguageReference.Parse("C#|1.2.3");
			Assert.Equal("C#", uut.Name);
			Assert.Equal(new SemanticVersion(1, 2, 3), uut.Version);
		}

		[Fact]
		public void Parse_CPlusPlus()
		{
			var uut = LanguageReference.Parse("C++|1.2.3");
			Assert.Equal("C++", uut.Name);
			Assert.Equal(new SemanticVersion(1, 2, 3), uut.Version);
		}
	}
}
