// <copyright file="RecipeToml.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Recipe.h"

namespace Soup::Build
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
				auto parser = cpptoml::parser(stream);
				auto root = parser.parse();
				if (!root->is_table())
					throw std::runtime_error("Recipe Toml file root must be a table.");

				// Load the entire root table
				auto table = ValueTable();
				Parse(ValueTableWrapper(table), *root);

				return Recipe(std::move(table));
			}
			catch(const cpptoml::parse_exception& ex)
			{
				throw std::runtime_error(std::string("Parsing the Recipe Toml failed: ") + ex.what());
			}
		}

		/// <summary>
		/// Save the recipe to the root file
		/// </summary>
		static void Serialize(Recipe& recipe, std::ostream& stream)
		{
			throw std::runtime_error("Toml serialize corrupts the stack!!");

			// Serialize the contents of the recipe
			auto root = Build(recipe.GetTable());

			// Write out the entire root table
			stream << *root;
		}

	private:
		static void Parse(ValueWrapper& target, cpptoml::base& source)
		{
			if (source.is_table())
			{
				Parse(target.EnsureTable(), *source.as_table());
			}
			else if (source.is_array())
			{
				Parse(target.EnsureList(), *source.as_array());
			}
			else if (source.is_value())
			{
				auto booleanValue = source.as<bool>();
				if (booleanValue != nullptr)
				{
					target.SetValueBoolean(booleanValue->get());
					return;
				}

				auto integerValue = source.as<int64_t>();
				if (integerValue != nullptr)
				{
					target.SetValueInteger(integerValue->get());
					return;
				}

				auto floatValue = source.as<double>();
				if (floatValue != nullptr)
				{
					target.SetValueFloat(floatValue->get());
					return;
				}

				auto stringValue = source.as<std::string>();
				if (stringValue != nullptr)
				{
					target.SetValueString(stringValue->get());
					return;
				}

				throw std::runtime_error("Unknown toml value type.");
			}
			else
			{
				throw std::runtime_error("Unknown toml type.");
			}
		}

		static void Parse(ValueTableWrapper& target, const cpptoml::table& source)
		{
			for (auto& item : source)
			{
				auto value = target.CreateValue(item.first);
				Parse(value, *item.second);
			}
		}

		static void Parse(ValueListWrapper& target, const cpptoml::array& source)
		{
			auto& values = source.get();
			target.Resize(values.size());
			for (size_t i = 0; i < values.size(); i++)
			{
				auto value = target.GetValueAt(i);
				Parse(value, *values[i]);
			}
		}

		static std::shared_ptr<cpptoml::base> Build(Value& value)
		{
			switch (value.GetType())
			{
				case ValueType::Empty:
					return nullptr;
				case ValueType::Table:
					return Build(value.AsTable());
				case ValueType::List:
					return Build(value.AsList());
				case ValueType::String:
					return cpptoml::make_value<std::string>(ValueWrapper(value).AsString().GetValue());
				case ValueType::Integer:
					return cpptoml::make_value<int64_t>(ValueWrapper(value).AsInteger().GetValue());
				case ValueType::Float:
					return cpptoml::make_value<double>(ValueWrapper(value).AsFloat().GetValue());
				case ValueType::Boolean:
					return cpptoml::make_value<bool>(ValueWrapper(value).AsBoolean().GetValue());
				default:
					throw std::runtime_error("Unknown value type.");
			}
		}

		static std::shared_ptr<cpptoml::base> Build(ValueTable& table)
		{
			auto result = cpptoml::make_table();

			for (auto& value : table.GetValues())
			{
				result->insert(value.first, Build(value.second));
			}

			return result;
		}

		static std::shared_ptr<cpptoml::base> Build(ValueList& list)
		{
			auto result = cpptoml::make_array();

			for (auto& value : list.GetValues())
			{
				result->get().push_back(Build(value));
			}

			return result;
		}
	};
}
