// <copyright file="RecipeTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::UnitTests
{
	class RecipeTests
	{
	public:
		// [[Fact]]
		void InitializerDefault()
		{
			auto uut = Recipe();

			Assert::IsFalse(uut.HasName(), "Verify has no name.");
			Assert::IsFalse(uut.HasLanguage(), "Verify has no language.");
			Assert::IsFalse(uut.HasVersion(), "Verify has no version.");

			Assert::AreEqual(std::vector<std::string>(), uut.GetDependencyTypes(), "Verify no dependency types");
		}

		// [[Fact]]
		void InitializerAll()
		{
			auto uut = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList({ "../OtherPackage/" }) },
						{ "Build", RecipeList({ "../DevTask/" }) },
					})
				},
			}));

			Assert::IsTrue(uut.HasName(), "Verify has name.");
			Assert::AreEqual<std::string_view>("MyPackage", uut.GetName(), "Verify name matches expected.");
			Assert::IsTrue(uut.HasLanguage(), "Verify has language.");
			Assert::AreEqual(LanguageReference("C++", SemanticVersion(1)), uut.GetLanguage(), "Verify language matches expected.");
			Assert::IsTrue(uut.HasVersion(), "Verify has version.");
			Assert::AreEqual(SemanticVersion(1, 2, 3), uut.GetVersion(), "Verify version is correct.");
			
			Assert::AreEqual(
				std::vector<std::string>({ "Runtime", "Build" }),
				uut.GetDependencyTypes(),
				"Verify dependency types");

			Assert::AreEqual(
				std::vector<PackageReference>({
					PackageReference(Path(Path("../OtherPackage/"))),
				}),
				uut.GetNamedDependencies("Runtime"),
				"Verify runtime dependencies are correct.");
			Assert::AreEqual(
				std::vector<PackageReference>({
					PackageReference(Path(Path("../DevTask/"))),
				}),
				uut.GetNamedDependencies("Build"),
				"Verify build dependencies are correct.");
		}

		// [[Fact]]
		void OperatorEqualDefault()
		{
			auto uut = Recipe();

			Assert::AreEqual(Recipe(), uut, "Verify equal.");
		}

		// [[Fact]]
		void OperatorEqualAll()
		{
			auto uut = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList({ "../OtherPackage/" }) },
						{ "Build", RecipeList({ "../DevTask/" }) },
					})
				},
			}));

			Assert::AreEqual(
				Recipe(RecipeTable(
				{
					{ "Name", "MyPackage" },
					{ "Language", "C++|1" },
					{ "Version", "1.2.3" },
					{
						"Dependencies",
						RecipeTable(
						{
							{ "Runtime", RecipeList({ "../OtherPackage/" }) },
							{ "Build", RecipeList({ "../DevTask/" }) },
						})
					},
				})),
				uut,
				"Verify equal.");
		}

		// [[Fact]]
		void OperatorNotEqualName()
		{
			auto uut = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList({ "../OtherPackage/" }) },
						{ "Build", RecipeList({ "../DevTask/" }) },
					})
				},
			}));

			Assert::AreNotEqual(
				Recipe(RecipeTable(
				{
					{ "Name", "MyPackage2" },
					{ "Language", "C++|1" },
					{ "Version", "1.2.3" },
					{
						"Dependencies",
						RecipeTable(
						{
							{ "Runtime", RecipeList({ "../OtherPackage/" }) },
							{ "Build", RecipeList({ "../DevTask/" }) },
						})
					},
				})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualLanguage()
		{
			auto uut = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList({ "../OtherPackage/" }) },
						{ "Build", RecipeList({ "../DevTask/" }) },
					})
				},
			}));

			Assert::AreNotEqual(
				Recipe(RecipeTable(
				{
					{ "Name", "MyPackage" },
					{ "Language", "C#|1" },
					{ "Version", "1.2.3" },
					{
						"Dependencies",
						RecipeTable(
						{
							{ "Runtime", RecipeList({ "../OtherPackage/" }) },
							{ "Build", RecipeList({ "../DevTask/" }) },
						})
					},
				})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualVersion()
		{
			auto uut = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList({ "../OtherPackage/" }) },
						{ "Build", RecipeList({ "../DevTask/" }) },
					})
				},
			}));

			Assert::AreNotEqual(
				Recipe(RecipeTable(
				{
					{ "Name", "MyPackage" },
					{ "Language", "C++|1" },
					{ "Version", "2.2.3" },
					{
						"Dependencies",
						RecipeTable(
						{
							{ "Runtime", RecipeList({ "../OtherPackage/" }) },
							{ "Build", RecipeList({ "../DevTask/" }) },
						})
					},
				})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualNoVersion()
		{
			auto uut = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList({ "../OtherPackage/" }) },
						{ "Build", RecipeList({ "../DevTask/" }) },
					})
				},
			}));

			Assert::AreNotEqual(
				Recipe(RecipeTable(
				{
					{ "Name", "MyPackage" },
					{ "Language", "C++|1" },
					{
						"Dependencies",
						RecipeTable(
						{
							{ "Runtime", RecipeList({ "../OtherPackage/" }) },
							{ "Build", RecipeList({ "../DevTask/" }) },
						})
					},
				})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualRuntimeDependencies()
		{
			auto uut = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList({ "../OtherPackage/" }) },
						{ "Build", RecipeList({ "../DevTask/" }) },
					})
				},
			}));

			Assert::AreNotEqual(
				Recipe(RecipeTable(
				{
					{ "Name", "MyPackage" },
					{ "Language", "C++|1" },
					{ "Version", "1.2.3" },
					{
						"Dependencies",
						RecipeTable(
						{
							{ "Runtime", RecipeList({ "../OtherPackage2/" }) },
							{ "Build", RecipeList({ "../DevTask/" }) },
						})
					},
				})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualNoRuntimeDependencies()
		{
			auto uut = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList({ "../OtherPackage/" }) },
						{ "Build", RecipeList({ "../DevTask/" }) },
					})
				},
			}));

			Assert::AreNotEqual(
				Recipe(RecipeTable(
				{
					{ "Name", "MyPackage" },
					{ "Language", "C++|1" },
					{ "Version", "1.2.3" },
					{
						"Dependencies",
						RecipeTable(
						{
							{ "Build", RecipeList({ "../DevTask/" }) },
						})
					},
				})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualBuildDependencies()
		{
			auto uut = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList({ "../OtherPackage/" }) },
						{ "Build", RecipeList({ "../DevTask/" }) },
					})
				},
			}));

			Assert::AreNotEqual(
				Recipe(RecipeTable(
				{
					{ "Name", "MyPackage" },
					{ "Language", "C++|1" },
					{ "Version", "1.2.3" },
					{
						"Dependencies",
						RecipeTable(
						{
							{ "Runtime", RecipeList({ "../OtherPackage/" }) },
							{ "Build", RecipeList({ "../DevTask2/" }) },
						})
					},
				})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualNoBuildDependencies()
		{
			auto uut = Recipe(RecipeTable(
			{
				{ "Name", "MyPackage" },
				{ "Language", "C++|1" },
				{ "Version", "1.2.3" },
				{
					"Dependencies",
					RecipeTable(
					{
						{ "Runtime", RecipeList({ "../OtherPackage/" }) },
						{ "Build", RecipeList({ "../DevTask/" }) },
					})
				},
			}));

			Assert::AreNotEqual(
				Recipe(RecipeTable(
				{
					{ "Name", "MyPackage" },
					{ "Language", "C++|1" },
					{ "Version", "1.2.3" },
					{
						"Dependencies",
						RecipeTable(
						{
							{ "Runtime", RecipeList({ "../OtherPackage/" }) },
						})
					},
				})),
				uut,
				"Verify are not equal.");
		}
	};
}
