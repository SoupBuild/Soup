// <copyright file="RecipeJsonTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class RecipeJsonTests
	{
	public:
		[[Fact]]
		void Deserialize_GarbageThrows()
		{
			auto recipe = std::stringstream("garbage");
			Assert::ThrowsRuntimeError([&recipe]() {
				auto actual = RecipeJson::Deserialize(recipe);
			});
		}

		[[Fact]]
		void Deserialize_MissingNameThrows()
		{
			auto recipe = std::stringstream(
				R"({
					"Version": "1.2.3"
				})");

			Assert::ThrowsRuntimeError([&recipe]() {
				auto actual = RecipeJson::Deserialize(recipe);
			});
		}

		[[Fact]]
		void Deserialize_MissingVersionThrows()
		{
			auto recipe = std::stringstream(
				R"({
					"Name": "MyPackage"
				})");

			Assert::ThrowsRuntimeError([&recipe]() {
				auto actual = RecipeJson::Deserialize(recipe);
			});
		}

		[[Fact]]
		void Deserialize_Simple()
		{
			auto recipe = std::stringstream(
				R"({
					"Name": "MyPackage",
					"Version": "1.2.3"
				})");
			auto actual = RecipeJson::Deserialize(recipe);

			auto expected = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3));

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Fact]]
		void Deserialize_AllProperties()
		{
			auto recipe = std::stringstream(
				R"({
					"Name": "MyPackage",
					"Version": "1.2.3",
					"Type": "Executable",
					"Language": "C++17",
					"Dependencies": [],
					"DevDependencies": [],
					"Public": "Public.cpp",
					"Source": [],
					"IncludePaths": [],
					"Defines": []
				})");
			auto actual = RecipeJson::Deserialize(recipe);

			auto expected = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>(),
				std::vector<PackageReference>(),
				"Public.cpp",
				std::vector<std::string>(),
				std::vector<std::string>(),
				std::vector<std::string>());

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Fact]]
		void Serialize_Simple()
		{
			auto recipe = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3));

			std::stringstream actual;
			RecipeJson::Serialize(recipe, actual);

			auto expected = 
				R"({
					"Name": "MyPackage",
					"Version": "1.2.3"
				})";

			VerifyJsonEquals(expected, actual.str(), "Verify matches expected.");
		}

		[[Fact]]
		void Serialize_AllProperties()
		{
			auto recipe = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>(),
				std::vector<PackageReference>(),
				"Public.cpp",
				std::vector<std::string>(),
				std::vector<std::string>(),
				std::vector<std::string>());

			std::stringstream actual;
			RecipeJson::Serialize(recipe, actual);

			auto expected = 
				R"({
					"Defines": [],
					"Dependencies": [],
					"DevDependencies": [],
					"IncludePaths": [],
					"Language": "C++17",
					"Name": "MyPackage",
					"Public": "Public.cpp",
					"Source": [],
					"Type": "Executable",
					"Version": "1.2.3"
				})";

			VerifyJsonEquals(expected, actual.str(), "Verify matches expected.");
		}

	private:
		static void VerifyJsonEquals(
			const std::string& expected,
			const std::string& actual,
			const std::string& message)
		{
			// Cleanup the expected json
			std::string error;
			auto jsonExpected = json11::Json::parse(expected, error);

			Assert::AreEqual(jsonExpected.dump(), actual, message);
		}
	};
}
