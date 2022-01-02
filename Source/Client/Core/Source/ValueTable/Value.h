// <copyright file="Value.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ValuePrimitive.h"

namespace Soup::Build::Runtime
{
	export class ValueTable;
	export class ValueList;

	export enum class ValueType : uint64_t
	{
		Table = 1,
		List = 2,
		String = 3,
		Integer = 4,
		Float = 5,
		Boolean = 6,
	};

	/// <summary>
	/// Build State Extension interface
	/// </summary>
	export class Value
	{
	public:
		/// <summary>
		/// Initializes a new instance of the Value class
		/// </summary>
		Value();
		Value(int64_t value);
		Value(double value);
		Value(bool value);
		Value(std::string value);
		Value(ValueList list);
		Value(ValueTable table);

		/// <summary>
		/// Type checker methods
		/// </summary>
		ValueType GetType() const noexcept;
		void SetType(ValueType type);

		/// <summary>
		/// Internal accessors
		/// </summary>
		std::string ToString();

		ValueTable& AsTable();
		ValueList& AsList();
		
		const ValueTable& AsTable() const;
		const ValueList& AsList() const;
		const ValuePrimitive<const char*>& AsString() const;
		const ValuePrimitive<int64_t>& AsInteger() const;
		const ValuePrimitive<double>& AsFloat() const;
		const ValuePrimitive<bool>& AsBoolean() const;

		ValueTable& EnsureTable();

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const Value& rhs) const;

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const Value& rhs) const;

	private:
		std::any _value;
	};
}
