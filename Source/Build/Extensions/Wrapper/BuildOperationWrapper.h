// <copyright file="BuildOperationWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "StringListWrapper.h"
#include "BuildOperationListWrapper.h"
#include "ValuePrimitiveWrapper.h"

namespace Soup::Build::Extensions
{
	/// <summary>
	/// Build Property Bag Extension used to make the IBuildOperation interface more usable
	/// </summary>
	export class BuildOperationWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildOperationWrapper class
		/// </summary>
		BuildOperationWrapper() :
			_value(nullptr)
		{
		}

		/// <summary>
		/// Initializes a new instance of the BuildOperationWrapper class
		/// </summary>
		BuildOperationWrapper(IBuildOperation* value) :
			_value(value)
		{
		}

		/// <summary>
		/// Initializes a new instance of the BuildOperationWrapper class
		/// </summary>
		BuildOperationWrapper(Memory::Reference<IBuildOperation>& value) :
			_value(value)
		{
		}

		std::string_view GetTitle() const
		{
			ThrowIfInvalid();
			return _value->GetTitle();
		}

		std::string_view GetExecutable() const
		{
			ThrowIfInvalid();
			return _value->GetExecutable();
		}

		std::string_view GetArguments() const
		{
			ThrowIfInvalid();
			return _value->GetArguments();
		}

		std::string_view GetWorkingDirectory() const
		{
			ThrowIfInvalid();
			return _value->GetWorkingDirectory();
		}

		const StringListWrapper GetInputFileList() const
		{
			ThrowIfInvalid();
			auto& list = _value->GetInputFileList();
			// TODO: Create const version of wrapper
			return StringListWrapper(const_cast<IList<const char *>&>(list));
		}

		const StringListWrapper GetOutputFileList() const
		{
			ThrowIfInvalid();
			auto& list = _value->GetOutputFileList();
			// TODO: Create const version of wrapper
			return StringListWrapper(const_cast<IList<const char *>&>(list));
		}

		BuildOperationListWrapper GetChildList()
		{
			ThrowIfInvalid();
			auto& list = _value->GetChildList();
			return BuildOperationListWrapper(const_cast<IList<IBuildOperation*>&>(list));
		}

		/// <summary>
		/// Get raw access to the internal interface
		/// </summary>
		bool IsValid() const
		{
			return _value != nullptr;
		}

		void ThrowIfInvalid() const
		{
			if (!IsValid())
				throw std::runtime_error("Tried to use invalid graph node.");
		}

		IBuildOperation* GetRaw()
		{
			return _value.GetRaw();
		}

		const IBuildOperation* GetRaw() const
		{
			return _value.GetRaw();
		}

	private:
		Memory::Reference<IBuildOperation> _value;
	};
}
