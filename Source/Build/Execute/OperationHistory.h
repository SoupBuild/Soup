// <copyright file="OperationHistory.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationInfo.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The cached operation history that is used to track input/output mappings for previous build
	/// executions to support incremental builds
	/// </summary>
	export class OperationHistory
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="OperationHistory"/> class.
		/// </summary>
		OperationHistory() :
			_operations()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationHistory"/> class.
		/// </summary>
		OperationHistory(
			std::vector<OperationInfo> operations) :
			_operations()
		{
			// Store the incoming vector of operations as a lookup for fast checks
			for (auto& info : operations)
			{
				_operations.emplace(info.Command, std::move(info));
			}
		}

		/// <summary>
		/// Get Operations
		/// </summary>
		const std::unordered_map<CommandInfo, OperationInfo>& GetOperations() const
		{
			return _operations;
		}

		/// <summary>
		/// Find an operation info
		/// </summary>
		bool TryFindOperationInfo(const CommandInfo& command, const OperationInfo*& operation) const
		{
			auto findResult = _operations.find(command);
			if (findResult != _operations.end())
			{
				operation = &findResult->second;
				return true;
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// Add an operation info
		/// </summary>
		void AddOperationInfo(OperationInfo operation)
		{
			auto result = _operations.emplace(operation.Command, std::move(operation));
			// TODO: Do we really want to allow two of the same operation?
			// if (!result.second)
			// 	throw std::runtime_error("The provided command already exists in the build history.");
		}

	private:
		std::unordered_map<CommandInfo, OperationInfo> _operations;
	};
}
