// <copyright file="RecipeType.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
	/// <summary>
	/// The enumeration of recipe types
	/// </summary>
	export enum class RecipeType
	{
		/// <summary>
		/// Executable
		/// </summary>
		Executable,

		/// <summary>
		/// Static linked Library
		/// </summary>
		StaticLibrary,

		/// <summary>
		/// Dynamic linked Library
		/// </summary>
		DynamicLibrary,
	};

	export std::string ToString(RecipeType value)
	{
		switch (value)
		{
			case RecipeType::Executable:
				return "Executable";
			case RecipeType::StaticLibrary:
				return "StaticLibrary";
			case RecipeType::DynamicLibrary:
				return "DynamicLibrary";
			default:
				throw std::runtime_error("Unknown recipe type.");
		}
	}

	export RecipeType ParseRecipeType(std::string_view value)
	{
		if (value == "Executable")
			return RecipeType::Executable;
		else if (value == "StaticLibrary")
			return RecipeType::StaticLibrary;
		else if (value == "DynamicLibrary")
			return RecipeType::DynamicLibrary;
		else
			throw std::runtime_error("Unknown recipe type value.");
	}
}
