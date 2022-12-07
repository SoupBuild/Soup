// <copyright file="RecipeSML.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe.h"
#include "SML/SML.h"

namespace Soup::Core
{
	/// <summary>
	/// The recipe SML serialize manager
	/// </summary>
	export class RecipeSML
	{
	public:
		/// <summary>
		/// Load from stream
		/// </summary>
		static RecipeTable Deserialize(
			const Path& recipeFile,
			std::istream& stream)
		{
			try
			{
				// Read the contents of the recipe file
				auto root = SMLDocument::Parse(stream);

				// Load the entire root table
				auto table = RecipeTable();
				Parse(table, root.GetRoot());

				return table;
			}
			catch(const std::exception& ex)
			{
				throw std::runtime_error(std::string("Parsing the Recipe SML failed: ") + ex.what() + " " + recipeFile.ToString());
			}
		}

		/// <summary>
		/// Save the recipe to the root file
		/// </summary>
		static void Serialize(RecipeTable& recipeTable, std::ostream& stream)
		{
			// Serialize the contents of the recipe
			auto document = SMLDocument(Build(recipeTable));

			// Write out the entire root table
			stream << document;
		}

	private:
		static RecipeValue Parse(const SMLValue& source)
		{
			switch (source.GetType())
			{
				case SMLValueType::Boolean:
				{
					return RecipeValue(source.AsBoolean());
				}
				case SMLValueType::Integer:
				{
					return RecipeValue(source.AsInteger());
				}
				case SMLValueType::Float:
				{
					return RecipeValue(source.AsFloat());
				}
				case SMLValueType::String:
				{
					return RecipeValue(source.AsString());
				}
				case SMLValueType::Array:
				{
					auto valueList = RecipeList();
					Parse(valueList, source.AsArray());
					return RecipeValue(std::move(valueList));
				}
				case SMLValueType::Table:
				{
					auto valueTable = RecipeTable();
					Parse(valueTable, source.AsTable());
					return RecipeValue(std::move(valueTable));
				}
				default:
				{
					throw std::runtime_error("Unknown SML type.");
				}
			}
		}

		static void Parse(RecipeTable& target, const SMLTable& source)
		{
			for (auto& item : source.GetValue())
			{
				auto value = Parse(item.second);
				target.emplace(item.first, std::move(value));
			}
		}

		static void Parse(RecipeList& target, const SMLArray& source)
		{
			target.reserve(source.GetSize());
			for (size_t i = 0; i < source.GetSize(); i++)
			{
				auto value = Parse(source[i]);
				target.push_back(std::move(value));
			}
		}

		static SMLValue Build(RecipeValue& value)
		{
			switch (value.GetType())
			{
				case RecipeValueType::Table:
					return Build(value.AsTable());
				case RecipeValueType::List:
					return Build(value.AsList());
				case RecipeValueType::String:
					return SMLValue(value.AsString());
				case RecipeValueType::Integer:
					return SMLValue(value.AsInteger());
				case RecipeValueType::Float:
					return SMLValue(value.AsFloat());
				case RecipeValueType::Boolean:
					return SMLValue(value.AsBoolean());
				default:
					throw std::runtime_error("Unknown Recipe value type.");
			}
		}

		static SMLTable Build(RecipeTable& table)
		{
			auto result = std::unordered_map<std::string, SMLValue>();

			for (auto& value : table)
			{
				result.emplace(value.first, Build(value.second));
			}

			return SMLTable(std::move(result));
		}

		static SMLArray Build(RecipeList& list)
		{
			auto result = std::vector<SMLValue>();

			for (auto& value : list)
			{
				result.push_back(Build(value));
			}

			return SMLArray(std::move(result));
		}
	};
}
