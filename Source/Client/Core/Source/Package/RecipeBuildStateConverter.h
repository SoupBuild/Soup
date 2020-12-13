// <copyright file="RecipeBuildStateConverter.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// The recipe build state converter that converts between a recipe table and a value table
	/// </summary>
	export class RecipeBuildStateConverter
	{
	public:
		/// <summary>
		/// Convert the root recipe table to a build Value Table entry
		/// </summary>
		static Runtime::ValueTable ConvertToRootBuildState(const RecipeTable& table)
		{
			// Convert teh root table
			auto recipeState = ConvertToBuildState(table);
			
			// Initialize the Recipe state
			auto state = Runtime::ValueTable();
			state.SetValue("Recipe", Runtime::Value(std::move(recipeState)));
			return state;
		}

		/// <summary>
		/// Convert the recipe internal representation to initial build state
		/// </summary>
		static Runtime::ValueTable ConvertToBuildState(const RecipeTable& table)
		{
			auto result = Runtime::ValueTable();
			for (auto& value : table)
			{
				auto buildValue = ConvertToBuildState(value.second);
				result.SetValue(value.first, std::move(buildValue));
			}

			return result;
		}

	private:
		/// <summary>
		/// Convert the recipe internal representation to initial build state
		/// </summary>
		static Runtime::ValueList ConvertToBuildState(const RecipeList& list)
		{
			auto result = Runtime::ValueList();
			for (auto& value : list)
			{
				auto buildValue = ConvertToBuildState(value);
				result.GetValues().push_back(std::move(buildValue));
			}

			return result;
		}

		/// <summary>
		/// Convert the recipe internal representation to initial build state
		/// </summary>
		static Runtime::Value ConvertToBuildState(const RecipeValue& value)
		{
			switch (value.GetType())
			{
				case RecipeValueType::Empty:
					return Runtime::Value();
				case RecipeValueType::Table:
					return Runtime::Value(ConvertToBuildState(value.AsTable()));
				case RecipeValueType::List:
					return Runtime::Value(ConvertToBuildState(value.AsList()));
				case RecipeValueType::String:
					return Runtime::Value(value.AsString());
				case RecipeValueType::Integer:
					return Runtime::Value(value.AsInteger());
				case RecipeValueType::Float:
					return Runtime::Value(value.AsFloat());
				case RecipeValueType::Boolean:
					return Runtime::Value(value.AsBoolean());
				default:
					throw std::runtime_error("Unknown value type.");
			}
		}
	};
}
