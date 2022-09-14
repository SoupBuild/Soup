// <copyright file="SMLManagerUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using Xunit;

namespace Soup.Build.Utilities.UnitTests
{
	public class SMLManagerUnitTests
	{
		[Fact]
		public void Deserialize_GarbageThrows()
		{
			var recipe = "garbage";
			Assert.Throws<InvalidOperationException>(() => {
				var actual = SMLManager.Deserialize(recipe);
			});
		}

		[Fact]
		public void Deserialize_Simple()
		{
			var recipe =
				@"Name = ""MyPackage""
				Language = ""C++|1""";
			var actual = SMLManager.Deserialize(recipe);

			var expected = new SMLTable(
				new Dictionary<string, SMLValue>()
				{
					{ "Name", new SMLValue("MyPackage") },
					{ "Language", new SMLValue("C++|1") },
				});

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
			var actual = SMLManager.Deserialize(recipe);

			var expected = new SMLTable(
				new Dictionary<string, SMLValue>()
				{
					{ "Name", new SMLValue("MyPackage") },
					{ "Language", new SMLValue("C++|1") },
					{ "Version", new SMLValue("1.2.3") },
					{ 
						"Dependencies",
						new SMLValue(new SMLTable(new Dictionary<string, SMLValue>()
						{
							{ "Runtime", new SMLValue(new SMLArray()) },
							{ "Build", new SMLValue(new SMLArray()) },
							{ "Test", new SMLValue(new SMLArray()) },
						}))
					},
				});

			Assert.Equal(expected, actual);
		}
	}
}
