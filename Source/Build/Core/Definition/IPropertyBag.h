// <copyright file="IPropertyBag.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationResult.h"
#include "IPropertyValue.h"
#include "IPropertyList.h"

namespace Soup::Build
{
	/// <summary>
	/// Build property bag interface that allows for read/write access to the active build state.
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	class IPropertyBag
	{
	public:
		/// <summary>
		/// Property access methods
		/// </summary>
		virtual OperationResult TryHasPropertyValue(const char* name, bool& result) const noexcept = 0;
		virtual OperationResult TryGetPropertyValue(const char* name, IPropertyValue*& result) noexcept = 0;
		virtual OperationResult TryCreatePropertyValue(const char* name, IPropertyValue*& result) noexcept = 0;

		virtual OperationResult TryHasPropertyStringList(const char* name, bool& result) const noexcept = 0;
		virtual OperationResult TryGetPropertyStringList(const char* name, IPropertyList<const char*>*& result) noexcept = 0;
		virtual OperationResult TryCreatePropertyStringList(const char* name, IPropertyList<const char*>*& result) noexcept = 0;
	};
}
