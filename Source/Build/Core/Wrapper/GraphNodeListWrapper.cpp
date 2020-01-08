// <copyright file="GraphNodeListWrapper.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
module;

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

module SoupBuildCore;

using namespace Opal;

namespace Soup::Build
{

	GraphNodeListWrapper::GraphNodeListWrapper(IList<IGraphNode*>& value) :
		_value(value)
	{
	}

	uint64_t GraphNodeListWrapper::GetSize() const noexcept
	{
		return _value.GetSize();
	}

	void GraphNodeListWrapper::Resize(uint64_t size)
	{
		auto status = _value.Resize(size);
		if (status != 0)
			throw std::runtime_error("Resize Failed");
	}

	GraphNodeWrapper GraphNodeListWrapper::GetValueAt(uint64_t index) const
	{
		IGraphNode* result = nullptr;
		auto status = _value.TryGetValueAt(index, result);

		// Store the out result to ensure we do not leak it by accident
		Memory::Reference<IGraphNode> resultReference = result;

		if (status != 0)
			throw std::runtime_error("TryGetValueAt Failed");

		return GraphNodeWrapper(resultReference);
	}

	void GraphNodeListWrapper::SetValueAt(uint64_t index, GraphNodeWrapper& value)
	{
		auto status = _value.TrySetValueAt(index, value.GetRaw());
		if (status != 0)
			throw std::runtime_error("TrySetValueAt Failed");
	}

	void GraphNodeListWrapper::Append(GraphNodeWrapper& value)
	{
		auto currentSize = static_cast<size_t>(GetSize());
		auto finalSize = currentSize + 1;
		Resize(static_cast<uint64_t>(finalSize));
		SetValueAt(currentSize, value);
	}

	void GraphNodeListWrapper::Append(const GraphNodeListWrapper& values)
	{
		auto currentSize = GetSize();
		auto finalSize = currentSize + values.GetSize();
		Resize(finalSize);
		for (size_t i = currentSize; i < finalSize; i++)
		{
			SetValueAt(i, values.GetValueAt(i - currentSize));
		}
	}

	void GraphNodeListWrapper::Append(std::vector<GraphNodeWrapper>& values)
	{
		auto currentSize = static_cast<size_t>(GetSize());
		auto finalSize = currentSize + values.size();
		Resize(static_cast<uint64_t>(finalSize));
		for (size_t i = currentSize; i < finalSize; i++)
		{
			SetValueAt(i, values[i - currentSize]);
		}
	}
}
