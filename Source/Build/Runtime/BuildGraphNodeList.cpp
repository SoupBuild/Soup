// <copyright file="BuildGraphNodeList.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

module;

#include <any>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

module Soup.Build.Runtime;

using namespace Opal;

namespace Soup::Build::Runtime
{
	BuildGraphNodeList::BuildGraphNodeList() :
		_values()
	{
	}

	BuildGraphNodeList::BuildGraphNodeList(std::vector<Memory::Reference<BuildGraphNode>> values) :
		_values(std::move(values))
	{
	}

	/// <summary>
	/// Size access methods
	/// </summary>
	uint64_t BuildGraphNodeList::GetSize() const noexcept
	{
		return _values.size();
	}

	OperationResult BuildGraphNodeList::Resize(uint64_t size) noexcept
	{
		try
		{
			_values.resize(size);
			return 0;
		}
		catch (...)
		{
			// Unknown error
			return -1;
		}
	}

	OperationResult BuildGraphNodeList::TryGetValueAt(uint64_t index, IGraphNode*& result) noexcept
	{
		try
		{
			result = _values.at(index).GetRaw();
			return 0;
		}
		catch (...)
		{
			// Unknown error
			return -1;
		}
	}

	OperationResult BuildGraphNodeList::TrySetValueAt(uint64_t index, IGraphNode* value) noexcept
	{
		try
		{
			// TODO: Store interface type?
			auto internalValue = dynamic_cast<BuildGraphNode*>(value);
			if (internalValue == nullptr)
				return -2;

			SetValueAt(index, internalValue);
			return 0;
		}
		catch (...)
		{
			// Unknown error
			return -1;
		}
	}

	void BuildGraphNodeList::SetValueAt(uint64_t index, BuildGraphNode* value)
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

	const std::vector<Memory::Reference<BuildGraphNode>>& BuildGraphNodeList::GetValues() const
	{
		return _values;
	}

#ifdef DEBUG
	void BuildGraphNodeList::CheckCycles(const std::set<int64_t>& rootNodes, const BuildGraphNode& node)
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
