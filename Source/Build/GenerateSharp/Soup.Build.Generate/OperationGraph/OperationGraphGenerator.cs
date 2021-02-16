// <copyright file="OperationGraph.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Utilities;
using System;
using System.Collections.Generic;

namespace Soup.Build.Generate
{
	/// <summary>
	/// The cached operation graph that is used to track input/output mappings for previous build
	/// executions to support incremental builds
	/// </summary>
	internal class OperationGraphGenerator
	{
		public OperationGraphGenerator(FileSystemState fileSystemState)
		{
			_fileSystemState = fileSystemState;
			_uniqueId = 0;
			_graph = new OperationGraph();
			_outputFileLookup = new Dictionary<FileId, IList<OperationInfo>>();
		}

		/// <summary>
		/// Create an operation graph from the provided generated graph
		/// At the same time verify that there are no cycles
		/// </summary>
		public void CreateOperation(
			string title,
			Path executable,
			string arguments,
			Path workingDirectory,
			IList<Path> declaredInput,
			IList<Path> declaredOutput)
		{
			if (!workingDirectory.HasRoot)
				throw new InvalidOperationException("Working directory must be an absolute path.");

			// Build up the operation unique command
			var commandInfo = new CommandInfo(
				workingDirectory,
				executable,
				arguments);

			// Ensure this is the a unique operation
			if (_graph.HasCommand(commandInfo))
			{
				throw new InvalidOperationException("Operation with this command already exists.");
			}

			// Generate a unique id for this new operation
			_uniqueId = new OperationId(_uniqueId.value + 1);
			var operationId = _uniqueId;

			// Build up the declared build operation
			auto declaredInputFileIds = _fileSystemState->ToFileIds(declaredInput, commandInfo.WorkingDirectory);
			auto declaredOutputFileIds = _fileSystemState->ToFileIds(declaredOutput, commandInfo.WorkingDirectory);
			_graph.AddOperation(
				OperationInfo(
					operationId,
					std::move(title),
					std::move(commandInfo),
					std::move(declaredInputFileIds),
					std::move(declaredOutputFileIds)));
		}

		public OperationGraph BuildGraph()
		{
			// Store the operation in the required file lookups to help build up the dependency graph
			for (auto& operation : _graph.GetOperations())
			{
				auto& operationInfo = operation.second;
				for (auto file : operationInfo.DeclaredOutput)
				{
					auto& filePath = _fileSystemState->GetFilePath(file);
					if (filePath.HasFileName())
					{
						EnsureOutputFileOperations(file).push_back(std::ref(operationInfo));
					}
					else
					{
						EnsureOutputDirectoryOperations(file).push_back(std::ref(operationInfo));
					}
				}
			}

			// Build up the child dependencies based on the operations that use this operations output files
			for (auto& activeOperation : _graph.GetOperations())
			{
				auto& activeOperationInfo = activeOperation.second;

				// Check for inputs that match previous output files
				for (auto file : activeOperationInfo.DeclaredInput)
				{
					IList<std::reference_wrapper<OperationInfo>>* matchedOperations = nullptr;
					if (TryGetOutputFileOperations(file, matchedOperations))
					{
						for (OperationInfo& matchedOperation : *matchedOperations)
						{
							// The active operation must run after the matched output operation
							if (UniqueAdd(matchedOperation.Children, activeOperationInfo.Id))
							{
								activeOperationInfo.DependencyCount++;
							}
						}
					}
				}

				// Check for output files that are under previous output directories
				for (auto file : activeOperationInfo.DeclaredOutput)
				{
					auto& filePath = _fileSystemState->GetFilePath(file);
					auto parentDirectory = filePath.GetParent();
					auto done = false;
					while (!done)
					{
						FileId parentDirectoryId;
						if (_fileSystemState->TryFindFileId(parentDirectory, parentDirectoryId))
						{
							IList<std::reference_wrapper<OperationInfo>>* matchedOperations = nullptr;
							if (TryGetOutputDirectoryOperations(parentDirectoryId, matchedOperations))
							{
								for (OperationInfo& matchedOperation : *matchedOperations)
								{
									// The matched directory output operation must run before the active operation
									if (UniqueAdd(matchedOperation.Children, activeOperationInfo.Id))
									{
										activeOperationInfo.DependencyCount++;
									}
								}
							}
						}

						// Get the next parent directory
						auto nextParentDirectory = parentDirectory.GetParent();
						done = nextParentDirectory.ToString().size() == parentDirectory.ToString().size();
						parentDirectory = std::move(nextParentDirectory);
					}
				}
			}

			// Add any operation with zero dependencies to the root
			auto rootOperations = IList<OperationId>();
			for (auto& activeOperation : _graph.GetOperations())
			{
				auto& activeOperationInfo = activeOperation.second;
				if (activeOperationInfo.DependencyCount == 0)
				{
					activeOperationInfo.DependencyCount = 1;
					rootOperations.push_back(activeOperationInfo.Id);
				}
			}

			_graph.SetRootOperationIds(std::move(rootOperations));

			return std::move(_graph);
		}

	private:
		bool UniqueAdd(IList<OperationId>& operationList, OperationId operation)
		{
			if (std::find(operationList.begin(), operationList.end(), operation) == operationList.end())
			{
				operationList.push_back(operation);
				return true;
			}
			else
			{
				return false;
			}
		}

		bool TryGetOutputFileOperations(
			FileId file,
			IList<std::reference_wrapper<OperationInfo>>*& operations)
		{
			auto result = _outputFileLookup.find(file);
			if (result != _outputFileLookup.end())
			{
				operations = &result->second;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool TryGetOutputDirectoryOperations(
			FileId file,
			IList<std::reference_wrapper<OperationInfo>>*& operations)
		{
			auto result = _outputDirectoryLookup.find(file);
			if (result != _outputDirectoryLookup.end())
			{
				operations = &result->second;
				return true;
			}
			else
			{
				return false;
			}
		}

		IList<std::reference_wrapper<OperationInfo>>& EnsureOutputFileOperations(
			FileId file)
		{
			auto result = _outputFileLookup.emplace(file, IList<std::reference_wrapper<OperationInfo>>());
			return result.first->second;
		}

		IList<std::reference_wrapper<OperationInfo>>& EnsureOutputDirectoryOperations(
			FileId file)
		{
			auto result = _outputDirectoryLookup.emplace(file, IList<std::reference_wrapper<OperationInfo>>());
			return result.first->second;
		}

	private:
		std::shared_ptr<FileSystemState> _fileSystemState;
		OperationId _uniqueId;
		OperationGraph _graph;
		std::unordered_map<FileId, IList<std::reference_wrapper<OperationInfo>>> _outputFileLookup;
		std::unordered_map<FileId, IList<std::reference_wrapper<OperationInfo>>> _outputDirectoryLookup;
	};
}
