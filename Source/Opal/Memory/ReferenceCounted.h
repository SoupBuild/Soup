// <copyright file="ReferenceCounted.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
#pragma once
#include "IReferenceCounted.h"

namespace Opal::Memory
{
	/// <summary>
	/// The shared implementation of a thread safe reference counted object
	/// </summary>
	export template<typename T>
	class ReferenceCounted : public T
	{
	protected:
		/// <summary>
		/// Initializes a new instance of the ReferenceCounted class
		/// Note: Protected to only allow inherited use
		/// </summary>
		ReferenceCounted() noexcept :
			_referenceCount(0)
		{
		}

		/// <summary>
		/// Initializes a copy instance of the ReferenceCounted class
		/// Note: Protected to only allow inherited use
		/// and forces the reference count back to zero.
		/// </summary>
		ReferenceCounted(const ReferenceCounted&) noexcept :
			_referenceCount(0)
		{
		}

		/// <summary>
		/// Assignment operator
		/// </summary>
		ReferenceCounted& operator=(const ReferenceCounted&) noexcept
		{
			return *this;
		}

		/// <summary>
		/// Finalizes an instance of ReferenceCounted class
		/// </summary>
		virtual ~ReferenceCounted() noexcept
		{
		}

	public:
		/// <summary>
		/// Adds a reference to the object
		/// </summary>
		void AddReference() const noexcept override final
		{
			_referenceCount++;
		}

		/// <summary>
		/// Releases a reference to the object and destructs if this is the last reference
		/// </summary>
		void ReleaseReference() const noexcept override final
		{
			auto currentCount = --_referenceCount;
			if (currentCount == 0)
			{
				delete this;
			}
			else if (currentCount < 0)
			{
				std::abort();
			}
		}

	private:
		mutable std::atomic<int64_t> _referenceCount;
	};
}
