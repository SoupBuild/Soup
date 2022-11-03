// <copyright file="RecipeSMLTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class RecipeSMLTests
	{
	public:
		// [[Fact]]
		void Deserialize_GarbageThrows()
		{
			auto recipeFile = Path("Recipe.sml");
			auto recipe = std::stringstream("garbage");
			auto exception = Assert::Throws<std::runtime_error>([&recipeFile, &recipe]() {
				auto actual = RecipeSML::Deserialize(recipeFile, recipe);
			});
		}

		// [[Fact]]
		void Deserialize_Simple()
		{
			auto recipeFile = Path("Recipe.sml");
			auto recipe = std::stringstream(
				R"(
					Name: "MyPackage"
					Language: "C++|1"
				)");
			auto actual = Recipe(RecipeSML::Deserialize(recipeFile, recipe));

			auto expected = Recipe(
				"MyPackage",
				LanguageReference("C++", SemanticVersion(1)));

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		// [[Fact]]
		void Deserialize_Comments()
		{
			auto recipeFile = Path("Recipe.sml");
			auto recipe = std::stringstream(
				R"(
					# This is an awesome project
					Name: "MyPackage"
					Language: "C++|1"
				)");
			auto actual = Recipe(RecipeSML::Deserialize(recipeFile, recipe));

			auto expected = Recipe(
				"MyPackage",
				LanguageReference("C++", SemanticVersion(1)));
			expected.GetNameValue().GetComments().push_back(" This is an awesome project");

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		// [[Fact]]
		void Deserialize_AllProperties()
		{
			auto recipeFile = Path("Recipe.sml");
			auto recipe = std::stringstream(
				R"(
					Name: "MyPackage"
					Language: "C++|1"
					Version: "1.2.3"
					Dependencies: {
						Runtime: []
						Build: []
						Test: []
					}
				)");
			auto actual = Recipe(RecipeSML::Deserialize(recipeFile, recipe));

			auto expected = Recipe(
				"MyPackage",
				LanguageReference("C++", SemanticVersion(1)),
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>(),
				std::vector<PackageReference>(),
				std::vector<PackageReference>());

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		// [[Fact]]
		void Serialize_Simple()
		{
			auto recipeFile = Path("Recipe.sml");
			auto recipe = Recipe(
				"MyPackage",
				LanguageReference("C++", SemanticVersion(1)));

			std::stringstream actual;
			RecipeSML::Serialize(recipe.GetTable(), actual);

			auto expected = 
R"(Name: "MyPackage"
Language: "C++|1"
)";

			VerifysmlEquals(expected, actual.str(), "Verify matches expected.");
		}

		// [[Fact]]
		void Serialize_Comments()
		{
			auto recipeFile = Path("Recipe.sml");
			auto recipe = Recipe(
				"MyPackage",
				LanguageReference("C++", SemanticVersion(1)));

			recipe.GetNameValue().GetComments().push_back(" This is an awesome package");

			std::stringstream actual;
			RecipeSML::Serialize(recipe.GetTable(), actual);

			auto expected =
R"(// This is an awesome package
Name: "MyPackage"
Language: "C++|1"
)";

			VerifysmlEquals(expected, actual.str(), "Verify matches expected.");
		}

		// [[Fact]]
		void Serialize_AllProperties()
		{
			auto recipeFile = Path("Recipe.sml");
			auto recipe = Recipe(
				"MyPackage",
				LanguageReference("C++", SemanticVersion(1)),
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>(),
				std::vector<PackageReference>(),
				std::vector<PackageReference>());

			std::stringstream actual;
			RecipeSML::Serialize(recipe.GetTable(), actual);

			auto expected = 
R"(Name: "MyPackage"
Language: "C++|1"
Version: "1.2.3"
Dependencies: {Runtime: []
Build: []
Test: []
}
)";

			VerifysmlEquals(expected, actual.str(), "Verify matches expected.");
		}

	private:
		static void VerifysmlEquals(
			const std::string& expected,
			const std::string& actual,
			const std::string& message)
		{
			Assert::AreEqual(expected, actual, message);
		}
	};
}
