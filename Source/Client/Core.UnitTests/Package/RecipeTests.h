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
			Assert::AreEqual(SemanticVersion(0, 0, 0), uut.GetVersion(), "Verify version is correct.");
			Assert::IsFalse(uut.HasType(), "Verify has no type.");
			Assert::IsFalse(uut.HasLanguageVersion(), "Verify has no language version.");
			Assert::IsFalse(uut.HasDependencies(), "Verify has no dependencies.");
			Assert::IsFalse(uut.HasExtensions(), "Verify has no extensions.");
			Assert::IsFalse(uut.HasPublic(), "Verify has no public.");
			Assert::IsFalse(uut.HasSource(), "Verify has no source.");
			Assert::IsFalse(uut.HasIncludePaths(), "Verify has no include paths.");
			Assert::IsFalse(uut.HasDefines(), "Verify has no defines.");
		}

		[[Fact]]
		void InitializerAll()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP20,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../DevTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreEqual<std::string_view>("MyPackage", uut.GetName(), "Verify name matches expected.");
			Assert::AreEqual(SemanticVersion(1, 2, 3), uut.GetVersion(), "Verify version is correct.");
			Assert::IsTrue(uut.HasType(), "Verify has type.");
			Assert::AreEqual(Build::Extensions::RecipeType::Executable, uut.GetType(), "Verify type is correct.");
			Assert::IsTrue(uut.HasLanguageVersion(), "Verify has language version.");
			Assert::AreEqual(Build::Extensions::RecipeLanguageVersion::CPP20, uut.GetLanguageVersion(), "Verify language version is correct.");
			Assert::IsTrue(uut.HasDependencies(), "Verify has dependencies.");
			Assert::AreEqual(
				std::vector<PackageReference>({
					PackageReference(Path(Path("../OtherPackage"))),
				}),
				uut.GetDependencies(),
				"Verify dependencies are correct.");
			Assert::IsTrue(uut.HasExtensions(), "Verify has extensions.");
			Assert::AreEqual(
				std::vector<PackageReference>({
					PackageReference(Path(Path("../DevTask"))),
				}),
				uut.GetExtensions(),
				"Verify extensions are correct.");
			Assert::IsTrue(uut.HasPublic(), "Verify has public.");
			Assert::AreEqual<std::string_view>("Main.cpp", uut.GetPublic(), "Verify public is correct.");
			Assert::IsTrue(uut.HasSource(), "Verify has source.");
			Assert::AreEqual(
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				uut.GetSource(),
				"Verify source is correct.");
			Assert::IsTrue(uut.HasIncludePaths(), "Verify has include paths.");
			Assert::AreEqual(
				std::vector<std::string>({
					"Include/",
				}),
				uut.GetIncludePaths(),
				"Verify include paths are correct.");
			Assert::IsTrue(uut.HasDefines(), "Verify has defines.");
			Assert::AreEqual(
				std::vector<std::string>({
					"MY_FLAG",
				}),
				uut.GetDefines(),
				"Verify include defines are correct.");
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
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify equal.");
		}

		[[Fact]]
		void OperatorNotEqualName()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage2",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualVersion()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(11, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualType()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::StaticLibrary,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualNoType()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					std::nullopt,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualLanguageVersion()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP20,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualNoLanguageVersion()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					std::nullopt,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualNoDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::nullopt,
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualDevDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualNoDevDependencies()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::nullopt,
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualPublic()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main2.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualNoPublic()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					std::nullopt,
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualSource()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualNoSource()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::nullopt,
					std::vector<std::string>({
						"Include/",
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualIncludePaths()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
					}),
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualNoIncludePaths()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::nullopt,
					std::vector<std::string>({
						"MY_FLAG",
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualDefines()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
					"Include/",
					}),
					std::vector<std::string>({
					})),
				uut,
				"Verify are not equal.");
		}

		[[Fact]]
		void OperatorNotEqualNoDefines()
		{
			auto uut = Recipe(
				"MyPackage",
				SemanticVersion(1, 2, 3),
				Build::Extensions::RecipeType::Executable,
				Build::Extensions::RecipeLanguageVersion::CPP17,
				std::vector<PackageReference>({
					PackageReference(Path("../OtherPackage")),
				}),
				std::vector<PackageReference>({
					PackageReference(Path("../BuildTask")),
				}),
				"Main.cpp",
				std::vector<std::string>({
					"SomeFile.cpp",
				}),
				std::vector<std::string>({
					"Include/",
				}),
				std::vector<std::string>({
					"MY_FLAG",
				}));

			Assert::AreNotEqual(
				Recipe(
					"MyPackage",
					SemanticVersion(1, 2, 3),
					Build::Extensions::RecipeType::Executable,
					Build::Extensions::RecipeLanguageVersion::CPP17,
					std::vector<PackageReference>({
						PackageReference(Path("../OtherPackage")),
					}),
					std::vector<PackageReference>({
						PackageReference(Path("../BuildTask")),
					}),
					"Main.cpp",
					std::vector<std::string>({
						"SomeFile.cpp",
					}),
					std::vector<std::string>({
						"Include/",
					}),
					std::nullopt),
				uut,
				"Verify are not equal.");
		}
	};
}
