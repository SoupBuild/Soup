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
				auto table = Build::ValueTable();
				ParseJson(Build::ValueTableWrapper(table), jsonRoot.object_items());

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
		static void ParseJson(Build::ValueWrapper& value, const json11::Json& item)
		{
			switch (item.type())
			{
				case json11::Json::Type::NUL:
					// Leave empty
					break;
				case json11::Json::Type::NUMBER:
					value.SetValueFloat(item.number_value());
					break;
				case json11::Json::Type::BOOL:
					value.SetValueBoolean(item.bool_value());
					break;
				case json11::Json::Type::STRING:
					value.SetValueString(item.string_value());
					break;
				case json11::Json::Type::ARRAY:
					ParseJson(value.EnsureList(), item.array_items());
					break;
				case json11::Json::Type::OBJECT:
					ParseJson(value.EnsureTable(), item.object_items());
					break;
				default:
					throw std::runtime_error("Unknown json type.");
			}
		}

		static void ParseJson(Build::ValueTableWrapper& table, const json11::Json::object& items)
		{
			for (auto& item : items)
			{
				auto value = table.CreateValue(item.first);
				ParseJson(value, item.second);
			}
		}

		static void ParseJson(Build::ValueListWrapper& list, const json11::Json::array& items)
		{
			list.Resize(items.size());
			for (size_t i = 0; i < items.size(); i++)
			{
				auto value = list.GetValueAt(i);
				ParseJson(value, items[i]);
			}
		}

		static json11::Json BuildJson(Build::Value& value)
		{
			switch (value.GetType())
			{
				case Build::ValueType::Empty:
					return json11::Json();
				case Build::ValueType::Table:
					return BuildJson(value.AsTable());
				case Build::ValueType::List:
					return BuildJson(value.AsList());
				case Build::ValueType::String:
					return json11::Json(
						Build::ValueWrapper(value).AsString().GetValue());
				case Build::ValueType::Integer:
					return json11::Json(
						static_cast<int>(Build::ValueWrapper(value).AsInteger().GetValue()));
				case Build::ValueType::Float:
					return json11::Json(
						Build::ValueWrapper(value).AsFloat().GetValue());
				case Build::ValueType::Boolean:
					return json11::Json(
						Build::ValueWrapper(value).AsBoolean().GetValue());
				default:
					throw std::runtime_error("Unknown value type.");
			}
		}

		static json11::Json BuildJson(Build::ValueTable& table)
		{
			json11::Json::object result = {};

			for (auto& value : table.GetValues())
			{
				result.emplace(value.first, BuildJson(value.second));
			}

			return result;
		}

		static json11::Json BuildJson(Build::ValueList& list)
		{
			json11::Json::array result = {};

			for (auto& value : list.GetValues())
			{
				result.push_back(BuildJson(value));
			}

			return result;
		}
	};
}
