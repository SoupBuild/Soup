// <copyright file="RecipeLanguageVersionTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Utilities::UnitTests
{
	class RecipeLanguageVersionTests
	{
	public:
		[[Theory]]
		[[InlineData(Soup::RecipeLanguageVersion::CPP11, "C++11")]]
		[[InlineData(Soup::RecipeLanguageVersion::CPP14, "C++14")]]
		[[InlineData(Soup::RecipeLanguageVersion::CPP17, "C++17")]]
		[[InlineData(Soup::RecipeLanguageVersion::CPP20, "C++20")]]
		void ToStringValues(RecipeLanguageVersion value, std::string expected)
		{
			auto actual = ToString(value);
			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Theory]]
		[[InlineData("C++11", Soup::RecipeLanguageVersion::CPP11)]]
		[[InlineData("C++14", Soup::RecipeLanguageVersion::CPP14)]]
		[[InlineData("C++17", Soup::RecipeLanguageVersion::CPP17)]]
		[[InlineData("C++20", Soup::RecipeLanguageVersion::CPP20)]]
		void ParseValues(std::string value, RecipeLanguageVersion expected)
		{
			auto actual = ParseRecipeLanguageVersion(value);
			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Fact]]
		void ParseGarbageThrows()
		{
			Assert::ThrowsRuntimeError([]() {
				auto actual = ParseRecipeLanguageVersion("garbage");
			});
		}
	};
}
