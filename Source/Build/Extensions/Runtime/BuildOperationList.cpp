// <copyright file="BuildOperationList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

module;

#include <any>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

module Soup.Build.Extensions;

using namespace Opal;

namespace Soup::Build::Extensions
{
	BuildOperationList::BuildOperationList() :
		_values()
	{
	}

	BuildOperationList::BuildOperationList(std::vector<Memory::Reference<BuildOperation>> values) :
		_values(std::move(values))
	{
	}

	/// <summary>
	/// Size access methods
	/// </summary>
	uint64_t BuildOperationList::GetSize() const noexcept
	{
		return _values.size();
	}

	ApiCallResult BuildOperationList::TryResize(uint64_t size) noexcept
	{
		try
		{
			_values.resize(size);
			return ApiCallResult::Success;
		}
		catch (...)
		{
			// Unknown error
			return ApiCallResult::Error;
		}
	}

	ApiCallResult BuildOperationList::TryGetValueAt(uint64_t index, IBuildOperation*& result) const noexcept
	{
		try
		{
			// TODO: figure out Const issues
			const IBuildOperation* raw = _values.at(index).GetRaw();
			result = const_cast<IBuildOperation*>(raw);
			return ApiCallResult::Success;
		}
		catch (...)
		{
			// Unknown error
			return ApiCallResult::Error;
		}
	}

	ApiCallResult BuildOperationList::TrySetValueAt(uint64_t index, IBuildOperation* value) noexcept
	{
		try
		{
			// TODO: Store interface type?
			auto internalValue = dynamic_cast<BuildOperation*>(value);
			if (internalValue == nullptr)
				return ApiCallResult::Error;

			SetValueAt(index, internalValue);
			return ApiCallResult::Success;
		}
		catch (...)
		{
			// Unknown error
			return ApiCallResult::Error;
		}
	}

	void BuildOperationList::SetValueAt(uint64_t index, BuildOperation* value)
	{
		_values.at(index) = value;

#ifdef DEBUG
		// Ensure that we are not creating a circular loop
		auto currentNodes = std::set<int64_t>();
		for (auto& node : _values)
		{
			if (node != nullptr)
			{
				currentNodes.insert(node->GetId());
			}
		}

		for (auto& node : _values)
		{
			if (node != nullptr)
			{
				CheckCycles(currentNodes, *node);
			}
		}
#endif
	}

	std::vector<Memory::Reference<BuildOperation>>& BuildOperationList::GetValues()
	{
		return _values;
	}

	const std::vector<Memory::Reference<BuildOperation>>& BuildOperationList::GetValues() const
	{
		return _values;
	}

#ifdef DEBUG
	void BuildOperationList::CheckCycles(const std::set<int64_t>& rootNodes, const BuildOperation& node)
	{
		for (auto& childNode : node.GetChildren())
		{
			if (rootNodes.contains(childNode->GetId()))
			{
				throw std::runtime_error("Insertion created cycle in the build graph.");
			}

			CheckCycles(rootNodes, *childNode);
		}
	}
#endif
}
