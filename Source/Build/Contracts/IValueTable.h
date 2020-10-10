// <copyright file="IValueTable.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ApiCallResult.h"
#include "IValue.h"
#include "IReadOnlyList.h"

namespace Soup::Build
{
	/// <summary>
	/// Build property bag interface that allows for read/write access to the active build state.
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	export class IValueTable
	{
	public:
		/// <summary>
		/// Check if a given property exists by name
		/// </summary>
		virtual ApiCallResult TryHasValue(const char* name, bool& result) const noexcept = 0;

		/// <summary>
		/// Get a given property by name
		/// </summary>
		virtual ApiCallResult TryGetValue(const char* name, IValue*& value) noexcept = 0;

		/// <summary>
		/// Create a given property by name
		/// </summary>
		virtual ApiCallResult TryCreateValue(const char* name, IValue*& value) noexcept = 0;

		/// <summary>
		/// Get the list of available value keys
		/// </summary>
		virtual const IReadOnlyList<const char*>& GetValueKeyList() const noexcept = 0;
	};
}
