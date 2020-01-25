// <copyright file="IList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IValue.h"
#include "OperationResult.h"

namespace Soup::Build
{
	/// <summary>
	/// The build list interface definition to allow build extensions read/write
	/// access to the a collection of values
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	class IValueList
	{
	public:
		/// <summary>
		/// Size access methods
		/// </summary>
		virtual uint64_t GetSize() const noexcept = 0;
		virtual OperationResult Resize(uint64_t size) noexcept = 0;

		/// <summary>
		/// Value accessor methods
		/// </summary>
		virtual OperationResult TryGetValueAt(uint64_t index, IValue*& result) noexcept = 0;
	};
}
