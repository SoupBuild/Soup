// <copyright file="Value.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Generate
{
	export class ValueTable;
	export class ValueList;

	/// <summary>
	/// Build State Extension interface
	/// </summary>
	export class Value : public IValue
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
		ValueType GetType() const noexcept override final;
		ApiCallResult TrySetType(ValueType type) noexcept override final;

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		ApiCallResult TryGetAsTable(IValueTable*& result) noexcept override final;
		ApiCallResult TryGetAsList(IValueList*& result) noexcept override final;
		ApiCallResult TryGetAsString(IValuePrimitive<const char*>*& result) noexcept override final;
		ApiCallResult TryGetAsInteger(IValuePrimitive<int64_t>*& result) noexcept override final;
		ApiCallResult TryGetAsFloat(IValuePrimitive<double>*& result) noexcept override final;
		ApiCallResult TryGetAsBoolean(IValuePrimitive<bool>*& result) noexcept override final;

		/// <summary>
		/// Internal accessors
		/// </summary>
		std::string ToString();

		ValueTable& AsTable();
		ValueList& AsList();

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
