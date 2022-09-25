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
		static void Parse(RecipeValue& target, const SMLValue& source)
		{
			// Copy over all of the comments
			// // target.GetComments().insert(
			// // 	target.GetComments().end(),
			// // 	source.comments().begin(),
			// // 	source.comments().end());

			switch (source.GetType())
			{
				case SMLValueType::Empty:
				{
					// Leave empty
					break;
				}
				case SMLValueType::Boolean:
				{
					target.SetValueBoolean(source.AsBoolean());
					break;
				}
				case SMLValueType::Integer:
				{
					target.SetValueInteger(source.AsInteger());
					break;
				}
				case SMLValueType::Float:
				{
					target.SetValueFloat(source.AsFloat());
					break;
				}
				case SMLValueType::String:
				{
					target.SetValueString(source.AsString());
					break;
				}
				case SMLValueType::Array:
				{
					auto valueList = RecipeList();
					Parse(valueList, source.AsArray());
					target.SetValueList(std::move(valueList));
					break;
				}
				case SMLValueType::Table:
				{
					auto valueTable = RecipeTable();
					Parse(valueTable, source.AsTable());
					target.SetValueTable(std::move(valueTable));
					break;
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
				auto value = RecipeValue();
				Parse(value, item.second);
				target.emplace(item.first, std::move(value));
			}
		}

		static void Parse(RecipeList& target, const SMLArray& source)
		{
			target.reserve(source.GetSize());
			for (size_t i = 0; i < source.GetSize(); i++)
			{
				auto value = RecipeValue();
				Parse(value, source[i]);
				target.push_back(std::move(value));
			}
		}

		static SMLValue Build(RecipeValue& value)
		{
			auto result = SMLValue();
			switch (value.GetType())
			{
				case RecipeValueType::Empty:
					// Leave empty
					break;
				case RecipeValueType::Table:
					result = Build(value.AsTable());
					break;
				case RecipeValueType::List:
					result = Build(value.AsList());
					break;
				case RecipeValueType::String:
					result = SMLValue(value.AsString());
					break;
				case RecipeValueType::Integer:
					result = SMLValue(value.AsInteger());
					break;
				case RecipeValueType::Float:
					result = SMLValue(value.AsFloat());
					break;
				case RecipeValueType::Boolean:
					result = SMLValue(value.AsBoolean());
					break;
				default:
					throw std::runtime_error("Unknown value type.");
			}

			// TODO: result.comments() = value.GetComments();

			return result;
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
