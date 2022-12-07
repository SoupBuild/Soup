// <copyright file="SemanticVersionUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Xunit;

namespace Opal.UnitTests
{
	public class SemanticVersionUnitTests
	{
		[Fact]
		public void DefaultInitializer()
		{
			var uut = new SemanticVersion();
			Assert.Equal(0, uut.Major);
			Assert.Null(uut.Minor);
			Assert.Null(uut.Patch);
		}

		[Theory]
		[InlineData(1, 2, 3)]
		[InlineData(3, 2, 1)]
		[InlineData(1, 1, 1)]
		[InlineData(100, 200, 300)]
		[InlineData(1, 2, 333)]
		[InlineData(1, 222, 3)]
		[InlineData(111, 2, 3)]
		public void InitializeValues(int major, int minor, int patch)
		{
			var uut = new SemanticVersion(major, minor, patch);
			Assert.Equal(major, uut.Major);
			Assert.Equal(minor, uut.Minor);
			Assert.Equal(patch, uut.Patch);
		}

		[Fact]
		public void OperatorEqual()
		{
			var uut = new SemanticVersion(1, 2, 3);

			Assert.Equal(
				new SemanticVersion(1, 2, 3),
				uut);
		}

		[Fact]
		public void OperatorNotEqualMajor()
		{
			var uut = new SemanticVersion(1, 2, 3);

			Assert.NotEqual(
				new SemanticVersion(11, 2, 3),
				uut);
		}

		[Fact]
		public void OperatorNotEqualMinor()
		{
			var uut = new SemanticVersion(1, 2, 3);

			Assert.NotEqual(
				new SemanticVersion(1, 22, 3),
				uut);
		}

		[Fact]
		public void OperatorNotEqualPatch()
		{
			var uut = new SemanticVersion(1, 2, 3);

			Assert.NotEqual(
				new SemanticVersion(1, 2, 33),
				uut);
		}

		[Theory]
		[InlineData("1.2.3", 1, 2, 3)]
		[InlineData("3.2.1", 3, 2, 1)]
		[InlineData("1.1.1", 1, 1, 1)]
		[InlineData("100.200.300", 100, 200, 300)]
		[InlineData("1.2.333", 1, 2, 333)]
		[InlineData("1.222.3", 1, 222, 3)]
		[InlineData("111.2.3", 111, 2, 3)]
		[InlineData("0.0.0", 0, 0, 0)]
		public void ParseValues(string value, int major, int minor, int patch)
		{
			var uut = SemanticVersion.Parse(value);
			Assert.Equal(
				new SemanticVersion(major, minor, patch),
				uut);
		}

		[Theory]
		[InlineData("1.2.3", true, 1, 2, 3)] // Success
		[InlineData("0.0.0", true, 0, 0, 0)] // All zeros success
		[InlineData("", false, 0, null, null)] // Empty fails
		[InlineData("1", true, 1, null, null)] // Major only success
		[InlineData("1.2", true, 1, 2, null)] // Major/Minor only success
		public void TryParseValues(string value, bool expectedResult, int major, int? minor, int? patch)
		{
			var result = SemanticVersion.TryParse(value, out var uut);
			Assert.Equal(
				expectedResult,
				result);
			Assert.Equal(
				new SemanticVersion(major, minor, patch),
				uut);
		}

		[Theory]
		[InlineData(1, 2, 3, "1.2.3")]
		[InlineData(0, 0, 0, "0.0.0")]
		public void ToStringValues(int major, int minor, int patch, string expected)
		{
			var uut = new SemanticVersion(major, minor, patch);
			var value = uut.ToString();
			Assert.Equal(
				expected,
				value);
		}
	}
}
