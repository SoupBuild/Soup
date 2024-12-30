// <copyright file="PackageIdentifierTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class PackageIdentifierTests
	{
	public:
		// [[Fact]]
		void Initialize()
		{
			auto uut = PackageIdentifier(
				"C#",
				"User1",
				"MyPackage");

			Assert::AreEqual("C#", uut.GetLanguage(), "Verify language matches expected.");
			Assert::AreEqual("User1", uut.GetOwner(), "Verify owner matches expected.");
			Assert::AreEqual("MyPackage", uut.GetName(), "Verify name matches expected.");
		}

		// [[Fact]]
		void OperatorEqual()
		{
			auto uut = PackageIdentifier(
				"C#",
				"User1",
				"MyPackage");

			Assert::AreEqual(
				PackageIdentifier(
					"C#",
					"User1",
					"MyPackage"),
				uut,
				"Verify are equal.");
		}

		// [[Fact]]
		void OperatorNotEqualLanguage()
		{
			auto uut = PackageIdentifier(
				"C#",
				"User1",
				"MyPackage");

			Assert::AreNotEqual(
				PackageIdentifier(
					"C++",
					"User1",
					"MyPackage"),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualOwner()
		{
			auto uut = PackageIdentifier(
				"C#",
				"User1",
				"MyPackage");

			Assert::AreNotEqual(
				PackageIdentifier(
					"C#",
					"User2",
					"MyPackage"),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualName()
		{
			auto uut = PackageIdentifier(
				"C#",
				"User1",
				"MyPackage");

			Assert::AreNotEqual(
				PackageIdentifier(
					"C#",
					"User1",
					"MyPackage2"),
				uut,
				"Verify are not equal.");
		}

		// [[Theory]]
		// [[InlineData("Name", std::nullopt, std::nullopt, "Name")]]
		// [[InlineData("[C#]Name", "C#", std::nullopt, "Name")]]
		// [[InlineData("User1|Name", std::nullopt, "User1", "Name")]]
		// [[InlineData("[C#]User1|Name", "C#", "User1", "Name")]]
		void ParseValues(std::string value, std::optional<std::string> language, std::optional<std::string> owner, std::string name)
		{
			auto uut = PackageIdentifier::Parse(value);
			Assert::AreEqual(
				PackageIdentifier(language, owner, name),
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
		// [[InlineData(std::nullopt, std::nullopt, "Name", "Name")]]
		// [[InlineData("C#", std::nullopt, "Name", "[C#]Name")]]
		// [[InlineData(std::nullopt, "User1", "Name", "User1|Name")]]
		// [[InlineData("C#", "User1", "Name", "[C#]User1|Name")]]
		void ToStringValues(std::optional<std::string> language, std::optional<std::string> owner, std::string name, std::string expected)
		{
			auto uut = PackageIdentifier(language, owner, name);
			auto value = uut.ToString();
			Assert::AreEqual(
				expected,
				value,
				"Verify matches expected value.");
		}
	};
}
