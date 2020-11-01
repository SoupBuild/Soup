// <copyright file="RecipeTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::UnitTests
{
	class RecipeTests
	{
	public:
		// [[Fact]]
		void InitializerDefault()
		{
			auto uut = Recipe();

			Assert::AreEqual<std::string_view>("", uut.GetName(), "Verify name matches expected.");
			Assert::AreEqual<std::string_view>("", uut.GetLanguage(), "Verify language matches expected.");
			Assert::IsFalse(uut.HasVersion(), "Verify has no version.");
			Assert::IsFalse(uut.HasRuntimeDependencies(), "Verify has no runtime dependencies.");
			Assert::IsFalse(uut.HasBuildDependencies(), "Verify has no build dependencies.");
			Assert::IsFalse(uut.HasTestDependencies(), "Verify has no test dependencies.");
		}

		// [[Fact]]
		void InitializerAll()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../DevTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreEqual<std::string_view>("MyPackage", uut.GetName(), "Verify name matches expected.");
			Assert::AreEqual<std::string_view>("C++", uut.GetLanguage(), "Verify language matches expected.");
			Assert::IsTrue(uut.HasVersion(), "Verify has version.");
			Assert::AreEqual(SemanticVersion(1, 2, 3), uut.GetVersion(), "Verify version is correct.");
			Assert::IsTrue(uut.HasRuntimeDependencies(), "Verify has runtime dependencies.");
			Assert::AreEqual(
				std::vector<PackageReference>({
					PackageReference(Path(Path("../OtherPackage/"))),
				}),
				uut.GetRuntimeDependencies(),
				"Verify runtime dependencies are correct.");
			Assert::IsTrue(uut.HasBuildDependencies(), "Verify has build dependencies.");
			Assert::AreEqual(
				std::vector<PackageReference>({
					PackageReference(Path(Path("../DevTask/"))),
				}),
				uut.GetBuildDependencies(),
				"Verify build dependencies are correct.");
			Assert::IsTrue(uut.HasTestDependencies(), "Verify has test dependencies.");
			Assert::AreEqual(
				std::vector<PackageReference>({
					PackageReference(Path(Path("../TestPackage/"))),
				}),
				uut.GetTestDependencies(),
				"Verify test dependencies are correct.");
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
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../TestPackage/")),
					})),
				uut,
				"Verify equal.");
		}

		// [[Fact]]
		void OperatorNotEqualName()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage2",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../TestPackage/")),
					})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualLanguage()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C#",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../TestPackage/")),
					})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualVersion()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(11, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../TestPackage/")),
					})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualNoVersion()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					std::nullopt,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../TestPackage/")),
					})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualRuntimeDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../TestPackage/")),
					})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualNoRuntimeDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::nullopt,
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../TestPackage/")),
					})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualBuildDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage/")),
					}),
					std::vector<PackageReference>({
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../TestPackage/")),
					})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualNoBuildDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage/")),
					}),
					std::nullopt,
					std::vector<PackageReference>({
						PackageReference(Path("../TestPackage/")),
					})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualTestDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask/")),
					}),
					std::vector<PackageReference>({
					})),
				uut,
				"Verify are not equal.");
		}

		// [[Fact]]
		void OperatorNotEqualNoTestDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				"C++",
				SemanticVersion(1, 2, 3),
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask/")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../TestPackage/")),
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					"C++",
					SemanticVersion(1, 2, 3),
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage/")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask/")),
					}),
					std::nullopt),
				uut,
				"Verify are not equal.");
		}
	};
}
