// <copyright file="PackageNameTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class PackageNameTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto uut = PackageName(
				"User1",
				"MyPackage");

			Assert::AreEqual("User1", uut.GetOwner(), "Verify owner matches expected.");
			Assert::AreEqual("MyPackage", uut.GetName(), "Verify name matches expected.");
		}

		// [[Fact]]
		void OperatorEqual()
		{
			auto uut = PackageName(
				"User1",
				"MyPackage");

			Assert::AreEqual(
				PackageName(
					"User1",
					"MyPackage"),
				uut,
				"Verify are equal.");
		}

		// [[Fact]]
		void OperatorNotEqualOwner()
		{
			auto uut = PackageName(
				"User1",
				"MyPackage");

			Assert::AreNotEqual(
				PackageName(
					"User2",
					"MyPackage"),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualName()
		{
			auto uut = PackageName(
				"User1",
				"MyPackage");

			Assert::AreNotEqual(
				PackageName(
					"User1",
					"MyPackage2"),
				uut,
				"Verify are not equal.");
		}

		// [[Theory]]
		// [[InlineData("Name", std::nullopt, "Name")]]
		// [[InlineData("User1|Name", "User1", "Name")]]
		void ParseValues(std::string value, std::optional<std::string> owner, std::string name)
		{
			auto uut = PackageName::Parse(value);
			Assert::AreEqual(
				PackageName(owner, name),
				uut,
				"Verify matches expected values.");
		}

		// [[Theory]]
		// [[InlineData("Package", true)]] // Success
		void TryParseValues(std::string value, bool expectedResult)
		{
			PackageReference uut;
			auto result = PackageReference::TryParse(value, uut);
			Assert::AreEqual(
				expectedResult,
				result,
				"Verify matches expected result.");
		}

		// [[Theory]]
		// [[InlineData(std::nullopt, "Name", "Name")]]
		// [[InlineData("User1", "Name", "User1|Name")]]
		void ToStringValues(std::optional<std::string> owner, std::string name, std::string expected)
		{
			auto uut = PackageName(owner, name);
			auto value = uut.ToString();
			Assert::AreEqual(
				expected,
				value,
				"Verify matches expected value.");
		}
	};
}
