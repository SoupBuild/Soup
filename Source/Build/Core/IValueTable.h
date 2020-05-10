// <copyright file="ITable.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationResult.h"
#include "IValue.h"

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
		virtual OperationResult TryCheckHasValue(const char* name, bool& result) const noexcept = 0;

		/// <summary>
		/// Get a given property by name
		/// </summary>
		virtual OperationResult TryGetValue(const char* name, IValue*& value) noexcept = 0;

		/// <summary>
		/// Create a given property by name
		/// </summary>
		virtual OperationResult TryCreateValue(const char* name, IValue*& value) noexcept = 0;
	};
}
