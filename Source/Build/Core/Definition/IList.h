// <copyright file="IList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "IReadOnlyList.h"
#include "OperationResult.h"

namespace Soup::Build
{
	/// <summary>
	/// The build list interface definition to allow build extensions read/write
	/// access to the a collection of values
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	template<typename T>
	class IList : public IReadOnlyList<T>
	{
	public:
		/// <summary>
		/// Size access methods
		/// </summary>
		virtual OperationResult Resize(uint64_t size) noexcept = 0;

		/// <summary>
		/// Property setter methods.
		/// </summary>
		virtual OperationResult TrySetValueAt(uint64_t index, T value) noexcept = 0;
	};
}
