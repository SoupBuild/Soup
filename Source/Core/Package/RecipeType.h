// <copyright file="RecipeType.cs" company="Soup">
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
		/// Library
		/// </summary>
		Library,
	};

	export std::string ToString(RecipeType value)
	{
		switch (value)
		{
			case RecipeType::Executable:
				return "Executable";
			case RecipeType::Library:
				return "Library";
			default:
				throw std::runtime_error("Unknown recipe type.");
		}
	}

	export RecipeType Parse(const std::string& value)
	{
		if (value == "Executable")
			return RecipeType::Executable;
		else if (value == "Library")
			return RecipeType::Library;
		else
			throw std::runtime_error("Unknown recipe type value.");
	}
}
