// <copyright file="IReadOnlyList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationResult.h"

namespace Soup::Build
{
	/// <summary>
	/// The build list interface definition to allow build extensions read/write
	/// access to the a collection of values
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	template<typename T>
	class IReadOnlyList
	{
	public:
		/// <summary>
		/// Size access methods
		/// </summary>
		virtual uint64_t GetSize() const noexcept = 0;

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		virtual OperationResult TryGetValueAt(uint64_t index, T& result) noexcept = 0;
	};
}
