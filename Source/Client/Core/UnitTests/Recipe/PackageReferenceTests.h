// <copyright file="PackageReferenceTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class PackageReferenceTests
	{
	public:
		// [[Fact]]
		void InitializeNamed()
		{
			auto uut = PackageReference(
				"C#",
				"MyPackage",
				SemanticVersion(1, 2, 3));

			Assert::IsFalse(uut.IsLocal(), "Verify is not local.");
			Assert::AreEqual("C#", uut.GetLanguage(), "Verify language matches expected.");
			Assert::AreEqual("MyPackage", uut.GetName(), "Verify name matches expected.");
			Assert::AreEqual(SemanticVersion(1, 2, 3), uut.GetVersion(), "Verify version matches expected.");
		}

		// [[Fact]]
		void InitializePath()
		{
			auto uut = PackageReference(Path("../MyPackage"));

			Assert::IsTrue(uut.IsLocal(), "Verify is local.");
			Assert::AreEqual(Path("../MyPackage"), uut.GetPath(), "Verify path matches expected.");
		}

		// [[Fact]]
		void OperatorEqualNameVersion()
		{
			auto uut = PackageReference(
				"C#",
				"MyPackage",
				SemanticVersion(1, 2, 3));

			Assert::AreEqual(
				PackageReference(
					"C#",
					"MyPackage",
					SemanticVersion(1, 2, 3)),
				uut,
				"Verify are equal.");
		}

		// [[Fact]]
		void OperatorEqualPath()
		{
			auto uut = PackageReference(Path("../MyPackage"));

			Assert::AreEqual(
				PackageReference(Path("../MyPackage")),
				uut,
				"Verify are equal.");
		}

		// [[Fact]]
		void OperatorNotEqualLanguage()
		{
			auto uut = PackageReference(
				"C#",
				"MyPackage",
				SemanticVersion(1, 2, 3));

			Assert::AreNotEqual(
				PackageReference(
					"C++",
					"MyPackage",
					SemanticVersion(1, 2, 3)),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualName()
		{
			auto uut = PackageReference(
				"C#",
				"MyPackage",
				SemanticVersion(1, 2, 3));

			Assert::AreNotEqual(
				PackageReference(
					"C#",
					"MyPackage2",
					SemanticVersion(1, 2, 3)),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualVersion()
		{
			auto uut = PackageReference(
				"C#",
				"MyPackage",
				SemanticVersion(1, 2, 3));

			Assert::AreNotEqual(
				PackageReference(
					"C#",
					"MyPackage",
					SemanticVersion(11, 2, 3)),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualPath()
		{
			auto uut = PackageReference(Path("../MyPackage"));

			Assert::AreNotEqual(
				PackageReference(Path("../MyPackage2")),
				uut,
				"Verify are not equal.");
		}

		// [[Theory]]
		// [[InlineData("Name", std::nullopt, "Name", std::nullopt)]]
		// [[InlineData("Name@1.2.3", std::nullopt, "Name", Soup::SemanticVersion(1, 2, 3))]]
		// [[InlineData("C#|Name", "C#", "Name", std::nullopt)]]
		// [[InlineData("C#|Name@1.2.3", "C#", "Name", Soup::SemanticVersion(1, 2, 3))]]
		void ParseNamedValues(std::string value, std::optional<std::string> language, std::string name, std::optional<SemanticVersion> version)
		{
			auto uut = PackageReference::Parse(value);
			Assert::AreEqual(
				PackageReference(language, name, version),
				uut,
				"Verify matches expected values.");
		}

		// [[Theory]]
		// [[InlineData("../Path")]]
		void ParsePathValues(std::string value)
		{
			auto uut = PackageReference::Parse(value);
			Assert::AreEqual(
				PackageReference(Path(value)),
				uut,
				"Verify matches expected values.");
		}

		// [[Theory]]
		// [[InlineData("Package@1.2.3", true)]] // Success
		// [[InlineData("Package@2", false)]] // Fails invalid version
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
		// [[InlineData(std::nullopt, "Name", std::nullopt, "Name")]]
		// [[InlineData(std::nullopt, "Name", Soup::SemanticVersion(1, 2, 3), "Name@1.2.3")]]
		// [[InlineData("C#", "Name", std::nullopt, "C#|Name")]]
		// [[InlineData("C#", "Name", Soup::SemanticVersion(1, 2, 3), "C#|Name@1.2.3")]]
		void ToStringNamedValues(std::optional<std::string> language, std::string name, std::optional<SemanticVersion> version, std::string expected)
		{
			auto uut = PackageReference(language, name, version);
			auto value = uut.ToString();
			Assert::AreEqual(
				expected,
				value,
				"Verify matches expected value.");
		}

		// [[Theory]]
		// [[InlineData("../Path")]]
		void ToStringPathValues(std::string path)
		{
			auto uut = PackageReference(Path(path));
			auto value = uut.ToString();
			Assert::AreEqual(
				path,
				value,
				"Verify matches expected value.");
		}
	};
}
