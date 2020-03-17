// <copyright file="RecipeToml.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe.h"

namespace Soup
{
	/// <summary>
	/// The recipe Toml serialize manager
	/// </summary>
	export class RecipeToml
	{
	public:
		/// <summary>
		/// Load from stream
		/// </summary>
		static Recipe Deserialize(std::istream& stream)
		{
			try
			{
				// Read the contents of the recipe file
				auto root = toml::parse(stream, "Recipe.toml");
				if (!root.is_table())
					throw std::runtime_error("Recipe Toml file root must be a table.");

				// Load the entire root table
				auto table = Build::ValueTable();
				Parse(Build::ValueTableWrapper(table), root.as_table());

				return Recipe(std::move(table));
			}
			catch(const toml::exception& ex)
			{
				throw std::runtime_error(std::string("Parsing the Recipe Toml failed: ") + ex.what());
			}
		}

		/// <summary>
		/// Save the recipe to the root file
		/// </summary>
		static void Serialize(Recipe& recipe, std::ostream& stream)
		{
			// Serialize the contents of the recipe
			auto root = Build(recipe.GetTable());

			// Write out the entire root table
			stream << root;
		}

	private:
		static void Parse(Build::ValueWrapper& target, const toml::value& source)
		{
			switch (source.type())
			{
				case toml::value_t::empty:
					// Leave empty
					break;
				case toml::value_t::boolean:
					target.SetValueBoolean(source.as_boolean());
					break;
				case toml::value_t::integer:
					target.SetValueInteger(source.as_integer());
					break;
				case toml::value_t::floating:
					target.SetValueFloat(source.as_floating());
					break;
				case toml::value_t::string:
					target.SetValueString(source.as_string().str);
					break;
				case toml::value_t::offset_datetime:
				case toml::value_t::local_datetime:
				case toml::value_t::local_date:
				case toml::value_t::local_time:
					throw std::runtime_error("TODO: What to do with datetime?");
				case toml::value_t::array:
					Parse(target.EnsureList(), source.as_array());
					break;
				case toml::value_t::table:
					Parse(target.EnsureTable(), source.as_table());
					break;
				default:
					throw std::runtime_error("Unknown toml type.");
			}
		}

		static void Parse(Build::ValueTableWrapper& target, const toml::table& source)
		{
			for (auto& item : source)
			{
				auto value = target.CreateValue(item.first);
				Parse(value, item.second);
			}
		}

		static void Parse(Build::ValueListWrapper& target, const toml::array& source)
		{
			target.Resize(source.size());
			for (size_t i = 0; i < source.size(); i++)
			{
				auto value = target.GetValueAt(i);
				Parse(value, source[i]);
			}
		}
		
		static toml::value Build(Build::Value& value)
		{
			switch (value.GetType())
			{
				case Build::ValueType::Empty:
					return toml::value();
				case Build::ValueType::Table:
					return Build(value.AsTable());
				case Build::ValueType::List:
					return Build(value.AsList());
				case Build::ValueType::String:
					return toml::value(Build::ValueWrapper(value).AsString().GetValue());
				case Build::ValueType::Integer:
					return toml::value(Build::ValueWrapper(value).AsInteger().GetValue());
				case Build::ValueType::Float:
					return toml::value(Build::ValueWrapper(value).AsFloat().GetValue());
				case Build::ValueType::Boolean:
					return toml::value(Build::ValueWrapper(value).AsBoolean().GetValue());
				default:
					throw std::runtime_error("Unknown value type.");
			}
		}

		static toml::value Build(Build::ValueTable& table)
		{
			toml::table result = {};

			for (auto& value : table.GetValues())
			{
				result.emplace(value.first, Build(value.second));
			}

			return result;
		}

		static toml::value Build(Build::ValueList& list)
		{
			toml::array result = {};

			for (auto& value : list.GetValues())
			{
				result.push_back(Build(value));
			}

			return result;
		}
	};
}
