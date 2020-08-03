// <copyright file="IList.h" company="Soup">
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
	class IList
	{
	public:
		/// <summary>
		/// Size access method
		/// </summary>
		virtual uint64_t GetSize() const noexcept = 0;

		/// <summary>
		/// Value getter method
		/// </summary>
		virtual ApiCallResult TryGetValueAt(uint64_t index, T& value) const noexcept = 0;

		/// <summary>
		/// Value setter method
		/// </summary>
		virtual ApiCallResult TrySetValueAt(uint64_t index, T value) noexcept = 0;

		/// <summary>
		/// Resize to the requested size
		/// </summary>
		virtual ApiCallResult TryResize(uint64_t size) noexcept = 0;
	};
}
