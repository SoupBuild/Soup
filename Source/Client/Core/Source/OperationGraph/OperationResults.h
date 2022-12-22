// <copyright file="OperationResults.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationInfo.h"
#include "OperationResult.h"

namespace Soup::Core
{
	/// <summary>
	/// The cached operation results that is used to track input/output mappings for previous build
	/// executions to support incremental builds
	/// </summary>
	#ifdef SOUP_BUILD
	export
	#endif
	class OperationResults
	{
	private:
		std::unordered_map<OperationId, OperationResult> _results;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="OperationResults"/> class.
		/// </summary>
		OperationResults() :
			_results()
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="OperationResults"/> class.
		/// </summary>
		OperationResults(
			std::unordered_map<OperationId, OperationResult> results) :
			_results(std::move(results))
		{
		}

		/// <summary>
		/// Get Results
		/// </summary>
		const std::unordered_map<OperationId, OperationResult>& GetResults() const
		{
			return _results;
		}
		std::unordered_map<OperationId, OperationResult>& GetResults()
		{
			return _results;
		}

		/// <summary>
		/// Find an operation result
		/// </summary>
		bool TryFindResult(
			OperationId operationId,
			OperationResult*& result)
		{
			auto findResult = _results.find(operationId);
			if (findResult != _results.end())
			{
				result = &findResult->second;
				return true;
			}
			else
			{
				return false;
			}
		}

		/// <summary>
		/// Add an operation result
		/// </summary>
		OperationResult& AddOrUpdateOperationResult(OperationId operationId, OperationResult result)
		{
			auto [insertIterator, wasInserted] = _results.insert_or_assign(operationId, std::move(result));
			return insertIterator->second;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		bool operator ==(const OperationResults& rhs) const
		{
			return _results == rhs._results;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		bool operator !=(const OperationResults& rhs) const
		{
			return !(*this == rhs);
		}
	};
}
