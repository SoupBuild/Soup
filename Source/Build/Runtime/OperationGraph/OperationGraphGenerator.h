// <copyright file="OperationGraph.h" company="Soup">
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
	export class OperationGraphGenerator
	{
	public:
		OperationGraphGenerator(
			FileSystemState& fileSystemState) :
			_fileSystemState(fileSystemState),
			_uniqueId(0),
			_duplicateMonitor()
		{
		}

		/// <summary>
		/// Create an operation graph from the provided generated graph
		/// At the same time verify that there are no cycles
		/// </summary>
		OperationGraph CreateFromDefinition(
			Utilities::BuildOperationListWrapper operations)
		{
			auto result = OperationGraph(
				_fileSystemState.GetId());

			// Build the initial operation dependency set to
			// ensure operations are built in the correct order 
			// and that there are no cycles
			auto emptyParentSet = std::set<OperationId>();

			auto rootOperationIds = BuildFromDefinition(
				operations,
				emptyParentSet,
				result);

			result.SetRootOperationIds(std::move(rootOperationIds));

			return result;
		}

	private:
		std::vector<OperationId> BuildFromDefinition(
			Utilities::BuildOperationListWrapper operations,
			const std::set<OperationId>& parentSet,
			OperationGraph& graph)
		{
			auto operationIds = std::vector<OperationId>();
			for (auto i = 0; i < operations.GetSize(); i++)
			{
				// Find the unique OperationInfo for the Build Operation
				auto operation = operations.GetValueAt(i);
				auto operatioId = BuildOperationInfo(operation, parentSet, graph);
				operationIds.push_back(operatioId);
			}

			return operationIds;
		}

		/// <summary>
		/// Ensure the provided operation exists in the operation graph
		/// If this is the first visit, then recurse to all children and store their ids
		/// If this is a subsequent visit, track the dependency count for easy builds in the future
		/// </summary>
		OperationId BuildOperationInfo(
			Utilities::BuildOperationWrapper operation,
			const std::set<OperationId>& parentSet,
			OperationGraph& graph)
		{
			// Build up the operation unique command
			auto commandInfo = CommandInfo(
				Path(operation.GetWorkingDirectory()),
				Path(operation.GetExecutable()),
				std::string(operation.GetArguments()));

			OperationInfo* rawOperationInfo = nullptr;
			if (graph.TryFindOperationInfo(commandInfo, rawOperationInfo))
			{
				auto& operationInfo = *rawOperationInfo;

				// Verify that there are no duplicate commands from different nodes
				auto findResult = _duplicateMonitor.find(operationInfo.Id);
				if (findResult == _duplicateMonitor.end())
					throw std::runtime_error("The existing operation id does not exist in the depency tracker");
				if (findResult->second != operation.GetRaw())
					throw std::runtime_error("Found duplicate operation commands from different nodes");

				// Increment the dependency count only since all children will be traversed on first visit
				operationInfo.DependencyCount++;

				return operationInfo.Id;
			}
			else
			{
				// Generate a unique id for this new operation
				auto operationId = ++_uniqueId;

				// Recurse to children only the first time we see an operation
				auto updatedParentSet = parentSet;
				updatedParentSet.insert(operationId);
				auto childOperationIds = BuildFromDefinition(
					operation.GetChildList(),
					updatedParentSet,
					graph);

				// Build up the declared build operation
				auto declaredInput = _fileSystemState.ToFileIds(
					operation.GetInputFileList().CopyAsPathVector(),
					commandInfo.WorkingDirectory);
				auto declaredOutput = _fileSystemState.ToFileIds(
					operation.GetOutputFileList().CopyAsPathVector(),
					commandInfo.WorkingDirectory);
				auto operationInfo = OperationInfo(
					operationId,
					std::string(operation.GetTitle()),
					std::move(commandInfo),
					std::move(declaredInput),
					std::move(declaredOutput),
					std::move(childOperationIds),
					1);

				auto& insertedOperationInfo = graph.AddOperationInfo(std::move(operationInfo));

				// Track the creator operation address to monitor for duplicate commands
				auto insertResult = _duplicateMonitor.emplace(insertedOperationInfo.Id, operation.GetRaw());
				if (!insertResult.second)
					throw std::runtime_error("The new operation id already exists in the duplicate tracker");

				return insertedOperationInfo.Id;
			}
		}

	private:
		FileSystemState& _fileSystemState;
		OperationId _uniqueId;
		std::map<OperationId, void*> _duplicateMonitor;
	};
}
