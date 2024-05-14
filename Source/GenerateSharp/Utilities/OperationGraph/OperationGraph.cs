// <copyright file="OperationGraph.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Path = Opal.Path;

namespace Soup.Build.Utilities;

/// <summary>
/// The cached operation graph that is used to track input/output mappings for previous build
/// executions to support incremental builds
/// </summary>
public class OperationGraph
{
	private readonly Dictionary<OperationId, OperationInfo> _operations;
	private readonly Dictionary<CommandInfo, OperationId> _operationLookup;

	/// <summary>
	/// Initializes a new instance of the <see cref="OperationGraph"/> class.
	/// </summary>
	public OperationGraph()
	{
		ReferencedFiles = [];
		RootOperationIds = [];
		this._operations = [];
		this._operationLookup = [];
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="OperationGraph"/> class.
	/// </summary>
	public OperationGraph(
		IList<(FileId FileId, Path Path)> referencedFiles,
		IList<OperationId> rootOperations,
		IList<OperationInfo> operations)
	{
		ReferencedFiles = referencedFiles;
		RootOperationIds = rootOperations;
		this._operations = [];
		this._operationLookup = [];

		// Store the incoming vector of operations as a lookup for fast checks
		foreach (var info in operations)
		{
			AddOperation(info);
		}
	}

	/// <summary>
	/// Get the set of referenced file ids that map to their paths
	/// </summary>
	public IList<(FileId FileId, Path Path)> ReferencedFiles { get; init; }

	/// <summary>
	/// Get the list of root operation ids
	/// </summary>
	public IList<OperationId> RootOperationIds { get; init; }

	/// <summary>
	/// Get Operations
	/// </summary>
	public IReadOnlyDictionary<OperationId, OperationInfo> Operations => this._operations;

	/// <summary>
	/// Find an operation info
	/// </summary>
	public bool HasCommand(CommandInfo command)
	{
		return this._operationLookup.ContainsKey(command);
	}

	/// <summary>
	/// Find an operation info
	/// </summary>
	public bool TryFindOperationInfo(
		CommandInfo command,
		out OperationInfo operation)
	{
		if (this._operationLookup.TryGetValue(command, out var operationId))
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
		return this._operations[operationId];
	}

	/// <summary>
	/// Add an operation info
	/// </summary>
	public void AddOperation(OperationInfo info)
	{
		this._operationLookup.Add(info.Command, info.Id);
		this._operations.Add(info.Id, info);
	}
}