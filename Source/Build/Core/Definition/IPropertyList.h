// <copyright file="IPropertyList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// The build property interface definition to allow build extensions read/write
	/// access to the active build state
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	template<typename T>
	class IPropertyList
	{
	public:
		/// <summary>
		/// Size access methods
		/// </summary>
		virtual uint64_t GetSize() const noexcept = 0;
		virtual OperationResult Resize(uint64_t size) noexcept = 0;

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		virtual OperationResult TryGetValueAt(uint64_t index, T& result) const noexcept = 0;

		/// <summary>
		/// Property setter methods.
		/// </summary>
		virtual OperationResult TrySetValueAt(uint64_t index, T value) noexcept = 0;
	};
}
