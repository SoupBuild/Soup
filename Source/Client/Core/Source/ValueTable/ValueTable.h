// <copyright file="ValueTable.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Value.h"
#include "ValueList.h"

namespace Soup::Core
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	export class ValueTable
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildPropertyBag class
		/// </summary>
		ValueTable() :
			_values()
		{
		}

		/// <summary>
		/// Initializes a new instance of the BuildPropertyBag class
		/// </summary>
		ValueTable(std::map<std::string, Value> values) :
			_values(std::move(values))
		{
		}

		Value& GetValue(const char* name)
		{
			auto findResult = _values.find(name);
			if (findResult != _values.end())
			{
				return findResult->second;
			}
			else
			{
				// The property does not exists
				throw std::runtime_error("Value does not exist.");
			}
		}

		/// <summary>
		/// Helper methods to make our lives easier
		/// </summary>
		Value& EnsureValue(std::string_view name)
		{
			auto findResult = _values.find(name.data());
			if (findResult != _values.end())
			{
				return findResult->second;
			}
			else
			{
				return SetValue(name, Value());
			}
		}

		/// <summary>
		/// Internal access to the state
		/// </summary>
		bool HasValue(std::string_view name) const
		{
			return _values.contains(name.data());
		}

		Value& GetValue(std::string_view name)
		{
			auto findResult = _values.find(name.data());
			if (findResult != _values.end())
			{
				return findResult->second;
			}
			else
			{
				// The property does not exists
				throw std::runtime_error(std::string("ValueTable::GetValue value (") + std::string(name) + ") does not exist");
			}
		}

		const Value& GetValue(std::string_view name) const
		{
			auto findResult = _values.find(name.data());
			if (findResult != _values.end())
			{
				return findResult->second;
			}
			else
			{
				// The property does not exists
				throw std::runtime_error("ValueTable::GetValue value does not exist");
			}
		}

		Value& SetValue(std::string_view name, Value value)
		{
			auto insertResult = _values.emplace(name, std::move(value));
			if (insertResult.second)
			{
				return insertResult.first->second;
			}
			else
			{
				throw std::runtime_error("Failed to insert a value.");
			}
		}

		std::map<std::string, Value>& GetValues()
		{
			return _values;
		}

		const std::map<std::string, Value>& GetValues() const
		{
			return _values;
		}

		/// <summary>
		/// Internal Accessor
		/// </summary>
		std::string ToString()
		{
			auto stream = std::stringstream();

			stream << "{ ";

			bool isFirst = true;
			for (auto& value : _values)
			{
				if (!isFirst)
				{
					stream << ", ";
				}

				stream << value.first << "=\"" << value.second.ToString() << "\"";
				isFirst = false;
			}

			stream << " }";

			return stream.str();
		}

		void Log()
		{
			auto stream = std::stringstream();
			for (auto& value : _values)
			{
				stream << "\t" << value.first << "=\"" << value.second.ToString() << "\"\n";
			}

			Log::Diag(stream.str());
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const ValueTable& rhs) const
		{
			return _values == rhs._values;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const ValueTable& rhs) const
		{
			return !(*this == rhs);
		}

	private:
		std::map<std::string, Value> _values;
	};
}
