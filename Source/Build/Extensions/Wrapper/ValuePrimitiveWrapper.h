// <copyright file="ValuePrimitiveWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Extensions
{
	/// <summary>
	/// Build State property list implementation, string specialization
	/// </summary>
	template<typename T>
	class ValuePrimitiveWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the ValuePrimitiveWrapper class
		/// </summary>
		ValuePrimitiveWrapper(IValuePrimitive<T>& value) :
			_value(value)
		{
		}

		/// <summary>
		/// Size access methods
		/// </summary>
		T GetValue() const
		{
			return _value.GetValue();
		}

		/// <summary>
		/// Property setter methods.
		/// </summary>
		void SetValue(T value)
		{
			auto status = _value.TrySetValue(value);
			if (status != ApiCallResult::Success)
				throw std::runtime_error("TrySetValue Failed");
		}

	private:
		IValuePrimitive<T>& _value;
	};
}
