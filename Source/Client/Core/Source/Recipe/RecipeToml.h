// <copyright file="RecipeToml.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe.h"

namespace Soup::Core
{
	using TomlValue = toml::basic_value<toml::preserve_comments>;
	using TomlArray = std::vector<TomlValue>;
	using TomlTable = std::unordered_map<toml::key, TomlValue>;

	/// <summary>
	/// The recipe Toml serialize manager
	/// </summary>
	export class RecipeToml
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
				auto root = toml::parse<toml::preserve_comments>(stream, recipeFile.ToString());
				if (!root.is_table())
					throw std::runtime_error("Recipe Toml file root must be a table.");

				// Load the entire root table
				auto table = RecipeTable();
				Parse(table, root.as_table());

				return table;
			}
			catch(const toml::exception& ex)
			{
				throw std::runtime_error(std::string("Parsing the Recipe Toml failed: ") + ex.what());
			}
		}

		/// <summary>
		/// Save the recipe to the root file
		/// </summary>
		static void Serialize(RecipeTable& recipeTable, std::ostream& stream)
		{
			// Serialize the contents of the recipe
			auto root = Build(recipeTable);

			// Write out the entire root table
			stream << root;
		}

	private:
		static void Parse(RecipeValue& target, const TomlValue& source)
		{
			// Copy over all of the comments
			target.GetComments().insert(
				target.GetComments().end(),
				source.comments().begin(),
				source.comments().end());

			switch (source.type())
			{
				case toml::value_t::empty:
				{
					// Leave empty
					break;
				}
				case toml::value_t::boolean:
				{
					target.SetValueBoolean(source.as_boolean());
					break;
				}
				case toml::value_t::integer:
				{
					target.SetValueInteger(source.as_integer());
					break;
				}
				case toml::value_t::floating:
				{
					target.SetValueFloat(source.as_floating());
					break;
				}
				case toml::value_t::string:
				{
					target.SetValueString(source.as_string().str);
					break;
				}
				case toml::value_t::offset_datetime:
				case toml::value_t::local_datetime:
				case toml::value_t::local_date:
				case toml::value_t::local_time:
				{
					throw std::runtime_error("TODO: What to do with datetime?");
				}
				case toml::value_t::array:
				{
					auto valueList = RecipeList();
					Parse(valueList, source.as_array());
					target.SetValueList(std::move(valueList));
					break;
				}
				case toml::value_t::table:
				{
					auto valueTable = RecipeTable();
					Parse(valueTable, source.as_table());
					target.SetValueTable(std::move(valueTable));
					break;
				}
				default:
				{
					throw std::runtime_error("Unknown toml type.");
				}
			}
		}

		static void Parse(RecipeTable& target, const TomlTable& source)
		{
			for (auto& item : source)
			{
				auto value = RecipeValue();
				Parse(value, item.second);
				target.emplace(item.first, std::move(value));
			}
		}

		static void Parse(RecipeList& target, const TomlArray& source)
		{
			target.reserve(source.size());
			for (size_t i = 0; i < source.size(); i++)
			{
				auto value = RecipeValue();
				Parse(value, source[i]);
				target.push_back(std::move(value));
			}
		}

		static TomlValue Build(RecipeValue& value)
		{
			auto result = TomlValue();
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
					result = TomlValue(value.AsString());
					break;
				case RecipeValueType::Integer:
					result = TomlValue(value.AsInteger());
					break;
				case RecipeValueType::Float:
					result = TomlValue(value.AsFloat());
					break;
				case RecipeValueType::Boolean:
					result = TomlValue(value.AsBoolean());
					break;
				default:
					throw std::runtime_error("Unknown value type.");
			}

			result.comments() = value.GetComments();

			return result;
		}

		static TomlValue Build(RecipeTable& table)
		{
			TomlTable result = {};

			for (auto& value : table)
			{
				result.emplace(value.first, Build(value.second));
			}

			return result;
		}

		static TomlValue Build(RecipeList& list)
		{
			TomlArray result = {};

			for (auto& value : list)
			{
				result.push_back(Build(value));
			}

			return result;
		}
	};
}
