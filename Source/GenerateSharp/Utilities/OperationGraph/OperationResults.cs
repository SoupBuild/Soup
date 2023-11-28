// <copyright file="OperationResults.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;

namespace Soup.Build.Utilities;

/// <summary>
/// The cached operation results that is used to track input/output mappings for previous build
/// executions to support incremental builds
/// </summary>
public class OperationResults
{
	private readonly Dictionary<OperationId, OperationResult> _results;

	/// <summary>
	/// Initializes a new instance of the <see cref="OperationResults"/> class.
	/// </summary>
	public OperationResults()
	{
		ReferencedFiles = new List<(FileId FileId, Path Path)>();
		_results = [];
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="OperationResults"/> class.
	/// </summary>
	public OperationResults(
		IList<(FileId FileId, Path Path)> referencedFiles,
		Dictionary<OperationId, OperationResult> results)
	{
		ReferencedFiles = referencedFiles;
		_results = results;
	}

	/// <summary>
	/// Get the set of referenced file ids that map to their paths
	/// </summary>
	public IList<(FileId FileId, Path Path)> ReferencedFiles { get; init; }

	/// <summary>
	/// Get Results
	/// </summary>
	public IDictionary<OperationId, OperationResult> Results => _results;

	/// <summary>
	/// Find an operation result
	/// </summary>
	public bool TryFindResult(
		OperationId operationId,
		[MaybeNullWhen(false)] out OperationResult result)
	{
		if (_results.TryGetValue(operationId, out var operationResult))
		{
			result = operationResult;
			return true;
		}
		else
		{
			result = null;
			return false;
		}
	}
}