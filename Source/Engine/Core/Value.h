// <copyright file="Value.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	class ValueTable;
	class ValueList;

	/// <summary>
	/// Build State Extension interface
	/// </summary>
	class Value : public IValue
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
		OperationResult TryGetType(uint64_t& type) const noexcept override final;
		OperationResult TrySetType(uint64_t type) noexcept override final;

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		OperationResult TryGetAsTable(IValueTable*& result) noexcept override final;
		OperationResult TryGetAsList(IValueList*& result) noexcept override final;
		OperationResult TryGetAsString(IValuePrimitive<const char*>*& result) noexcept override final;
		OperationResult TryGetAsInteger(IValuePrimitive<int64_t>*& result) noexcept override final;
		OperationResult TryGetAsFloat(IValuePrimitive<double>*& result) noexcept override final;
		OperationResult TryGetAsBoolean(IValuePrimitive<bool>*& result) noexcept override final;

		/// <summary>
		/// Internal accessors
		/// </summary>
		ValueType GetType() const;
		std::string ToString();

		ValueTable& AsTable();
		ValueList& AsList();

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
