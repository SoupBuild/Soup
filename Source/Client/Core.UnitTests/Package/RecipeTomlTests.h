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
			auto recipeFile = Path("Recipe.toml");
			auto recipe = std::stringstream("garbage");
			Assert::ThrowsRuntimeError([&recipeFile, &recipe]() {
				auto actual = RecipeToml::Deserialize(recipeFile, recipe);
			});
		}

		[[Fact]]
		void Deserialize_MissingNameThrows()
		{
			auto recipeFile = Path("Recipe.toml");
			auto recipe = std::stringstream(
				R"(
					Version="1.2.3"
				)");

			Assert::ThrowsRuntimeError([&recipeFile, &recipe]() {
				auto actual = RecipeToml::Deserialize(recipeFile, recipe);
			});
		}

		[[Fact]]
		void Deserialize_MissingVersionThrows()
		{
			auto recipeFile = Path("Recipe.toml");
			auto recipe = std::stringstream(
				R"(
					Name="MyPackage"
				)");

			Assert::ThrowsRuntimeError([&recipeFile, &recipe]() {
				auto actual = RecipeToml::Deserialize(recipeFile, recipe);
			});
		}

		[[Fact]]
		void Deserialize_Simple()
		{
			auto recipeFile = Path("Recipe.toml");
			auto recipe = std::stringstream(
				R"(
					Name="MyPackage"
					Version="1.2.3"
				)");
			auto actual = RecipeToml::Deserialize(recipeFile, recipe);

			auto expected = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3));

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Fact]]
		void Deserialize_Comments()
		{
			auto recipeFile = Path("Recipe.toml");
			auto recipe = std::stringstream(
				R"(
					# This is an awesome project
					Name="MyPackage"
					Version="1.2.3"
				)");
			auto actual = RecipeToml::Deserialize(recipeFile, recipe);

			auto expected = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3));
			expected.GetNameValue().GetComments().push_back(" This is an awesome project");

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}


		[[Fact]]
		void Deserialize_AllProperties()
		{
			auto recipeFile = Path("Recipe.toml");
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
			auto actual = RecipeToml::Deserialize(recipeFile, recipe);

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
			auto recipeFile = Path("Recipe.toml");
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
		void Serialize_Comments()
		{
			auto recipeFile = Path("Recipe.toml");
			auto recipe = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3));

			recipe.GetNameValue().GetComments().push_back(" This is an awesome package");

			std::stringstream actual;
			RecipeToml::Serialize(recipe, actual);

			auto expected =
R"(# This is an awesome package
Name = "MyPackage"
Version = "1.2.3"
)";

			VerifyTomlEquals(expected, actual.str(), "Verify matches expected.");
		}

		[[Fact]]
		void Serialize_AllProperties()
		{
			auto recipeFile = Path("Recipe.toml");
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
			RecipeToml::Serialize(recipe, actual);

			auto expected = 
R"(Name = "MyPackage"
Version = "1.2.3"
Type = "Executable"
Language = "C++17"
Extensions = []
Dependencies = []
Public = "Public.cpp"
Source = []
IncludePaths = []
Defines = []
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
