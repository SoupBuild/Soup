// <copyright file="IValue.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ApiCallResult.h"

namespace Soup::Build
{
	export enum class ValueType : uint64_t
	{
		Empty = 1,
		Table = 2,
		List = 3,
		String = 4,
		Integer = 5,
		Float = 6,
		Boolean = 7,
	};

	class IValueTable;
	class IValueList;
	template<typename T>
	class IValuePrimitive;

	/// <summary>
	/// The build property value interface definition to allow build extensions read/write
	/// access to a single property value.
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	export class IValue
	{
	public:
		/// <summary>
		/// Type checker methods
		/// </summary>
		virtual ValueType GetType() const noexcept = 0;
		virtual ApiCallResult TrySetType(ValueType type) noexcept = 0;

		virtual ApiCallResult TryGetAsTable(IValueTable*& result) noexcept = 0;
		virtual ApiCallResult TryGetAsList(IValueList*& result) noexcept = 0;
		virtual ApiCallResult TryGetAsString(IValuePrimitive<const char*>*& result) noexcept = 0;
		virtual ApiCallResult TryGetAsInteger(IValuePrimitive<int64_t>*& result) noexcept = 0;
		virtual ApiCallResult TryGetAsFloat(IValuePrimitive<double>*& result) noexcept = 0;
		virtual ApiCallResult TryGetAsBoolean(IValuePrimitive<bool>*& result) noexcept = 0;
	};
}
