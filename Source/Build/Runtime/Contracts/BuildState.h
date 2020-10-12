// <copyright file="BuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ValueTable.h"
#include "OperationGraph/OperationGraphGenerator.h"

namespace Soup::Build::Runtime
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	export class BuildState : public IBuildState
	{
	public:
		/// <summary>
		/// Initializes a new instance of the BuildState class
		/// </summary>
		BuildState(ValueTable activeState, FileSystemState& fileSystemState) :
			_activeState(std::move(activeState)),
			_sharedState(),
			_graphGenerator(fileSystemState)
		{
		}

		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		IValueTable& GetActiveState() noexcept override final
		{
			return _activeState;
		}

		/// <summary>
		/// Get a reference to the shared state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		IValueTable& GetSharedState() noexcept override final
		{
			return _sharedState;
		}

		/// <summary>
		/// Create a build operation
		/// </summary>
		ApiCallResult TryCreateOperation(
			const char* title,
			const char* executable,
			const char* arguments,
			const char* workingDirectory,
			IReadOnlyList<const char*>& declaredInput,
			IReadOnlyList<const char*>& declaredOutput) noexcept override final
		{
			try
			{
				_graphGenerator.CreateOperation(
					title,
					Path(executable),
					arguments,
					Path(workingDirectory),
					Utilities::ReadOnlyStringListWrapper(declaredInput).CopyAsPathVector(),
					Utilities::ReadOnlyStringListWrapper(declaredOutput).CopyAsPathVector());
				return ApiCallResult::Success;
			}
			catch (...)
			{
				// Unknown error
				return ApiCallResult::Error;
			}
		}

		/// <summary>
		/// Log a message to the build system
		/// </summary>
		ApiCallResult TryLogTrace(TraceLevel level, const char* message) noexcept override final
		{
			try
			{
				switch (level)
				{
				case TraceLevel::Error:
					Log::Error(message);
					break;
				case TraceLevel::Warning:
					Log::Warning(message);
					break;
				case TraceLevel::HighPriority:
					Log::HighPriority(message);
					break;
				case TraceLevel::Information:
					Log::Info(message);
					break;
				case TraceLevel::Debug:
					Log::Diag(message);
					break;
				default:
					return ApiCallResult::Error;
				}

				return ApiCallResult::Success;
			}
			catch (...)
			{
				// Unknown error
				return ApiCallResult::Error;
			}
		}

		/// <summary>
		/// Pull out the shared state
		/// </summary>
		ValueTable RetrieveSharedState() noexcept
		{
			return std::move(_sharedState);
		}

		OperationGraph BuildOperationGraph()
		{
			return _graphGenerator.BuildGraph();
		}

		void LogActive()
		{
			_activeState.Log();
		}

	private:
		ValueTable _activeState;
		ValueTable _sharedState;
		OperationGraphGenerator _graphGenerator;
	};
}
