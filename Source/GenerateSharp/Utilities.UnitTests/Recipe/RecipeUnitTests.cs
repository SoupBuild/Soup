// <copyright file="RecipeUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Xunit;

namespace Soup.Build.Utilities.UnitTests;

public class RecipeUnitTests
{
	[Fact]
	public void DefaultInitializer()
	{
		var uut = new Recipe();
		Assert.False(uut.HasVersion);
		Assert.False(uut.HasBuildDependencies);
		Assert.False(uut.HasRuntimeDependencies);
	}

	[Fact]
	public void AddRuntimeDependency_Success()
	{
		var uut = new Recipe(
			new SMLDocument(
				new Dictionary<string, SMLTableValue>()
				{
						{
							"Name",
							new SMLTableValue(
								new SMLToken("Name"),
								"Name",
								new SMLToken(":"),
								new SMLValue(new SMLStringValue("TestProject")))
						},
						{
							"Language",
							new SMLTableValue(
								new SMLToken("Language"),
								"Language",
								new SMLToken(":"),
								new SMLValue(new SMLStringValue("C#")))
						},
				}));

		uut.AddRuntimeDependency("Owner|SomeDependency");

		Assert.True(uut.HasRuntimeDependencies);
		Assert.Equal(
			[
				new PackageReference(null, "Owner", "SomeDependency", null),
			],
			uut.RuntimeDependencies);
	}
}