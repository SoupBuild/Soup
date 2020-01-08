// <copyright file="Reference.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
#pragma once
#include "IReferenceCounted.h"

namespace Opal::Memory
{
	/// <summary>
	/// A basic reference counted smart pointer.
	/// </summary>
	export template<typename T>
	class Reference
	{
	public:
		/// <summary>
		/// Default initializer with null pointer
		/// </summary>
		Reference() noexcept :
			_reference(nullptr)
		{
		}

		/// <summary>
		/// Special initializer that allows for creation with null pointer
		/// </summary>
		Reference(std::nullptr_t) noexcept :
			_reference(nullptr)
		{
		}

		/// <summary>
		/// Initializer that takes in a raw pointer.
		/// </summary>
		Reference(T* reference) noexcept :
			_reference(nullptr)
		{
			Assign(reference);
		}

		/// <summary>
		/// Initializes a copy instance of the Reference class
		/// </summary>
		Reference(const Reference<T>& reference) noexcept :
			_reference(nullptr)
		{
			Assign(reference._reference);
		}

		/// <summary>
		/// Assignment operator
		/// </summary>
		Reference<T>& operator=(const Reference<T>& reference) noexcept
		{
			Assign(reference._reference);
			return *this;
		}

		/// <summary>
		/// Finalize an instance of the Reference class
		/// </summary>
		~Reference() noexcept
		{
			Assign(nullptr);
		}

		/// <summary>
		/// Pointer operator overloads
		/// </summary>
		operator T*() noexcept
		{
			return _reference;
		}

		operator const T*() const noexcept
		{
			return _reference;
		}

		T* operator->() noexcept
		{
			return _reference;
		}
		
		const T* operator->() const noexcept
		{
			return _reference;
		}

		template<class U>
		bool operator==(const Reference<U>& rhs) noexcept
		{
			return GetRaw() == rhs.GetRaw();
		}

		/// <summary>
		/// Get the raw pointer value
		/// </summary>
		T* GetRaw() noexcept
		{
			return _reference;
		}

	private:
		/// <summary>
		/// Assign the reference to a new value
		/// </summary>
		inline void Assign(T* reference) noexcept
		{
			// Add an initial reference to the incoming pointer to ensure we never assign
			// a pointer that has been lost
			if (reference != nullptr)
			{
				reference->AddReference();
			}

			// Move out the old reference and replace it with the new
			auto previousReference = _reference;
			_reference = reference;

			// Release the old reference if it was valid
			if (previousReference != nullptr)
			{
				previousReference->ReleaseReference();
			}
		}

	private:
		T* _reference;
	};
}
