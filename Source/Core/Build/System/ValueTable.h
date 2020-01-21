// <copyright file="BuildPropertyBag.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Value.h"
#include "ValueList.h"

namespace Soup::Build
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	class ValueTable : public IValueTable
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
		/// Property access methods
		/// </summary>
		OperationResult TryCheckHasValue(const char* name, bool& result) const noexcept override final
		{
			try
			{
				result = false;
				result = _values.contains(name);
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		OperationResult TryGetValue(const char* name, IValue*& result) noexcept override final
		{
			try
			{
				result = nullptr;
				auto findResult = _values.find(name);
				if (findResult != _values.end())
				{
					result = &findResult->second;
					return 0;
				}
				else
				{
					// The property does not exists
					return -3;
				}
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		OperationResult TryCreateValue(const char* name, IValue*& result) noexcept override final
		{
			try
			{
				result = nullptr;
				auto insertResult = _values.emplace(name, Value());
				if (insertResult.second)
				{
					result = &insertResult.first->second;
					return 0;
				}
				else
				{
					// The property already exists
					return -4;
				}
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		/// <summary>
		/// Internal access to the state
		/// </summary>
		std::map<std::string, Value>& GetValues()
		{
			return _values;
		}


		/// <summary>
		/// Internal Accessor
		/// </summary>
		std::string ToString() const
		{
			auto stream = std::stringstream();

			stream << "{";
			stream << "}";

			return stream.str();
		}

		void Log()
		{
			auto stream = std::stringstream();
			stream << "BuildState\n";
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
