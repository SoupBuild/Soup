// <copyright file="GraphNodeWrapper.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "StringListWrapper.h"
#include "GraphNodeListWrapper.h"

namespace Soup::Build::Extensions
{
	/// <summary>
	/// Build Property Bag Extension used to make the IGraphNode interface more usable
	/// </summary>
	export class GraphNodeWrapper
	{
	public:
		/// <summary>
		/// Initializes a new instance of the GraphNodeWrapper class
		/// </summary>
		GraphNodeWrapper() :
			_value(nullptr)
		{
		}

		/// <summary>
		/// Initializes a new instance of the GraphNodeWrapper class
		/// </summary>
		GraphNodeWrapper(IGraphNode* value) :
			_value(value)
		{
		}

		/// <summary>
		/// Initializes a new instance of the GraphNodeWrapper class
		/// </summary>
		GraphNodeWrapper(Memory::Reference<IGraphNode>& value) :
			_value(value)
		{
		}

		int64_t GetId() const noexcept
		{
			ThrowIfInvalid();
			return _value->GetId();
		}

		std::string_view GetTitle() const noexcept
		{
			ThrowIfInvalid();
			return _value->GetTitle();
		}

		void SetTitle(std::string_view value)
		{
			ThrowIfInvalid();
			auto status = _value->TrySetTitle(value.data());
			if (status != 0)
				throw std::runtime_error("TrySetTitle Failed");
		}

		std::string_view GetProgram() const noexcept
		{
			ThrowIfInvalid();
			return _value->GetProgram();
		}

		void SetProgram(std::string_view value)
		{
			ThrowIfInvalid();
			auto status = _value->TrySetProgram(value.data());
			if (status != 0)
				throw std::runtime_error("TrySetProgram Failed");
		}

		std::string_view GetArguments() const noexcept
		{
			ThrowIfInvalid();
			return _value->GetArguments();
		}

		void SetArguments(std::string_view value)
		{
			ThrowIfInvalid();
			auto status = _value->TrySetArguments(value.data());
			if (status != 0)
				throw std::runtime_error("TrySetArguments Failed");
		}

		std::string_view GetWorkingDirectory() const noexcept
		{
			ThrowIfInvalid();
			return _value->GetWorkingDirectory();
		}

		void SetWorkingDirectory(std::string_view value)
		{
			ThrowIfInvalid();
			auto status = _value->TrySetWorkingDirectory(value.data());
			if (status != 0)
				throw std::runtime_error("TrySetWorkingDirectory Failed");
		}

		StringListWrapper GetInputFileList() noexcept
		{
			ThrowIfInvalid();
			return StringListWrapper(_value->GetInputFileList());
		}

		StringListWrapper GetOutputFileList() noexcept
		{
			ThrowIfInvalid();
			return StringListWrapper(_value->GetOutputFileList());
		}

		GraphNodeListWrapper GetChildList() noexcept
		{
			ThrowIfInvalid();
			return GraphNodeListWrapper(_value->GetChildList());
		}

		/// <summary>
		/// Get raw access to the internal interface
		/// </summary>
		bool IsValid() const noexcept
		{
			return _value != nullptr;
		}

		void ThrowIfInvalid() const
		{
			if (!IsValid())
				throw std::runtime_error("Tried to use invalid graph node.");
		}

		IGraphNode* GetRaw() noexcept
		{
			return _value.GetRaw();
		}

	private:
		Memory::Reference<IGraphNode> _value;
	};
}
