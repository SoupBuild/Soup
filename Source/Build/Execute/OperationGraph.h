// <copyright file="OperationGraph.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationInfo.h"

namespace Soup::Build::Execute
{
	/// <summary>
	/// The cached operation graph that is used to track input/output mappings for previous build
	/// executions to support incremental builds
	/// </summary>
	export class OperationGraph
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="OperationGraph"/> class.
		/// </summary>
		OperationGraph(FileSystemStateId stateId) :
			_stateId(stateId),
			_rootOperations(),
			_operations(),
			_operationLookup()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationGraph"/> class.
		/// </summary>
		OperationGraph(
			FileSystemStateId stateId,
			std::vector<OperationId> rootOperations,
			std::vector<OperationInfo> operations) :
			_stateId(stateId),
			_rootOperations(std::move(rootOperations)),
			_operations(),
			_operationLookup()
		{
			// Store the incoming vector of operations as a lookup for fast checks
			for (auto& info : operations)
			{
				AddOperationInfo(std::move(info));
			}
		}

		/// <summary>
		/// Get the file system state id used for this operation graph
		/// </summary>
		FileSystemStateId GetStateId() const
		{
			return _stateId;
		}

		/// <summary>
		/// Get the list of root operation ids
		/// </summary>
		const std::vector<OperationId>& GetRootOperationIds() const
		{
			return _rootOperations;
		}

		/// <summary>
		/// Set the list of root operation ids
		/// </summary>
		void SetRootOperationIds(std::vector<OperationId> value)
		{
			_rootOperations = std::move(value);
		}

		/// <summary>
		/// Get Operations
		/// </summary>
		const std::unordered_map<OperationId, OperationInfo>& GetOperations() const
		{
			return _operations;
		}

		/// <summary>
		/// Get Operations
		/// </summary>
		std::unordered_map<OperationId, OperationInfo>& GetOperations()
		{
			return _operations;
		}

		/// <summary>
		/// Find an operation info
		/// </summary>
		bool TryFindOperationInfo(
			const CommandInfo& command,
			OperationInfo*& operation)
		{
			auto findResult = _operationLookup.find(command);
			if (findResult != _operationLookup.end())
			{
				operation = &GetOperationInfo(findResult->second);
				return true;
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// Get an operation info
		/// </summary>
		OperationInfo& GetOperationInfo(OperationId operationId)
		{
			auto findResult = _operations.find(operationId);
			if (findResult != _operations.end())
			{
				return findResult->second;
			}
			else
			{
				throw std::runtime_error("The provided operation id does not exist");
			}
		}

		/// <summary>
		/// Add an operation info
		/// </summary>
		OperationInfo& AddOperationInfo(OperationInfo info)
		{
			auto insertLookupResult = _operationLookup.emplace(info.Command, info.Id);
			if (!insertLookupResult.second)
				throw std::runtime_error("The provided command already exists in the graph");

			auto insertResult = _operations.emplace(info.Id, std::move(info));
			if (!insertResult.second)
				throw std::runtime_error("The provided operation id already exists in the graph");

			return insertResult.first->second;
		}

	private:
		FileSystemStateId _stateId;
		std::vector<OperationId> _rootOperations;
		std::unordered_map<OperationId, OperationInfo> _operations;
		std::unordered_map<CommandInfo, OperationId> _operationLookup;
	};
}
