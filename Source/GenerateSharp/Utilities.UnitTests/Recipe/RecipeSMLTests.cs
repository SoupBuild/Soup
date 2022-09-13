// <copyright file="RecipeSMLTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using Xunit;

namespace Soup.Build.Utilities.UnitTests
{
	public class RecipeSMLTests
	{
		[Fact]
		public void Deserialize_GarbageThrows()
		{
			var recipe = "garbage";
			Assert.Throws<InvalidOperationException>(() => {
				var actual = RecipeSML.Deserialize(recipe);
			});
		}

		[Fact]
		public void Deserialize_Simple()
		{
			var recipe =
				@"Name = ""MyPackage""
				Language = ""C++|1""";
			var actual = RecipeSML.Deserialize(recipe);

			var expected = new Recipe(
				"MyPackage",
				new LanguageReference("C++", new SemanticVersion(1)));

			Assert.Equal(expected, actual);
		}

		[Fact]
		public void Deserialize_AllProperties()
		{
			var recipe =
				@"Name = ""MyPackage""
				Language = ""C++|1""
				Version = ""1.2.3""
				Dependencies = {
					Runtime =[]
					Build =[]
					Test =[]
				}";
			var actual = RecipeSML.Deserialize(recipe);

			var expected = new Recipe(
				"MyPackage",
				new LanguageReference("C++", new SemanticVersion(1)));

			Assert.Equal(expected, actual);
		}
	}
}
