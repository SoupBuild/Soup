// <copyright file="ValueWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	export class ValueTableWrapper;
	export class ValueListWrapper;
	export template<typename T>
	class ValuePrimitiveWrapper;

	/// <summary>
	/// Build Property Value Extension used to make the ITable interface more usable
	/// </summary>
	export class ValueWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the TableWrapper class
		/// </summary>
		ValueWrapper(IValue& value);

		/// <summary>
		/// Property access methods
		/// </summary>
		ValueType GetType() const;
		void SetType(ValueType type);

		ValueTableWrapper AsTable();
		ValueListWrapper AsList();
		ValuePrimitiveWrapper<const char*> AsString();
		ValuePrimitiveWrapper<int64_t> AsInteger();
		ValuePrimitiveWrapper<double> AsFloat();
		ValuePrimitiveWrapper<bool> AsBoolean();

		/// <summary>
		/// Helper methods that will auto convert the internal type if empty
		/// and then return the requested accessor
		/// </summary>
		ValueTableWrapper EnsureTable();
		ValueListWrapper EnsureList();
		ValuePrimitiveWrapper<const char*> EnsureString();
		ValuePrimitiveWrapper<int64_t> EnsureInteger();
		ValuePrimitiveWrapper<double> EnsureFloat();
		ValuePrimitiveWrapper<bool> EnsureBoolean();

		/// <summary>
		/// Helper methods that will set the value to the requested value/type if 
		/// the type matches or if the value is currently empty
		/// </summary>
		// TODO: MSVC Bug does not allow same name
		void SetValue(ValueWrapper value);
		void SetValueStringList(const std::vector<std::string>& value);
		void SetValuePathList(const std::vector<Path>& value);
		void SetValueString(std::string_view value);
		void SetValueInteger(int64_t value);
		void SetValueFloat(double value);
		void SetValueBoolean(bool value);

		IValue& GetReference();

	private:
		IValue& _value;
	};
}
