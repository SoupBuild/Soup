// <copyright file="RecipeTypeTests.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::UnitTests
{
	class RecipeTypeTests
	{
	public:
		[[Theory]]
		[[InlineData(Soup::RecipeType::Executable, "Executable")]]
		[[InlineData(Soup::RecipeType::StaticLibrary, "StaticLibrary")]]
		[[InlineData(Soup::RecipeType::DynamicLibrary, "DynamicLibrary")]]
		void ToStringValues(RecipeType value, std::string expected)
		{
			auto actual = ToString(value);
			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Theory]]
		[[InlineData("Executable", Soup::RecipeType::Executable)]]
		[[InlineData("StaticLibrary", Soup::RecipeType::StaticLibrary)]]
		[[InlineData("DynamicLibrary", Soup::RecipeType::DynamicLibrary)]]
		void ParseValues(std::string value, RecipeType expected)
		{
			auto actual = Parse(value);
			Assert::AreEqual(expected, actual, "Verify matches expected.");
		}

		[[Fact]]
		void ParseGarbageThrows()
		{
			Assert::ThrowsRuntimeError([]() {
				auto actual = Parse("garbage");
			});
		}
	};
}
