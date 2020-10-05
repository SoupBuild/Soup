// <copyright file="RecipeTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class RecipeTests
	{
	public:
		[[Fact]]
		void InitializerDefault()
		{
			auto uut = Recipe();

			Assert::AreEqual<std::string_view>("", uut.GetName(), "Verify name matches expected.");
			Assert::AreEqual<std::string_view>("", uut.GetVersion(), "Verify language matches expected.");
			Assert::IsFalse(uut.HasVersion(), "Verify has no version.");
			Assert::IsFalse(uut.HasDependencies(), "Verify has no dependencies.");
			Assert::IsFalse(uut.HasDevDependencies(), "Verify has no dev dependencies.");
		}

		[[Fact]]
		void InitializerAll()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../DevTask")),
				}));

			Assert::AreEqual<std::string_view>("MyPackage", uut.GetName(), "Verify name matches expected.");
			Assert::AreEqual<std::string_view>("C++", uut.GetLanguage(), "Verify language matches expected.");
			Assert::IsTrue(uut.HasVersion(), "Verify has version.");
			Assert::AreEqual(SemanticVersion(1, 2, 3), uut.GetVersion(), "Verify version is correct.");
			Assert::IsTrue(uut.HasDependencies(), "Verify has dependencies.");
			Assert::AreEqual(
				std::vector<PackageReference>({
					PackageReference(Path(Path("../OtherPackage"))),
				}),
				uut.GetDependencies(),
				"Verify dependencies are correct.");
			Assert::IsTrue(uut.HasDevDependencies(), "Verify has dev dependencies.");
			Assert::AreEqual(
				std::vector<PackageReference>({
					PackageReference(Path(Path("../DevTask"))),
				}),
				uut.GetDevDependencies(),
				"Verify dev dependencies are correct.");
		}

		[[Fact]]
		void OperatorEqualDefault()
		{
			auto uut = Recipe();

			Assert::AreEqual(Recipe(), uut, "Verify equal.");
		}

		[[Fact]]
		void OperatorEqualAll()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}));

			Assert::AreEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					})),
				uut,
				"Verify equal.");
		}

		[[Fact]]
		void OperatorNotEqualName()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage2",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualLanguage()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++"
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C#"
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualVersion()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(11, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualNoVersion()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					std::nullopt,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualNoDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::nullopt,
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualDevDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualNoDevDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::nullopt),
				uut,
				"Verify are not equal.");
		}
	};
}
