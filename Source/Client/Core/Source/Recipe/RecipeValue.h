// <copyright file="RecipeValue.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

export namespace Soup::Core
{
	class RecipeValue;
	using RecipeList = std::vector<RecipeValue>;
	using RecipeTable = std::unordered_map<std::string, RecipeValue>;

	enum class RecipeValueType
	{
		Table,
		List,
		String,
		Integer,
		Float,
		Boolean,
	};

	class RecipeValue
	{
	public:
		RecipeValue(RecipeTable value) :
			_value(std::move(value))
		{
		}

		RecipeValue(RecipeList value) :
			_value(std::move(value))
		{
		}

		RecipeValue(const char* value) :
			_value(value)
		{
		}

		RecipeValue(std::string value) :
			_value(std::move(value))
		{
		}

		RecipeValue(int64_t value) :
			_value(value)
		{
		}

		RecipeValue(double value) :
			_value(value)
		{
		}

		RecipeValue(bool value) :
			_value(value)
		{
		}

		bool IsString() const
		{
			return GetType() == RecipeValueType::String;
		}

		const std::string& AsString() const
		{
			if (IsString())
			{
				return std::get<std::string>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as string with incorrect type.");
			}
		}

		int64_t AsInteger() const
		{
			if (GetType() == RecipeValueType::Integer)
			{
				return std::get<int64_t>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as integer with incorrect type.");
			}
		}

		double AsFloat() const
		{
			if (GetType() == RecipeValueType::Float)
			{
				return std::get<double>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as float with incorrect type.");
			}
		}

		bool AsBoolean() const
		{
			if (GetType() == RecipeValueType::Boolean)
			{
				return std::get<bool>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as boolean with incorrect type.");
			}
		}

		bool IsTable() const
		{
			return GetType() == RecipeValueType::Table;
		}

		RecipeTable& AsTable()
		{
			if (IsTable())
			{
				return std::get<RecipeTable>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as RecipeTable with incorrect type.");
			}
		}

		const RecipeTable& AsTable() const
		{
			if (IsTable())
			{
				return std::get<RecipeTable>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as RecipeTable with incorrect type.");
			}
		}

		RecipeList& AsList()
		{
			if (GetType() == RecipeValueType::List)
			{
				return std::get<RecipeList>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as RecipeList with incorrect type.");
			}
		}

		const RecipeList& AsList() const
		{
			if (GetType() == RecipeValueType::List)
			{
				return std::get<RecipeList>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as RecipeList with incorrect type.");
			}
		}

		RecipeValueType GetType() const
		{
			switch (_value.index())
			{
				case 0:
					return RecipeValueType::Table;
				case 1:
					return RecipeValueType::List;
				case 2:
					return RecipeValueType::String;
				case 3:
					return RecipeValueType::Integer;
				case 4:
					return RecipeValueType::Float;
				case 5:
					return RecipeValueType::Boolean;
				default:
					throw std::runtime_error("Unknown recipe value type.");
			}
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const RecipeValue& rhs) const
		{
			if (GetType() == rhs.GetType())
			{
				switch (GetType())
				{
					case RecipeValueType::Table:
						return std::get<RecipeTable>(_value) == std::get<RecipeTable>(rhs._value);
					case RecipeValueType::List:
						return std::get<RecipeList>(_value) == std::get<RecipeList>(rhs._value);
					case RecipeValueType::String:
						return std::get<std::string>(_value) == std::get<std::string>(rhs._value);
					case RecipeValueType::Integer:
						return std::get<int64_t>(_value) == std::get<int64_t>(rhs._value);
					case RecipeValueType::Float:
						return std::get<double>(_value) == std::get<double>(rhs._value);
					case RecipeValueType::Boolean:
						return std::get<bool>(_value) == std::get<bool>(rhs._value);
					default:
						throw std::runtime_error("Unkown Recipe ValueType for comparison.");
				}
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const RecipeValue& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		std::variant<
			RecipeTable,
			RecipeList,
			std::string,
			int64_t,
			double,
			bool> _value;
	};
}
