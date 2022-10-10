// <copyright file="IBuildEvaluateEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationGraph/OperationGraph.h"

namespace Soup::Core
{
	/// <summary>
	/// The core build evaluation interface that knows how to perform a build from a provided Operation Graph.
	/// </summary>
	export class IEvaluateEngine
	{
	public:
		/// <summary>
		/// Execute the entire operation graph that is referenced by this build evaluate engine
		/// </summary>
		virtual void Evaluate(
			OperationGraph& operationGraph,
			const Path& temporaryDirectory,
			const std::vector<Path>& globalAllowedReadAccess,
			const std::vector<Path>& globalAllowedWriteAccess) = 0;
	};
}
