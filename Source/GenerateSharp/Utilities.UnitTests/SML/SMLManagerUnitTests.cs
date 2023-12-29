// <copyright file="SMLManagerUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;
using Xunit;

namespace Soup.Build.Utilities.UnitTests;

public class SMLManagerUnitTests
{
	[Fact]
	public void Deserialize_GarbageThrows()
	{
		var recipe = "garbage";
		_ = Assert.Throws<InvalidOperationException>(() =>
		{
			var actual = SMLManager.Deserialize(recipe);
		});
	}

	[Fact]
	public void Deserialize_Simple_Flat()
	{
		var recipe =
			@"Name: 'MyPackage'
				Language: 'C++|1'";
		var actual = SMLManager.Deserialize(recipe);

		var expected = new SMLDocument(
			new Dictionary<string, SMLTableValue>()
			{
					{
						"Name",
						new SMLTableValue(
							new SMLToken("Name"),
							"Name",
							new SMLToken(":"),
							new SMLValue(new SMLStringValue("MyPackage")))
					},
					{
						"Language",
						new SMLTableValue(
							new SMLToken("Language"),
							"Language",
							new SMLToken(":"),
							new SMLValue(new SMLStringValue("C++|1")))
					},
			});

		Assert.Equal(expected, actual);
	}

	[Fact]
	public void Deserialize_ComplexKey()
	{
		var recipe =
			@"'#1%^`': 'ComplexValue'";
		var actual = SMLManager.Deserialize(recipe);

		var expected = new SMLDocument(
			new Dictionary<string, SMLTableValue>()
			{
					{
						"#1%^`",
						new SMLTableValue(
							new SMLToken("'#1%^`'"),
							"#1%^`",
							new SMLToken(":"),
							new SMLValue(new SMLStringValue("ComplexValue")))
					},
			});

		Assert.Equal(expected, actual);
	}

	[Fact]
	public void Deserialize_Simple_Inline()
	{
		var recipe =
			@"Name: 'MyPackage', Language: 'C++|1'";
		var actual = SMLManager.Deserialize(recipe);

		var expected = new SMLDocument(
			new Dictionary<string, SMLTableValue>()
			{
					{
						"Name",
						new SMLTableValue(
							new SMLToken("Name"),
							"Name",
							new SMLToken(":"),
							new SMLValue(new SMLStringValue("MyPackage")))
					},
					{
						"Language",
						new SMLTableValue(
							new SMLToken("Language"),
							"Language",
							new SMLToken(":"),
							new SMLValue(new SMLStringValue("C++|1")))
					},
			});

		Assert.Equal(expected, actual);
	}

	[Fact]
	public void Deserialize_AllProperties()
	{
		var recipe =
			@"Name: 'MyPackage'

				# A Comment in the file
				Language: 'C++|1'
				Version: '1.2.3'
				EnableErrorsAsWarnings: false
				EnableCoolFeature: true
				Dependencies: {
					Runtime:[], Build:[]

					Test :[
						123
						false, 'string' ]
				}";
		var actual = SMLManager.Deserialize(recipe);

		var expected = new SMLDocument(
			new Dictionary<string, SMLTableValue>()
			{
					{
						"Name",
						new SMLTableValue(
							new SMLToken("Name"),
							"Name",
							new SMLToken(":"),
							new SMLValue(new SMLStringValue("MyPackage")))
					},
					{
						"Language",
						new SMLTableValue(
							new SMLToken("Language"),
							"Language",
							new SMLToken(":"),
							new SMLValue(new SMLStringValue("C++|1")))
					},
					{
						"Version",
						new SMLTableValue(
							new SMLToken("Version"),
							"Version",
							new SMLToken(":"),
							new SMLValue(new SMLStringValue("1.2.3")))
					},
					{
						"EnableErrorsAsWarnings",
						new SMLTableValue(
							new SMLToken("EnableErrorsAsWarnings"),
							"EnableErrorsAsWarnings",
							new SMLToken(":"),
							new SMLValue(new SMLBooleanValue(false)))
					},
					{
						"EnableCoolFeature",
						new SMLTableValue(
							new SMLToken("EnableCoolFeature"),
							"EnableCoolFeature",
							new SMLToken(":"),
							new SMLValue(new SMLBooleanValue(true)))
					},
					{
						"Dependencies",
						new SMLTableValue(
							new SMLToken("Dependencies"),
							"Dependencies",
							new SMLToken(":"),
							new SMLValue(new SMLTable(new Dictionary<string, SMLTableValue>()
							{
								{
									"Runtime",
									new SMLTableValue(
										new SMLToken("Runtime"),
										"Runtime",
										new SMLToken(":"),
										new SMLValue(new SMLArray()))
								},
								{
									"Build",
									new SMLTableValue(
										new SMLToken("Build"),
										"Build",
										new SMLToken(":"),
										new SMLValue(new SMLArray()))
								},
								{
									"Test",
									new SMLTableValue(
										new SMLToken("Test"),
										"Test",
										new SMLToken(":"),
										new SMLValue(new SMLArray(new List<SMLArrayValue>()
										{
											new SMLArrayValue(new SMLValue(new SMLIntegerValue(123))),
											new SMLArrayValue(new SMLValue(new SMLBooleanValue(false))),
											new SMLArrayValue(new SMLValue(new SMLStringValue("string"))),
										})))
								},
							})))
					},
			});

		Assert.Equal(expected, actual);
	}

	[Fact]
	public async Task RoundTrip_AllProperties()
	{
		var expected =
			@"Name: 'MyPackage'

				# A Comment in the file
				Language: 'C++|1'
				Version: '1.2.3'
				EnableErrorsAsWarnings: false
				EnableCoolFeature: true
				'Z@#1%': 'Complex'
				Dependencies: {
					Runtime:[], Build:[]

					Test :[
						123
						false, 'string' ]
				}";
		var document = SMLManager.Deserialize(expected);

		var actual = await SerializeAsync(document);

		Assert.Equal(expected, actual);
	}

	private static async Task<string> SerializeAsync(SMLDocument document)
	{
		using var stream = new MemoryStream();
		await SMLManager.SerializeAsync(
			document,
			stream);

		_ = stream.Seek(0, SeekOrigin.Begin);
		using var reader = new StreamReader(stream);

		return await reader.ReadToEndAsync();
	}
}