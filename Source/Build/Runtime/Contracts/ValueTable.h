// <copyright file="BuildPropertyBag.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Value.h"
#include "ValueList.h"

namespace Soup::Build::Runtime
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	export class ValueTable : public IValueTable
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildPropertyBag class
		/// </summary>
		ValueTable() :
			_keyList(),
			_values()
		{
		}

		/// <summary>
		/// Property access methods
		/// </summary>
		ApiCallResult TryHasValue(const char* name, bool& result) const noexcept override final
		{
			try
			{
				result = HasValue(name);
				return ApiCallResult::Success;
			}
			catch (...)
			{
				// Unknown error
				return ApiCallResult::Error;
			}
		}

		ApiCallResult TryGetValue(const char* name, IValue*& result) noexcept override final
		{
			try
			{
				result = nullptr;
				auto findResult = _values.find(name);
				if (findResult != _values.end())
				{
					result = &findResult->second;
					return ApiCallResult::Success;
				}
				else
				{
					// The property does not exists
					return ApiCallResult::Error;
				}
			}
			catch (...)
			{
				// Unknown error
				return ApiCallResult::Error;
			}
		}

		ApiCallResult TryCreateValue(const char* name, IValue*& result) noexcept override final
		{
			try
			{
				result = nullptr;
				result = &SetValue(name, Value());
				return ApiCallResult::Success;
			}
			catch (...)
			{
				// Unknown error
				return ApiCallResult::Error;
			}
		}

		const IReadOnlyList<const char*>& GetValueKeyList() const noexcept override final
		{
			return _keyList;
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

		Value& SetValue(std::string_view name, Value value)
		{
			auto insertResult = _values.emplace(name, std::move(value));
			if (insertResult.second)
			{
				// Keep the key list in sync
				auto& keys = _keyList.GetValues();
				keys.clear();
				keys.reserve(_values.size());
				for(auto& value : _values)
					keys.push_back(value.first);

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
		Utilities::ReadOnlyStringList _keyList;
		std::map<std::string, Value> _values;
	};
}
