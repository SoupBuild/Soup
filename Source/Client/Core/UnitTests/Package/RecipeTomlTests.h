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
		void Deserialize_Simple()
		{
			auto recipeFile = Path("Recipe.toml");
			auto recipe = std::stringstream(
				R"(
					Name="MyPackage"
					Language="C++"
				)");
			auto actual = Recipe(RecipeToml::Deserialize(recipeFile, recipe));

			auto expected = Recipe(
				"MyPackage",
				"C++");

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
					Language="C++"
				)");
			auto actual = Recipe(RecipeToml::Deserialize(recipeFile, recipe));

			auto expected = Recipe(
				"MyPackage",
				"C++");
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
					Language="C++"
					Version="1.2.3"
					Dependencies=[]
					DevDependencies=[]
				)");
			auto actual = Recipe(RecipeToml::Deserialize(recipeFile, recipe));

			auto expected = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>(),
				std::vector<PackageReference>());

			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Fact]]
		void Serialize_Simple()
		{
			auto recipeFile = Path("Recipe.toml");
			auto recipe = Recipe(
				"MyPackage",
				"C++");

			std::stringstream actual;
			RecipeToml::Serialize(recipe.GetTable(), actual);

			auto expected = 
R"(Name = "MyPackage"
Language = "C++"
)";

			VerifyTomlEquals(expected, actual.str(), "Verify matches expected.");
		}

		[[Fact]]
		void Serialize_Comments()
		{
			auto recipeFile = Path("Recipe.toml");
			auto recipe = Recipe(
				"MyPackage",
				"C++");

			recipe.GetNameValue().GetComments().push_back(" This is an awesome package");

			std::stringstream actual;
			RecipeToml::Serialize(recipe.GetTable(), actual);

			auto expected =
R"(# This is an awesome package
Name = "MyPackage"
Language = "C++"
)";

			VerifyTomlEquals(expected, actual.str(), "Verify matches expected.");
		}

		[[Fact]]
		void Serialize_AllProperties()
		{
			auto recipeFile = Path("Recipe.toml");
			auto recipe = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>(),
				std::vector<PackageReference>());

			std::stringstream actual;
			RecipeToml::Serialize(recipe.GetTable(), actual);

			auto expected = 
R"(Name = "MyPackage"
Language = "C++"
Version = "1.2.3"
Dependencies = []
DevDependencies = []
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
