// <copyright file="Value.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	export class Value;
	export using ValueList = std::vector<Value>;
	export using ValueTable = std::unordered_map<std::string, Value>;

	export enum class ValueType : uint64_t
	{
		Empty = 0,
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
		Value(ValueTable table);
		Value(ValueList list);
		Value(std::string value);
		Value(int64_t value);
		Value(double value);
		Value(bool value);

		/// <summary>
		/// Type checker methods
		/// </summary>
		ValueType GetType() const;

		/// <summary>
		/// Internal accessors
		/// </summary>
		std::string ToString();

		ValueTable& AsTable();
		ValueList& AsList();
		
		const ValueTable& AsTable() const;
		const ValueList& AsList() const;
		const std::string& AsString() const;
		int64_t AsInteger() const;
		double AsFloat() const;
		bool AsBoolean() const;

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const Value& rhs) const;

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const Value& rhs) const;

	private:
		std::variant<
			std::monostate,
			ValueTable,
			ValueList,
			std::string,
			int64_t,
			double,
			bool> _value;
	};
}
