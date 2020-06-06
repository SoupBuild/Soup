// <copyright file="IReadOnlyList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IValue.h"
#include "ApiCallResult.h"

namespace Soup::Build
{
	/// <summary>
	/// The build list interface definition to allow build extensions read/write
	/// access to the a collection of typed values
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	export template<typename T>
	class IReadOnlyList
	{
	public:
		/// <summary>
		/// Size access methods
		/// </summary>
		virtual uint64_t GetSize() const noexcept = 0;

		/// <summary>
		/// Value accessor methods
		/// </summary>
		virtual ApiCallResult TryGetValueAt(uint64_t index, T& value) const noexcept = 0;
	};
}
