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
	private:
		std::atomic<int> m_uniqueId;
		std::vector<std::string> _requests;

	public:
		/// <summary>
		/// Initializes a new instance of the <see cref="MockEvaluateEngine"/> class.
		/// </summary>
		MockEvaluateEngine()
		{
		}

		/// <summary>
		/// Get the load requests
		/// </summary>
		const std::vector<std::string>& GetRequests() const
		{
			return _requests;
		}

		/// <summary>
		/// Execute the entire operation graph that is referenced by this build evaluate engine
		/// </summary>
		bool Evaluate(
			const OperationGraph& operationGraph,
			OperationResults& operationResults,
			const Path& temporaryDirectory,
			const std::vector<Path>& /*globalAllowedReadAccess*/,
			const std::vector<Path>& /*globalAllowedWriteAccess*/)
		{
			std::stringstream message;
			message << "Evaluate: " << temporaryDirectory.ToString();

			_requests.push_back(message.str());

			auto time = std::chrono::clock_cast<std::chrono::file_clock>(
				std::chrono::time_point<std::chrono::system_clock>());
			for (auto& operation : operationGraph.GetOperations())
			{
				auto& operationInfo = operation.second;
				operationResults.AddOrUpdateOperationResult(
					operationInfo.Id,
					OperationResult(
					true,
					time,
					{ },
					{ }));
			}

			return true;
		}
	};
}
