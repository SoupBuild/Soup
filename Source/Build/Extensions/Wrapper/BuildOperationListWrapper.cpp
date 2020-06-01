// <copyright file="BuildOperationListWrapper.cpp" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

module;

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

module Soup.Build.Extensions;

using namespace Opal;

namespace Soup::Build::Extensions
{
	BuildOperationListWrapper::BuildOperationListWrapper(IList<IBuildOperation*>& value) :
		_value(value)
	{
	}

	uint64_t BuildOperationListWrapper::GetSize() const noexcept
	{
		return _value.GetSize();
	}

	void BuildOperationListWrapper::Resize(uint64_t size)
	{
		auto status = _value.TryResize(size);
		if (status != ApiCallResult::Success)
			throw std::runtime_error("Resize Failed");
	}

	BuildOperationWrapper BuildOperationListWrapper::GetValueAt(uint64_t index) const
	{
		IBuildOperation* result = nullptr;
		auto status = _value.TryGetValueAt(index, result);

		// Store the out result to ensure we do not leak it by accident
		Memory::Reference<IBuildOperation> resultReference = result;

		if (status != ApiCallResult::Success)
			throw std::runtime_error("TryGetValueAt Failed");

		return BuildOperationWrapper(resultReference);
	}

	void BuildOperationListWrapper::SetValueAt(uint64_t index, BuildOperationWrapper& value)
	{
		auto status = _value.TrySetValueAt(index, value.GetRaw());
		if (status != ApiCallResult::Success)
			throw std::runtime_error("TrySetValueAt Failed");
	}

	void BuildOperationListWrapper::Append(BuildOperationWrapper& value)
	{
		auto currentSize = static_cast<size_t>(GetSize());
		auto finalSize = currentSize + 1;
		Resize(static_cast<uint64_t>(finalSize));
		SetValueAt(currentSize, value);
	}

	void BuildOperationListWrapper::Append(const BuildOperationListWrapper& values)
	{
		auto currentSize = GetSize();
		auto finalSize = currentSize + values.GetSize();
		Resize(finalSize);
		for (size_t i = currentSize; i < finalSize; i++)
		{
			SetValueAt(i, values.GetValueAt(i - currentSize));
		}
	}

	void BuildOperationListWrapper::Append(std::vector<BuildOperationWrapper>& values)
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
