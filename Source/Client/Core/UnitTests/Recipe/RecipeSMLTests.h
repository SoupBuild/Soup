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

			auto expected = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
			}));

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

			auto expected = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
			}));

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

			auto expected = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList() },
						{ "Build", RecipeList() },
						{ "Test", RecipeList() },
					})
				},
			}));

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		// [[Fact]]
		void Serialize_Simple()
		{
			auto recipeFile = Path("Recipe.sml");
			auto recipe = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
			}));

			std::stringstream actual;
			RecipeSML::Serialize(recipe.GetTable(), actual);

			auto expected = 
R"(Name: "MyPackage"
Language: "C++|1"
)";

			Assert::AreEqual(expected, actual.str(), "Verify matches expected.");
		}

		// [[Fact]]
		void Serialize_AllProperties()
		{
			auto recipeFile = Path("Recipe.sml");
			auto recipe = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList() },
						{ "Build", RecipeList() },
						{ "Test", RecipeList() },
					})
				},
			}));

			std::stringstream actual;
			RecipeSML::Serialize(recipe.GetTable(), actual);

			auto expected = 
R"(Name: "MyPackage"
Language: "C++|1"
Version: "1.2.3"
Dependencies: {Build: []
Runtime: []
Test: []
}
)";

			Assert::AreEqual(expected, actual.str(), "Verify matches expected.");
		}
	};
}
