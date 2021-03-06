﻿// <copyright file="OperationGraph.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationInfo.h"

namespace Soup::Build::Runtime
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
		OperationGraph() :
			_referencedFiles(),
			_rootOperations(),
			_operations(),
			_operationLookup()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationGraph"/> class.
		/// </summary>
		OperationGraph(
			std::vector<std::pair<FileId, Path>> referencedFiles,
			std::vector<OperationId> rootOperations,
			std::vector<OperationInfo> operations) :
			_referencedFiles(std::move(referencedFiles)),
			_rootOperations(std::move(rootOperations)),
			_operations(),
			_operationLookup()
		{
			// Store the incoming vector of operations as a lookup for fast checks
			for (auto& info : operations)
			{
				AddOperation(std::move(info));
			}
		}

		/// <summary>
		/// Get the set of referenced file ids that map to their paths
		/// </summary>
		std::vector<std::pair<FileId, Path>>& GetReferencedFiles()
		{
			return _referencedFiles;
		}

		const std::vector<std::pair<FileId, Path>>& GetReferencedFiles() const
		{
			return _referencedFiles;
		}
		
		void SetReferencedFiles(std::vector<std::pair<FileId, Path>> files)
		{
			_referencedFiles = std::move(files);
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
		bool HasCommand(const CommandInfo& command)
		{
			auto findResult = _operationLookup.find(command);
			if (findResult != _operationLookup.end())
			{
				return true;
			}
			else
			{
				return false;
			}
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
		OperationInfo& AddOperation(OperationInfo info)
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
		std::vector<std::pair<FileId, Path>> _referencedFiles;
		std::vector<OperationId> _rootOperations;
		std::unordered_map<OperationId, OperationInfo> _operations;
		std::unordered_map<CommandInfo, OperationId> _operationLookup;
	};
}
