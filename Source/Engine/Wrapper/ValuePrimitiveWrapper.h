// <copyright file="ValuePrimitiveWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
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
			T result;
			auto status = _value.TryGetValue(result);
			if (status != 0)
				throw std::runtime_error("TryGetValue Failed");

			return result;
		}

		/// <summary>
		/// Property setter methods.
		/// </summary>
		void SetValue(T value)
		{
			auto status = _value.TrySetValue(value);
			if (status != 0)
				throw std::runtime_error("TrySetValue Failed");
		}

	private:
		IValuePrimitive<T>& _value;
	};
}
