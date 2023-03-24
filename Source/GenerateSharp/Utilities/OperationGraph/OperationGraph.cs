// <copyright file="OperationGraph.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The cached operation graph that is used to track input/output mappings for previous build
	/// executions to support incremental builds
	/// </summary>
	public class OperationGraph
	{
		private List<(FileId FileId, Path Path)> _referencedFiles;
		private List<OperationId> _rootOperations;
		private Dictionary<OperationId, OperationInfo> _operations;
		private Dictionary<CommandInfo, OperationId> _operationLookup;

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationGraph"/> class.
		/// </summary>
		public OperationGraph()
		{
			_referencedFiles = new List<(FileId FileId, Path Path)>();
			_rootOperations = new List<OperationId>();
			_operations = new Dictionary<OperationId, OperationInfo>();
			_operationLookup = new Dictionary<CommandInfo, OperationId>();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationGraph"/> class.
		/// </summary>
		public OperationGraph(
			List<(FileId FileId, Path Path)> referencedFiles,
			List<OperationId> rootOperations,
			List<OperationInfo> operations)
		{
			_referencedFiles = referencedFiles;
			_rootOperations = rootOperations;
			_operations = new Dictionary<OperationId, OperationInfo>();
			_operationLookup = new Dictionary<CommandInfo, OperationId>();

			// Store the incoming vector of operations as a lookup for fast checks
			foreach (var info in operations)
			{
				AddOperation(info);
			}
		}

		/// <summary>
		/// Get the set of referenced file ids that map to their paths
		/// </summary>
		public List<(FileId FileId, Path Path)> ReferencedFiles
		{
			get { return _referencedFiles; }
			set { _referencedFiles = value; }
		}

		/// <summary>
		/// Get the list of root operation ids
		/// </summary>
		public List<OperationId> RootOperationIds
		{
			get { return _rootOperations; }
			set { _rootOperations = value; }
		}

		/// <summary>
		/// Get Operations
		/// </summary>
		public IReadOnlyDictionary<OperationId, OperationInfo> Operations => _operations;

		/// <summary>
		/// Find an operation info
		/// </summary>
		public bool HasCommand(CommandInfo command)
		{
			return _operationLookup.ContainsKey(command);
		}

		/// <summary>
		/// Find an operation info
		/// </summary>
		public bool TryFindOperationInfo(
			CommandInfo command,
			out OperationInfo operation)
		{
			if (_operationLookup.TryGetValue(command, out var operationId))
			{
				operation = GetOperationInfo(operationId);
				return true;
			}
			else
			{
				operation = new OperationInfo();
				return false;
			}
		}

		/// <summary>
		/// Get an operation info
		/// </summary>
		public OperationInfo GetOperationInfo(OperationId operationId)
		{
			return _operations[operationId];
		}

		/// <summary>
		/// Add an operation info
		/// </summary>
		public void AddOperation(OperationInfo info)
		{
			_operationLookup.Add(info.Command, info.Id);
			_operations.Add(info.Id, info);
		}
	}
}
