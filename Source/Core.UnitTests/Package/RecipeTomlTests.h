// <copyright file="RecipeTomlTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class RecipeTomlTests
	{
	public:
		[[Fact]]
		void Deserialize_GarbageThrows()
		{
			auto recipe = std::stringstream("garbage");
			Assert::ThrowsRuntimeError([&recipe]() {
				auto actual = RecipeToml::Deserialize(recipe);
			});
		}

		[[Fact]]
		void Deserialize_MissingNameThrows()
		{
			auto recipe = std::stringstream(
				R"(
					Version="1.2.3"
				)");

			Assert::ThrowsRuntimeError([&recipe]() {
				auto actual = RecipeToml::Deserialize(recipe);
			});
		}

		[[Fact]]
		void Deserialize_MissingVersionThrows()
		{
			auto recipe = std::stringstream(
				R"(
					Name="MyPackage"
				)");

			Assert::ThrowsRuntimeError([&recipe]() {
				auto actual = RecipeToml::Deserialize(recipe);
			});
		}

		[[Fact]]
		void Deserialize_Simple()
		{
			auto recipe = std::stringstream(
				R"(
					Name="MyPackage"
					Version="1.2.3"
				)");
			auto actual = RecipeToml::Deserialize(recipe);

			auto expected = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3));

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Fact]]
		void Deserialize_AllProperties()
		{
			auto recipe = std::stringstream(
				R"(
					Name="MyPackage"
					Version="1.2.3"
					Type="Executable"
					Language="C++17"
					Dependencies=[]
					Extensions=[]
					Public="Public.cpp"
					Source=[]
					IncludePaths=[]
					Defines=[]
				)");
			auto actual = RecipeToml::Deserialize(recipe);

			auto expected = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				RecipeType::Executable,
				RecipeLanguageVersion::CPP17,
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
			RecipeToml::Serialize(recipe, actual);

			auto expected = 
R"(Name = "MyPackage"
Version = "1.2.3"
)";

			VerifyTomlEquals(expected, actual.str(), "Verify matches expected.");
		}

		[[Fact]]
		void Serialize_AllProperties()
		{
			auto recipe = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				RecipeType::Executable,
				RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>(),
				std::vector<PackageReference>(),
				"Public.cpp",
				std::vector<std::string>(),
				std::vector<std::string>(),
				std::vector<std::string>());

			std::stringstream actual;
			RecipeToml::Serialize(recipe, actual);

			auto expected = 
R"(Language = "C++17"
Extensions = []
Defines = []
Source = []
Public = "Public.cpp"
Dependencies = []
IncludePaths = []
Name = "MyPackage"
Type = "Executable"
Version = "1.2.3"
)";

			VerifyTomlEquals(expected, actual.str(), "Verify matches expected.");
		}

	private:
		static void VerifyTomlEquals(
			const std::string& expected,
			const std::string& actual,
			const std::string& message)
		{
			Assert::AreEqual(expected, actual, message);
		}
	};
}
