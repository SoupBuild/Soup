// <copyright file="GenerateState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "OperationGraphGenerator.h"

namespace Soup::Core::Generate
{
	/// <summary>
	/// Generate State used to track all context for state and operation graph creation
	/// </summary>
	class GenerateState
	{
	private:
		ValueTable _globalState;
		ValueTable _activeState;
		ValueTable _sharedState;
		ValueTable _generateInfo;
		OperationGraphGenerator _graphGenerator;

	public:
		/// <summary>
		/// Initializes a new instance of the GenerateState class
		/// </summary>
		 GenerateState(
			ValueTable globalState,
			FileSystemState& fileSystemState,
			std::vector<Path> readAccessList,
			std::vector<Path> writeAccessList) :
			_globalState(std::move(globalState)),
			_activeState(),
			_sharedState(),
			_generateInfo(),
			_graphGenerator(fileSystemState, std::move(readAccessList), std::move(writeAccessList))
		{
		}

		/// <summary>
		/// Get a reference to the global state
		/// </summary>
		const ValueTable& GetGlobalState() const
		{
			return _globalState;
		}

		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		const ValueTable& GetActiveState() const
		{
			return _activeState;
		}

		/// <summary>
		/// Get a reference to the shared state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		const ValueTable& GetSharedState() const
		{
			return _sharedState;
		}

		/// <summary>
		/// Get a reference to the generate info table. This is a collection of runtime information stored
		/// for easy debugging of the intermediate state during generate.
		/// </summary>
		const ValueTable& GetGenerateInfo() const
		{
			return _generateInfo;
		}
		void SetGenerateInfo(ValueTable value)
		{
			_generateInfo = std::move(value);
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
			auto declaredInputPaths = std::vector<Path>();
			for (auto& value : declaredInput)
				declaredInputPaths.push_back(Path(std::move(value)));

			auto declaredOutputPaths = std::vector<Path>();
			for (auto& value : declaredOutput)
				declaredOutputPaths.push_back(Path(std::move(value)));

			_graphGenerator.CreateOperation(
				std::move(title),
				Path(std::move(executable)),
				std::move(arguments),
				Path(std::move(workingDirectory)),
				std::move(declaredInputPaths),
				std::move(declaredOutputPaths));
		}

		void Update(ValueTable activeState, ValueTable sharedState)
		{
			_activeState = std::move(activeState);
			_sharedState = std::move(sharedState);
		}

		OperationGraph BuildOperationGraph()
		{
			return _graphGenerator.FinalizeGraph();
		}
	};
}
