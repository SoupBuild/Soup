// <copyright file="OperationGraph.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Utilities;
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
			_uniqueId = new OperationId(0);
			_graph = new OperationGraph();
			_outputFileLookup = new Dictionary<FileId, IList<OperationInfo>>();
			_outputDirectoryLookup = new Dictionary<FileId, IList<OperationInfo>>();
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
			List<Path> declaredInput,
			List<Path> declaredOutput)
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
			var declaredInputFileIds = _fileSystemState.ToFileIds(declaredInput, commandInfo.WorkingDirectory);
			var declaredOutputFileIds = _fileSystemState.ToFileIds(declaredOutput, commandInfo.WorkingDirectory);
			_graph.AddOperation(
				new OperationInfo(
					operationId,
					title,
					commandInfo,
					declaredInputFileIds,
					declaredOutputFileIds));
		}

		public OperationGraph BuildGraph()
		{
			// Store the operation in the required file lookups to help build up the dependency graph
			foreach (var operationInfo in _graph.GetOperations().Values)
			{
				foreach (var file in operationInfo.DeclaredOutput)
				{
					var filePath = _fileSystemState.GetFilePath(file);
					if (filePath.HasFileName())
					{
						EnsureOutputFileOperations(file).Add(operationInfo);
					}
					else
					{
						EnsureOutputDirectoryOperations(file).Add(operationInfo);
					}
				}
			}

			// Build up the child dependencies based on the operations that use this operations output files
			foreach (var activeOperationInfo in _graph.GetOperations().Values)
			{
				// Check for inputs that match previous output files
				foreach (var file in activeOperationInfo.DeclaredInput)
				{
					if (TryGetOutputFileOperations(file, out var matchedOperations))
					{
						foreach (var matchedOperation in matchedOperations)
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
				foreach (var file in activeOperationInfo.DeclaredOutput)
				{
					var filePath = _fileSystemState.GetFilePath(file);
					var parentDirectory = filePath.GetParent();
					var done = false;
					while (!done)
					{
						if (_fileSystemState.TryFindFileId(parentDirectory, out var parentDirectoryId))
						{
							if (TryGetOutputDirectoryOperations(parentDirectoryId, out var matchedOperations))
							{
								foreach (OperationInfo matchedOperation in matchedOperations)
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
						var nextParentDirectory = parentDirectory.GetParent();
						done = nextParentDirectory.ToString().Length == parentDirectory.ToString().Length;
						parentDirectory = nextParentDirectory;
					}
				}
			}

			// Add any operation with zero dependencies to the root
			var rootOperations = new List<OperationId>();
			foreach (var activeOperationInfo in _graph.GetOperations().Values)
			{
				if (activeOperationInfo.DependencyCount == 0)
				{
					activeOperationInfo.DependencyCount = 1;
					rootOperations.Add(activeOperationInfo.Id);
				}
			}

			_graph.SetRootOperationIds(rootOperations);

			return _graph;
		}

		private bool UniqueAdd(IList<OperationId> operationList, OperationId operation)
		{
			if (!operationList.Contains(operation))
			{
				operationList.Add(operation);
				return true;
			}
			else
			{
				return false;
			}
		}

		private bool TryGetOutputFileOperations(
			FileId file,
			out IList<OperationInfo> operations)
		{
			if (_outputFileLookup.TryGetValue(file, out var value))
            {
				operations = value;
				return true;
			}
			else
            {
				operations = new List<OperationInfo>();
				return false;
            }
		}

		private bool TryGetOutputDirectoryOperations(
			FileId file,
			out IList<OperationInfo> operations)
		{
			if (_outputDirectoryLookup.TryGetValue(file, out var value))
			{
				operations = value;
				return true;
			}
			else
			{
				operations = new List<OperationInfo>();
				return false;
			}
		}

		private IList<OperationInfo> EnsureOutputFileOperations(
			FileId file)
		{
			var result = new List<OperationInfo>();
			_outputFileLookup.Add(file, result);
			return result;
		}

		private IList<OperationInfo> EnsureOutputDirectoryOperations(
			FileId file)
		{
			var result = new List<OperationInfo>();
			_outputDirectoryLookup.Add(file, result);
			return result;
		}

		private FileSystemState _fileSystemState;
		private OperationId _uniqueId;
		private OperationGraph _graph;
		private Dictionary<FileId, IList<OperationInfo>> _outputFileLookup;
		private Dictionary<FileId, IList<OperationInfo>> _outputDirectoryLookup;
	};
}
