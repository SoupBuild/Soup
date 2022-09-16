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
				Language: ""C++|1""
				";
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
		public void Deserialize_Simple_Inline()
		{
			var recipe =
				@"Name: ""MyPackage"", Language: ""C++|1"",";
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
						false, ""string"", ]
				},";
			var actual = SMLManager.Deserialize(recipe);

			var expected = new SMLTable(
				new Dictionary<string, SMLValue>()
				{
					{ "Name", new SMLValue("MyPackage") },
					{ "Language", new SMLValue("C++|1") },
					{ "Version", new SMLValue("1.2.3") },
					{ "EnableErrorsAsWarnings", new SMLValue(false) },
					{ "EnableCoolFeature", new SMLValue(true) },
					{ 
						"Dependencies",
						new SMLValue(new SMLTable(new Dictionary<string, SMLValue>()
						{
							{ "Runtime", new SMLValue(new SMLArray()) },
							{ "Build", new SMLValue(new SMLArray()) },
							{ 
								"Test",
								new SMLValue(new SMLArray(new List<SMLValue>()
								{
									new SMLValue(123),
									new SMLValue(false),
									new SMLValue("string"),
								}))
							},
						}))
					},
				});

			Assert.Equal(expected, actual);
		}

		[Fact]
		public async Task Serialize_AllProperties()
		{
			var uut = new SMLDocument(new SMLTable(
				new Dictionary<string, SMLValue>()
				{
					{ "Name", new SMLValue("MyPackage") },
					{ "Language", new SMLValue("C++|1") },
					{ "Version", new SMLValue("1.2.3") },
					{ "EnableErrorsAsWarnings", new SMLValue(false) },
					{ "EnableCoolFeature", new SMLValue(true) },
					{
						"Dependencies",
						new SMLValue(new SMLTable(new Dictionary<string, SMLValue>()
						{
							{ "Runtime", new SMLValue(new SMLArray()) },
							{ "Build", new SMLValue(new SMLArray()) },
							{
								"Test",
								new SMLValue(new SMLArray(new List<SMLValue>()
								{
									new SMLValue(123),
									new SMLValue(false),
									new SMLValue("string"),
								}))
							},
						}))
					},
				}));

			var actual = await SerializeAsync(uut);

			var expected =
@"Name: ""MyPackage""
Language: ""C++|1""
Version: ""1.2.3""
EnableErrorsAsWarnings: False
EnableCoolFeature: True
Dependencies: {
Runtime: [
]
Build: [
]
Test: [
123
False
""string""
]
}
";

			Assert.Equal(expected, actual);
		}

		private async Task<string> SerializeAsync(SMLDocument document)
		{
			using var stream = new MemoryStream();
			await SMLManager.SerializeAsync(document, stream);

			stream.Seek(0, SeekOrigin.Begin);
			using var reader = new StreamReader(stream);

			return await reader.ReadToEndAsync();
		}
	}
}
