// <copyright file="OperationGraph.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Runtime
{
	/// <summary>
	/// The cached operation graph that is used to track input/output mappings for previous build
	/// executions to support incremental builds
	/// </summary>
	internal class OperationGraphGenerator
	{
		private FileSystemState fileSystemState;
		private IList<Path> readAccessList;
		private IList<Path> writeAccessList;
		private OperationId uniqueId;
		private OperationGraph graph;
		private Dictionary<FileId, IList<OperationInfo>> outputFileLookup;
		private Dictionary<FileId, IList<OperationInfo>> outputDirectoryLookup;

		public OperationGraphGenerator(
			FileSystemState fileSystemState,
			IList<Path> readAccessList,
			IList<Path> writeAccessList)
		{
			this.fileSystemState = fileSystemState;
			this.readAccessList = readAccessList;
			this.writeAccessList = writeAccessList;

			this.uniqueId = new OperationId(0);
			this.graph = new OperationGraph();
			this.outputFileLookup = new Dictionary<FileId, IList<OperationInfo>>();
			this.outputDirectoryLookup = new Dictionary<FileId, IList<OperationInfo>>();
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
			Log.Diag($"Create Operation: {title}");

			if (!workingDirectory.HasRoot)
				throw new InvalidOperationException("Working directory must be an absolute path.");

			// Build up the operation unique command
			var commandInfo = new CommandInfo(
				workingDirectory,
				executable,
				arguments);

			// Ensure this is the a unique operation
			if (this.graph.HasCommand(commandInfo))
			{
				throw new InvalidOperationException("Operation with this command already exists.");
			}

			// Verify allowed read access
			if (!IsAllowedAccess(this.readAccessList, declaredInput, workingDirectory, out var readAccess))
			{
				throw new InvalidOperationException("Operation does not have permission to read requested input.");
			}

			Log.Diag($"Read Access Subset:");
			foreach (var file in readAccess)
				Log.Diag($"{file}");

			// Verify allowed write access
			if (!IsAllowedAccess(this.writeAccessList, declaredOutput, workingDirectory, out var writeAccess))
			{
				throw new InvalidOperationException("Operation does not have permission to write requested output.");
			}

			Log.Diag($"Write Access Subset:");
			foreach (var file in writeAccess)
				Log.Diag($"{file}");

			// Generate a unique id for this new operation
			this.uniqueId = new OperationId(this.uniqueId.value + 1);
			var operationId = this.uniqueId;

			// Build up the declared build operation
			var declaredInputFileIds = this.fileSystemState.ToFileIds(declaredInput, commandInfo.WorkingDirectory);
			var declaredOutputFileIds = this.fileSystemState.ToFileIds(declaredOutput, commandInfo.WorkingDirectory);
			var readAccessFileIds = this.fileSystemState.ToFileIds(readAccess, commandInfo.WorkingDirectory);
			var writeAccessFileIds = this.fileSystemState.ToFileIds(writeAccess, commandInfo.WorkingDirectory);
			this.graph.AddOperation(
				new OperationInfo(
					operationId,
					title,
					commandInfo,
					declaredInputFileIds,
					declaredOutputFileIds,
					readAccessFileIds,
					writeAccessFileIds));
		}

		public OperationGraph BuildGraph()
		{
			// Store the operation in the required file lookups to help build up the dependency graph
			foreach (var operationInfo in this.graph.Operations.Values)
			{
				foreach (var file in operationInfo.DeclaredOutput)
				{
					var filePath = this.fileSystemState.GetFilePath(file);
					if (filePath.HasFileName)
					{
						AddOutputFileOperations(file).Add(operationInfo);
					}
					else
					{
						AddOutputDirectoryOperations(file).Add(operationInfo);
					}
				}
			}

			// Build up the child dependencies based on the operations that use this operations output files
			foreach (var activeOperationInfo in this.graph.Operations.Values)
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
					var filePath = this.fileSystemState.GetFilePath(file);
					var parentDirectory = filePath.GetParent();
					var done = false;
					while (!done)
					{
						if (this.fileSystemState.TryFindFileId(parentDirectory, out var parentDirectoryId))
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
			foreach (var activeOperationInfo in this.graph.Operations.Values)
			{
				if (activeOperationInfo.DependencyCount == 0)
				{
					activeOperationInfo.DependencyCount = 1;
					rootOperations.Add(activeOperationInfo.Id);
				}
			}

			this.graph.RootOperationIds = rootOperations;

			// Remove extra dependency references that are already covered by upstream references
			var recursiveChildren = new Dictionary<OperationId, HashSet<OperationId>>();
			BuildRecursiveChildSets(recursiveChildren, rootOperations);
			foreach (var operation in this.graph.Operations.Values)
			{
				// Check each child to see if it is already covered by another child
				var removeList = new List<OperationId>();
				foreach (var childId in operation.Children)
				{
					var isDuplicate = operation.Children.Any(secondChildId =>
					{
						if (secondChildId != childId)
						{
							var childRecursiveSet = recursiveChildren[secondChildId];
							return childRecursiveSet.Contains(childId);
						}
						else
						{
							return false;
						}
					});

					if (isDuplicate)
					{
						// Cache the item to remove outside the iterator
						removeList.Add(childId);
					}
				}

				foreach (var childId in removeList)
				{
					operation.Children.Remove(childId);
					var childOperation = this.graph.GetOperationInfo(childId);
					childOperation.DependencyCount--;
				}
			}	

			return this.graph;
		}

		private void BuildRecursiveChildSets(Dictionary<OperationId, HashSet<OperationId>> recursiveChildren, IList<OperationId> operations)
		{
			foreach (var operationId in operations)
			{
				var operation = this.graph.GetOperationInfo(operationId);
				BuildRecursiveChildSets(recursiveChildren, operation.Children);

				// Check if this node was already handled in a different branch
				if (!recursiveChildren.ContainsKey(operationId))
				{
					var childSet = new HashSet<OperationId>();
					childSet.UnionWith(operation.Children);
					foreach (var childId in operation.Children)
					{
						var childRecursiveSet = recursiveChildren[childId];
						childSet.UnionWith(childRecursiveSet);
					}

					recursiveChildren.Add(operationId, childSet);
				}
			}
		}

		private bool IsAllowedAccess(
			IList<Path> accessList,
			IList<Path> files,
			Path workingDirectory,
			out IReadOnlyList<Path> usedAccessList)
		{
			var accessSet = new HashSet<Path>();
			foreach (var file in files)
			{
				var accessDirectory = GetAllowedAccess(accessList, file, workingDirectory);
				if (accessDirectory is null)
				{
					// The file was not under any of the provided directories
					Log.Error($"File access denied: {file}");
					usedAccessList = new List<Path>();
					return false;
				}
				else
				{
					accessSet.Add(accessDirectory);
				}
			}


			usedAccessList = accessSet.ToList();
			return true;
		}

		private Path? GetAllowedAccess(IList<Path> accessList, Path file, Path workingDirectory)
		{
			// Combine the working directory if a relative file path
			if (!file.HasRoot)
				file = workingDirectory + file;

			var fileString = file.ToString();
			foreach (var accessDirectory in accessList)
			{
				if (fileString.StartsWith(accessDirectory.ToString()))
				{
					return accessDirectory;
				}
			}

			return null;
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
			if (this.outputFileLookup.TryGetValue(file, out var value))
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
			if (this.outputDirectoryLookup.TryGetValue(file, out var value))
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

		private IList<OperationInfo> AddOutputFileOperations(
			FileId file)
		{
			if (this.outputFileLookup.ContainsKey(file))
			{
				var filePath = this.fileSystemState.GetFilePath(file);
				throw new InvalidOperationException($"Operation output file already exists: {filePath}");
			}
			else
			{
				var result = new List<OperationInfo>();
				this.outputFileLookup.Add(file, result);
				return result;
			}
		}

		private IList<OperationInfo> AddOutputDirectoryOperations(
			FileId file)
		{
			if (this.outputFileLookup.ContainsKey(file))
			{
				throw new InvalidOperationException("Operation output directory already exists");
			}
			else
			{
				var result = new List<OperationInfo>();
				this.outputDirectoryLookup.Add(file, result);
				return result;
			}
		}
	}
}
