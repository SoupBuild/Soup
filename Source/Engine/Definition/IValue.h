// <copyright file="IValue.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationResult.h"

namespace Soup::Build
{
	export enum class ValueType
	{
		Empty,
		Table,
		List,
		String,
		Integer,
		Float,
		Boolean,
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
		virtual OperationResult TryGetType(uint64_t& type) const noexcept = 0;
		virtual OperationResult TrySetType(uint64_t type) noexcept = 0;

		virtual OperationResult TryGetAsTable(IValueTable*& result) noexcept = 0;
		virtual OperationResult TryGetAsList(IValueList*& result) noexcept = 0;
		virtual OperationResult TryGetAsString(IValuePrimitive<const char*>*& result) noexcept = 0;
		virtual OperationResult TryGetAsInteger(IValuePrimitive<int64_t>*& result) noexcept = 0;
		virtual OperationResult TryGetAsFloat(IValuePrimitive<double>*& result) noexcept = 0;
		virtual OperationResult TryGetAsBoolean(IValuePrimitive<bool>*& result) noexcept = 0;
	};
}
