// <copyright file="GenerateState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Core::Generate
{
	/// <summary>
	/// Generate State used to track all context for state and operation graph creation
	/// </summary>
	class GenerateState
	{
	private:
		ValueTable _activeState;
		ValueTable _sharedState;
		// OperationGraphGenerator _graphGenerator;

	public:
		/// <summary>
		/// Initializes a new instance of the GenerateState class
		/// </summary>
		 GenerateState(
			ValueTable activeState,
			// FileSystemState& fileSystemState,
			std::vector<Path> readAccessList,
			std::vector<Path> writeAccessList) :
			_activeState(std::move(activeState)),
			_sharedState()
			// _graphGenerator(fileSystemState, std::move(readAccessList), std::move(writeAccessList))
		{
		}

		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		const ValueTable& ActiveState() const
		{
			return _activeState;
		}

		/// <summary>
		/// Get a reference to the shared state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		const ValueTable& SharedState() const
		{
			return _sharedState;
		}

		/// <summary>
		/// Create a build operation
		/// </summary>
		void CreateOperation(
			std::string title,
			std::string executable,
			std::string arguments,
			std::string workingDirectory,
			std::vector<std::string> declaredInput,
			std::vector<std::string> declaredOutput)
		{
			// graphGenerator.CreateOperation(
			// 	std::move(title),
			// 	Path(std::move(executable)),
			// 	std::move(arguments),
			// 	Path(std::move(workingDirectory)),
			// 	declaredInput.Select(value => new Path(value)).ToList(),
			// 	declaredOutput.Select(value => new Path(value)).ToList());
		}

		// OperationGraph BuildOperationGraph()
		// {
		// 	this.graphGenerator.FinalizeGraph();
		// 	return this.graph;
		// }
	};
}
