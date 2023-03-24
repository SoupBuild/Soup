// <copyright file="Value.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

#ifdef SOUP_BUILD
export
#endif
namespace Soup::Core
{
	class Value;
	using ValueList = std::vector<Value>;
	using ValueTable = std::map<std::string, Value>;

	enum class ValueType : uint64_t
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
	class Value
	{
	public:
		/// <summary>
		/// Initializes a new instance of the Value class
		/// </summary>
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

		bool IsTable() const;
		bool IsList() const;
		bool IsString() const;
		bool IsInteger() const;
		bool IsFloat() const;
		bool IsBoolean() const;

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
			ValueTable,
			ValueList,
			std::string,
			int64_t,
			double,
			bool> _value;
	};
}
