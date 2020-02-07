// <copyright file="ValueTableWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ValueWrapper.h"
#include "ValuePrimitiveWrapper.h"

namespace Soup::Build
{
	/// <summary>
	/// Build Property Table Extension used to make the IValueTable interface more usable
	/// </summary>
	export class ValueTableWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the TableWrapper class
		/// </summary>
		ValueTableWrapper(IValueTable& value) :
			_value(value)
		{
		}

		/// <summary>
		/// Property access methods
		/// </summary>
		bool HasValue(std::string_view name) const
		{
			bool result = false;
			auto status = _value.TryCheckHasValue(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryCheckHasValue Failed");

			return result;
		}

		ValueWrapper GetValue(std::string_view name)
		{
			IValue* result = nullptr;
			auto status = _value.TryGetValue(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryGetValue(\"" + std::string(name) + "\") Failed");
			if (result == nullptr)
				throw std::runtime_error("TryGetValue(\"" + std::string(name) + "\") has no value.");

			return ValueWrapper(*result);
		}

		ValueWrapper CreateValue(std::string_view name)
		{
			IValue* result = nullptr;
			auto status = _value.TryCreateValue(name.data(), result);
			if (status != 0)
				throw std::runtime_error("TryCreateValue(\"" + std::string(name) + "\") Failed");
			if (result == nullptr)
				throw std::runtime_error("TryCreateValue(\"" + std::string(name) + "\") has no value.");

			return ValueWrapper(*result);
		}

		/// <summary>
		/// Helper methods to make our lives easier
		/// </summary>
		ValueWrapper EnsureValue(std::string_view name)
		{
			if (HasValue(name))
			{
				return GetValue(name);
			}
			else
			{
				return CreateValue(name);
			}
		}

	private:
		IValueTable& _value;
	};
}
