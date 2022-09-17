// <copyright file="SMLManagerUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
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
		public void Deserialize_Simple_Flat()
		{
			var recipe =
				@"Name: ""MyPackage""
				Language: ""C++|1""";
			var actual = SMLManager.Deserialize(recipe);

			var expected = new SMLDocument(
				new Dictionary<string, SMLTableValue>()
				{
					{ 
						"Name",
						new SMLTableValue(new SMLToken("Name"), new SMLValue(new SMLStringValue("MyPackage")))
					},
					{
						"Language",
						new SMLTableValue(new SMLToken("Language"), new SMLValue(new SMLStringValue("C++|1")))
					},
				});

			Assert.Equal(expected, actual);
		}

		[Fact]
		public void Deserialize_Simple_Inline()
		{
			var recipe =
				@"Name: ""MyPackage"", Language: ""C++|1""";
			var actual = SMLManager.Deserialize(recipe);

			var expected = new SMLDocument(
				new Dictionary<string, SMLTableValue>()
				{
					{ "Name", new SMLTableValue(new SMLToken("Name"), new SMLValue(new SMLStringValue("MyPackage"))) },
					{ "Language", new SMLTableValue(new SMLToken("Language"), new SMLValue(new SMLStringValue("C++|1"))) },
				});

			Assert.Equal(expected, actual);
		}

		[Fact]
		public void Deserialize_AllProperties()
		{
			var recipe =
				@"Name: ""MyPackage""

				// A Comment in the file
				Language: ""C++|1""
				Version: ""1.2.3""
				EnableErrorsAsWarnings: false
				EnableCoolFeature: true
				Dependencies: {
					Runtime:[], Build:[]

					Test :[
						123
						false, ""string"" ]
				}";
			var actual = SMLManager.Deserialize(recipe);

			var expected = new SMLDocument(
				new Dictionary<string, SMLTableValue>()
				{
					{ "Name", new SMLTableValue(new SMLToken("Name"), new SMLValue(new SMLStringValue("MyPackage"))) },
					{ "Language", new SMLTableValue(new SMLToken("Language"), new SMLValue(new SMLStringValue("C++|1"))) },
					{ "Version", new SMLTableValue(new SMLToken("Version"), new SMLValue(new SMLStringValue("1.2.3"))) },
					{ "EnableErrorsAsWarnings", new SMLTableValue(new SMLToken("EnableErrorsAsWarnings"), new SMLValue(false)) },
					{ "EnableCoolFeature", new SMLTableValue(new SMLToken("EnableCoolFeature"), new SMLValue(true)) },
					{ 
						"Dependencies",
						new SMLTableValue(new SMLToken("Dependencies"), new SMLValue(new SMLTable(new Dictionary<string, SMLTableValue>()
						{
							{ "Runtime", new SMLTableValue(new SMLToken("Runtime"), new SMLValue(new SMLArray())) },
							{ "Build", new SMLTableValue(new SMLToken("Build"), new SMLValue(new SMLArray())) },
							{ 
								"Test",
								new SMLTableValue(new SMLToken("Test"), new SMLValue(new SMLArray(new List<SMLValue>()
								{
									new SMLValue(123),
									new SMLValue(false),
									new SMLValue(new SMLStringValue("string")),
								})))
							},
						})))
					},
				});

			Assert.Equal(expected, actual);
		}
	}
}
