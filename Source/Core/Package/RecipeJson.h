// <copyright file="RecipeJson.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe.h"

namespace Soup
{
	/// <summary>
	/// The recipe json serialize manager
	/// </summary>
	export class RecipeJson
	{
	public:
		/// <summary>
		/// Load from stream
		/// </summary>
		static Recipe Deserialize(std::istream& stream)
		{
			// Read the entire file into a string
			std::string content(
				(std::istreambuf_iterator<char>(stream)),
				std::istreambuf_iterator<char>());

			// Read the contents of the recipe file
			std::string error = "";
			auto jsonRoot = json11::Json::parse(content, error);
			if (jsonRoot.is_null())
			{
				auto message = "Failed to parse the recipe json: " + error;
				throw std::runtime_error(std::move(message));
			}
			else
			{
				if (!jsonRoot.is_object())
					throw std::runtime_error("Root json was not an object.");

				// Load the entire json blob into a root table
				auto table = RecipeTable();
				ParseJson(table, jsonRoot.object_items());

				return Recipe(std::move(table));
			}
		}

		/// <summary>
		/// Save the recipe to the root file
		/// </summary>
		static void Serialize(Recipe& recipe, std::ostream& stream)
		{
			// Serialize the contents of the recipe
			json11::Json json = BuildJson(recipe.GetTable());

			stream << json.dump();
		}

	private:
		static void ParseJson(RecipeValue& value, const json11::Json& item)
		{
			switch (item.type())
			{
				case json11::Json::Type::NUL:
				{
					// Leave empty
					break;
				}
				case json11::Json::Type::NUMBER:
				{
					value.SetValueFloat(item.number_value());
					break;
				}
				case json11::Json::Type::BOOL:
				{
					value.SetValueBoolean(item.bool_value());
					break;
				}
				case json11::Json::Type::STRING:
				{
					value.SetValueString(item.string_value());
					break;
				}
				case json11::Json::Type::ARRAY:
				{
					auto list = RecipeList();
					ParseJson(list, item.array_items());
					value.SetValueList(std::move(list));
					break;
				}
				case json11::Json::Type::OBJECT:
				{
					auto table = RecipeTable();
					ParseJson(table, item.object_items());
					value.SetValueTable(std::move(table));
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown json type.");
				}
			}
		}

		static void ParseJson(RecipeTable& table, const json11::Json::object& items)
		{
			for (auto& item : items)
			{
				auto value = RecipeValue();
				ParseJson(value, item.second);
				table.emplace(item.first, std::move(value));
			}
		}

		static void ParseJson(RecipeList& list, const json11::Json::array& items)
		{
			list.reserve(items.size());
			for (size_t i = 0; i < items.size(); i++)
			{
				auto value = RecipeValue();
				ParseJson(value, items[i]);
				list.push_back(std::move(value));
			}
		}

		static json11::Json BuildJson(RecipeValue& value)
		{
			switch (value.GetType())
			{
				case RecipeValueType::Empty:
					return json11::Json();
				case Build::ValueType::Table:
					return BuildJson(value.AsTable());
				case RecipeValueType::List:
					return BuildJson(value.AsList());
				case RecipeValueType::String:
					return json11::Json(value.AsString());
				case RecipeValueType::Integer:
					return json11::Json(
						static_cast<int>(value.AsInteger()));
				case RecipeValueType::Float:
					return json11::Json(value.AsFloat());
				case RecipeValueType::Boolean:
					return json11::Json(value.AsBoolean());
				default:
					throw std::runtime_error("Unknown value type.");
			}
		}

		static json11::Json BuildJson(RecipeTable& table)
		{
			json11::Json::object result = {};

			for (auto& value : table)
			{
				result.emplace(value.first, BuildJson(value.second));
			}

			return result;
		}

		static json11::Json BuildJson(RecipeList& list)
		{
			json11::Json::array result = {};

			for (auto& value : list)
			{
				result.push_back(BuildJson(value));
			}

			return result;
		}
	};
}
