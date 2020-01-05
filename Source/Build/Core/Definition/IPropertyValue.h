// <copyright file="IPropertyValue.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// The build property value interface definition to allow build extensions read/write
	/// access to a single property value.
	/// Note: Has strict ABI requirements to prevent version incompatible
	/// </summary>
	class IPropertyValue
	{
	public:
		/// <summary>
		/// Type checker methods
		/// </summary>
		virtual bool IsString() const noexcept = 0;
		virtual bool IsInteger() const noexcept = 0;
		virtual bool IsDouble() const noexcept = 0;
		virtual bool IsBoolean() const noexcept = 0;

		/// <summary>
		/// Returns true if the propery is an immutable system constant
		/// </summary>
		virtual bool IsConstant() const noexcept = 0;

		/// <summary>
		/// Type specific accessor methods
		/// </summary>
		virtual OperationResult TryGetStringValue(const char*& result) const noexcept = 0;
		virtual OperationResult TryGetIntegerValue(int64_t& result) const noexcept = 0;
		virtual OperationResult TryGetDoubleValue(double& result) const noexcept = 0;
		virtual OperationResult TryGetBooleanValue(bool& result) const noexcept = 0;

		/// <summary>
		/// Property setter methods.
		/// TODO: MSVC bug does not allow these to have the same name, could cleanup
		/// </summary>
		virtual OperationResult TrySetStringValue(const char* value) noexcept = 0;
		virtual OperationResult TrySetIntegerValue(int64_t value) noexcept = 0;
		virtual OperationResult TrySetDoubleValue(double value) noexcept = 0;
		virtual OperationResult TrySetBooleanValue(bool value) noexcept = 0;
	};
}
