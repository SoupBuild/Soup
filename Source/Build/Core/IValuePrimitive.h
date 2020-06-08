// <copyright file="IValue.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ApiCallResult.h"

namespace Soup::Build
{
	/// <summary>
	/// The build property value interface definition to allow build extensions read/write
	/// access to a single property value.
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	export template<typename T>
	class IValuePrimitive
	{
	public:
		/// <summary>
		/// Type checker methods
		/// </summary>
		virtual T GetValue() const noexcept = 0;
		virtual ApiCallResult TrySetValue(T value) noexcept = 0;
	};
}
