// <copyright file="RecipeUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Utilities.UnitTests
{
	public class RecipeUnitTests
	{
		[Fact]
		public void DefaultInitializer()
		{
			var uut = new Recipe();
			Assert.False(uut.HasVersion);
			Assert.False(uut.HasBuildDependencies);
			Assert.False(uut.HasRuntimeDependencies);
			Assert.False(uut.HasTestDependencies);
		}

		[Fact]
		public void AddRuntimeDependency_Success()
		{
			var uut = new Recipe(
				new SMLDocument(
					new Dictionary<string, SMLTableValue>()
					{
						{ "Name", new SMLTableValue(new SMLToken("Name"), new SMLValue("TestProject")) },
						{ "Language",  new SMLTableValue(new SMLToken("Language"), new SMLValue("C#")) },
					}));

			uut.AddRuntimeDependency("SomeDependency");

			Assert.True(uut.HasRuntimeDependencies);
			Assert.Equal(
				new List<PackageReference>()
				{
					new PackageReference(null, "SomeDependency", null),
				},
				uut.RuntimeDependencies);
		}
	}
}
