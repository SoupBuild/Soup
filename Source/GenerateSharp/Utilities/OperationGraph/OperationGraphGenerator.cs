// <copyright file="OperationGraph.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The cached operation graph that is used to track input/output mappings for previous build
	/// executions to support incremental builds
	/// </summary>
	public class OperationGraphGenerator
	{
		private FileSystemState fileSystemState;
		private IList<Path> readAccessList;
		private IList<Path> writeAccessList;
		private OperationId uniqueId;
		private OperationGraph graph;

		// Running state used to build graph dynamically
		private Dictionary<FileId, IList<OperationInfo>> inputFileLookup;
		private Dictionary<FileId, OperationInfo> outputFileLookup;
		private Dictionary<FileId, OperationInfo> outputDirectoryLookup;

		public OperationGraphGenerator(
			FileSystemState fileSystemState,
			IList<Path> readAccessList,
			IList<Path> writeAccessList,
			OperationGraph graph)
		{
			this.fileSystemState = fileSystemState;
			this.readAccessList = readAccessList;
			this.writeAccessList = writeAccessList;
			this.graph = graph;

			this.uniqueId = new OperationId(0);

			this.inputFileLookup = new Dictionary<FileId, IList<OperationInfo>>();
			this.outputFileLookup = new Dictionary<FileId, OperationInfo>();
			this.outputDirectoryLookup = new Dictionary<FileId, OperationInfo>();
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

			// Resolve the requested files to unique ids
			var declaredInputFileIds = this.fileSystemState.ToFileIds(declaredInput, commandInfo.WorkingDirectory);
			var declaredOutputFileIds = this.fileSystemState.ToFileIds(declaredOutput, commandInfo.WorkingDirectory);
			var readAccessFileIds = this.fileSystemState.ToFileIds(readAccess, commandInfo.WorkingDirectory);
			var writeAccessFileIds = this.fileSystemState.ToFileIds(writeAccess, commandInfo.WorkingDirectory);

			// Build up the declared build operation
			var operationInfo = new OperationInfo(
				operationId,
				title,
				commandInfo,
				declaredInputFileIds,
				declaredOutputFileIds,
				readAccessFileIds,
				writeAccessFileIds);
			this.graph.AddOperation(operationInfo);

			StoreLookupInfo(operationInfo);
			ResolveDependencies(operationInfo);
		}

		public void FinalizeGraph()
		{
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
		}

		private void StoreLookupInfo(OperationInfo operationInfo)
		{
			// Store the operation in the required file lookups to ensure single target
			// and help build up the dependency graph
			foreach (var file in operationInfo.DeclaredOutput)
			{
				var filePath = this.fileSystemState.GetFilePath(file);
				if (filePath.HasFileName)
				{
					CheckSetOutputFileOperation(file, operationInfo);
				}
				else
				{
					CheckSetOutputDirectoryOperation(file, operationInfo);
				}
			}

			foreach (var file in operationInfo.DeclaredInput)
			{
				var filePath = this.fileSystemState.GetFilePath(file);
				if (filePath.HasFileName)
				{
					AddInputFileOperation(file, operationInfo);
				}
			}
		}

		private void ResolveDependencies(OperationInfo operationInfo)
		{
			// Build up the child dependencies based on the operations that use this operations output files

			// Check for inputs that match previous output files
			foreach (var file in operationInfo.DeclaredInput)
			{
				if (TryGetOutputFileOperation(file, out var matchedOperation))
				{
					// The active operation must run after the matched output operation
					CheckAddChildOperation(matchedOperation, operationInfo);
				}
			}

			// Check for outputs that match previous input files
			foreach (var file in operationInfo.DeclaredOutput)
			{
				if (TryGetInputFileOperations(file, out var matchedOperations))
				{
					foreach (var matchedOperation in matchedOperations)
					{
						// The active operation must run before the matched output operation
						CheckAddChildOperation(operationInfo, matchedOperation);
					}
				}
			}

			// Check for output files that are under previous output directories
			foreach (var file in operationInfo.DeclaredOutput)
			{
				var filePath = this.fileSystemState.GetFilePath(file);
				var parentDirectory = filePath.GetParent();
				var done = false;
				while (!done)
				{
					if (this.fileSystemState.TryFindFileId(parentDirectory, out var parentDirectoryId))
					{
						if (TryGetOutputDirectoryOperation(parentDirectoryId, out var matchedOperation))
						{
							// The matched directory output operation must run before the active operation
							CheckAddChildOperation(matchedOperation, operationInfo);
						}
					}

					// Get the next parent directory
					var nextParentDirectory = parentDirectory.GetParent();
					done = nextParentDirectory.ToString().Length == parentDirectory.ToString().Length;
					parentDirectory = nextParentDirectory;
				}
			}

			// Ensure there are no circular references
			var closure = new HashSet<OperationId>();
			BuildChildClosure(operationInfo.Children, closure);
			if (closure.Contains(operationInfo.Id))
			{
				throw new InvalidOperationException("Operation introduced circular reference");
			}	
		}

		private void BuildChildClosure(
			IList<OperationId> operations,
			HashSet<OperationId> closure)
		{
			foreach (var operationId in operations)
			{
				// Check if this node was already handled in a different branch
				if (!closure.Contains(operationId))
				{
					closure.Add(operationId);

					var operation = this.graph.GetOperationInfo(operationId);
					BuildChildClosure(operation.Children, closure);
				}
			}
		}

		private void BuildRecursiveChildSets(
			Dictionary<OperationId, HashSet<OperationId>> recursiveChildren,
			IList<OperationId> operations)
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

		private void CheckAddChildOperation(OperationInfo parentOperation, OperationInfo childOperation)
		{
			// Check if this item is already known
			if (!parentOperation.Children.Contains(childOperation.Id))
			{
				// Keep track of the parent child references
				parentOperation.Children.Add(childOperation.Id);
				childOperation.DependencyCount++;
			}
		}

		private bool TryGetInputFileOperations(
			FileId file,
			[MaybeNullWhen(false)] out IList<OperationInfo> operations)
		{
			if (this.inputFileLookup.TryGetValue(file, out var value))
			{
				operations = value;
				return true;
			}
			else
			{
				operations = null;
				return false;
			}
		}

		private bool TryGetOutputFileOperation(
			FileId file,
			[MaybeNullWhen(false)] out OperationInfo operation)
		{
			if (this.outputFileLookup.TryGetValue(file, out var value))
			{
				operation = value;
				return true;
			}
			else
			{
				operation = null;
				return false;
			}
		}

		private bool TryGetOutputDirectoryOperation(
			FileId file,
			[MaybeNullWhen(false)] out OperationInfo operation)
		{
			if (this.outputDirectoryLookup.TryGetValue(file, out var value))
			{
				operation = value;
				return true;
			}
			else
			{
				operation = null;
				return false;
			}
		}

		private void CheckSetOutputFileOperation(
			FileId file,
			OperationInfo operation)
		{
			if (this.outputFileLookup.TryGetValue(file, out var existingOperation))
			{
				var filePath = this.fileSystemState.GetFilePath(file);
				throw new InvalidOperationException($"File \"{filePath}\" already written to by operation \"{existingOperation.Title}\"");
			}
			else
			{
				this.outputFileLookup.Add(file, operation);
			}
		}

		private void CheckSetOutputDirectoryOperation(
			FileId file,
			OperationInfo operation)
		{
			if (this.outputDirectoryLookup.TryGetValue(file, out var existingOperation))
			{
				var filePath = this.fileSystemState.GetFilePath(file);
				throw new InvalidOperationException($"Directory \"{filePath}\" already written to by operation \"{existingOperation.Title}\"");
			}
			else
			{
				this.outputDirectoryLookup.Add(file, operation);
			}
		}

		private void AddInputFileOperation(
			FileId file,
			OperationInfo operation)
		{
			if (this.inputFileLookup.TryGetValue(file, out var operations))
			{
				operations.Add(operation);
			}
			else
			{
				this.inputFileLookup.Add(
					file,
					new List<OperationInfo>()
					{
						operation,
					});
			}
		}
	}
}
