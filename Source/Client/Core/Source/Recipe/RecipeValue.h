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
		Empty,
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
		RecipeValue() :
			_comments(),
			_value()
		{
		}

		RecipeValue(std::string value) :
			_comments(),
			_value(std::move(value))
		{
		}

		std::vector<std::string>& GetComments()
		{
			return _comments;
		}

		bool IsString() const
		{
			return _value.type() == typeid(std::string);
		}

		const std::string& AsString() const
		{
			if (IsString())
			{
				return std::any_cast<const std::string&>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as string with incorrect type.");
			}
		}

		void SetValueString(std::string value)
		{
			_value = std::move(value);
		}

		int64_t AsInteger() const
		{
			if (_value.type() == typeid(int64_t))
			{
				return std::any_cast<int64_t>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as integer with incorrect type.");
			}
		}

		void SetValueInteger(int64_t value)
		{
			_value = value;
		}

		double AsFloat() const
		{
			if (_value.type() == typeid(double))
			{
				return std::any_cast<double>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as float with incorrect type.");
			}
		}

		void SetValueFloat(double value)
		{
			_value = value;
		}

		bool AsBoolean() const
		{
			if (_value.type() == typeid(bool))
			{
				return std::any_cast<bool>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as boolean with incorrect type.");
			}
		}

		void SetValueBoolean(bool value)
		{
			_value = value;
		}

		bool IsTable() const
		{
			return _value.type() == typeid(RecipeTable);
		}

		RecipeTable& AsTable()
		{
			if (IsTable())
			{
				return std::any_cast<RecipeTable&>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as RecipeTable with incorrect type.");
			}
		}

		const RecipeTable& AsTable() const
		{
			if (_value.type() == typeid(RecipeTable))
			{
				return std::any_cast<const RecipeTable&>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as RecipeTable with incorrect type.");
			}
		}

		void SetValueTable(RecipeTable value)
		{
			_value = std::move(value);
		}

		RecipeList& AsList()
		{
			if (_value.type() == typeid(RecipeList))
			{
				return std::any_cast<RecipeList&>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as RecipeList with incorrect type.");
			}
		}

		const RecipeList& AsList() const
		{
			if (_value.type() == typeid(RecipeList))
			{
				return std::any_cast<const RecipeList&>(_value);
			}
			else
			{
				// Wrong type
				throw std::runtime_error("Attempt to access value as RecipeList with incorrect type.");
			}
		}

		void SetValueList(RecipeList value)
		{
			_value = std::move(value);
		}

		RecipeValueType GetType() const
		{
			if (_value.has_value())
			{
				auto& valueType = _value.type();
				if (valueType == typeid(RecipeTable))
					return RecipeValueType::Table;
				else if (valueType == typeid(RecipeList))
					return RecipeValueType::List;
				else if (valueType == typeid(std::string))
					return RecipeValueType::String;
				else if (valueType == typeid(int64_t))
					return RecipeValueType::Integer;
				else if (valueType == typeid(double))
					return RecipeValueType::Float;
				else if (valueType == typeid(bool))
					return RecipeValueType::Boolean;
				else
					throw std::runtime_error("Unknown recipe value type.");
			}
			else
			{
				return RecipeValueType::Empty;
			}
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const RecipeValue& rhs) const
		{
			if (_comments != rhs._comments)
			{
				return false;
			}

			if (GetType() == rhs.GetType())
			{
				switch (GetType())
				{
					case RecipeValueType::Empty:
						return true;
					case RecipeValueType::Table:
						return std::any_cast<const RecipeTable&>(_value) == std::any_cast<const RecipeTable&>(rhs._value);
					case RecipeValueType::List:
						return std::any_cast<const RecipeList&>(_value) == std::any_cast<const RecipeList&>(rhs._value);
					case RecipeValueType::String:
						return std::any_cast<const std::string&>(_value) == std::any_cast<const std::string&>(rhs._value);
					case RecipeValueType::Integer:
						return std::any_cast<int64_t>(_value) == std::any_cast<int64_t>(rhs._value);
					case RecipeValueType::Float:
						return std::any_cast<double>(_value) == std::any_cast<double>(rhs._value);
					case RecipeValueType::Boolean:
						return std::any_cast<bool>(_value) == std::any_cast<bool>(rhs._value);
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
		std::vector<std::string> _comments;
		std::any _value;
	};

	using RecipeArray = std::vector<RecipeValue>;
	using RecipeTable = std::unordered_map<std::string, RecipeValue>;
}
