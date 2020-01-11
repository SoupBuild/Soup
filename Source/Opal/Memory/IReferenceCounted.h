// <copyright file="IReferenceCounted.hs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
#pragma once

namespace Opal::Memory
{
	/// <summary>
	/// A container for a path string
	/// </summary>
	export class IReferenceCounted
	{
	public:
		/// <summary>
		/// Virtual destructor to ensure proper cleanup when deleting this as a base
		/// Note: This could be a private destructor to force delete through the release reference
		/// but that would be very restrictive on other code bases that wish to use this in some scenarios.
		/// </summary>
		virtual ~IReferenceCounted() noexcept
		{
		}

		/// <summary>
		/// Adds a reference to the object
		/// Note: These methods are marked as const to allow for constant references
		/// This will require the implementation to use a mutable reference count.
		/// </summary>
		virtual void AddReference() const noexcept = 0;

		/// <summary>
		/// Releases a reference to the object and destructs if this is the last reference
		/// </summary>
		virtual void ReleaseReference() const noexcept = 0;
	};
}
