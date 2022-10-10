// <copyright file="MockEvaluateEngine.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core
{
	/// <summary>
	/// The mock build evaluation engine
	/// </summary>
	class MockEvaluateEngine : public IEvaluateEngine
	{
	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="MockEvaluateEngine"/> class.
		/// </summary>
		MockEvaluateEngine()
		{
		}

		/// <summary>
		/// Execute the entire operation graph that is referenced by this build evaluate engine
		/// </summary>
		void Evaluate(
			OperationGraph& operationGraph,
			const Path& temporaryDirectory,
			const std::vector<Path>& globalAllowedReadAccess,
			const std::vector<Path>& globalAllowedWriteAccess)
		{
		}
	};
}
