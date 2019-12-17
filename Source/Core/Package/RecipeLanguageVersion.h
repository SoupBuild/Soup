// <copyright file="RecipeLanguageVersion.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
	/// <summary>
	/// The enumeration of recipe language versions
	/// </summary>
	export enum class RecipeLanguageVersion
	{
		/// <summary>
		/// C++ 11
		/// </summary>
		CPP11,

		/// <summary>
		/// C++ 14
		/// </summary>
		CPP14,

		/// <summary>
		/// C++ 17
		/// </summary>
		CPP17,

		/// <summary>
		/// C++ 20
		/// </summary>
		CPP20,
	};

	export std::string ToString(RecipeLanguageVersion value)
	{
		switch (value)
		{
			case RecipeLanguageVersion::CPP11:
				return "C++11";
			case RecipeLanguageVersion::CPP14:
				return "C++14";
			case RecipeLanguageVersion::CPP17:
				return "C++17";
			case RecipeLanguageVersion::CPP20:
				return "C++20";
			default:
				throw std::runtime_error("Unknown recipe language type.");
		}
	}

	export RecipeLanguageVersion ParseRecipeLanguageVersion(const std::string& value)
	{
		if (value == "C++11")
			return RecipeLanguageVersion::CPP11;
		else if (value == "C++14")
			return RecipeLanguageVersion::CPP14;
		else if (value == "C++17")
			return RecipeLanguageVersion::CPP17;
		else if (value == "C++20")
			return RecipeLanguageVersion::CPP20;
		else
			throw std::runtime_error("Unknown recipe language type value.");
	}
}
