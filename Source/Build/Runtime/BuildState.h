// <copyright file="BuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "ValueTable.h"

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
		BuildState() :
			_rootOperations(),
			_activeState(),
			_sharedState()
		{
		}

		/// <summary>
		/// Initializes a new instance of the BuildState class
		/// </summary>
		BuildState(ValueTable recipeState) :
			_rootOperations(),
			_activeState(),
			_sharedState()
		{
			// Initialize the Recipe state
			_activeState.SetValue("Recipe", Value(std::move(recipeState)));
		}

		/// <summary>
		/// Build Graph Access Methods
		/// </summary>
		IList<IBuildOperation*>& GetRootOperationList() noexcept override final
		{
			return _rootOperations;
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
		/// Internal access to build operations
		/// </summary>
		IList<IBuildOperation*>& GetBuildOperations()
		{
			return _rootOperations;
		}

		/// <summary>
		/// Copy the shared state from the child build into the correct dependencies table location.
		/// </summary>
		void CombineSharedState(
			const std::string& taskName,
			BuildState& childState)
		{
			auto& dependenciesTable = _activeState.EnsureValue("Dependencies").EnsureTable();
			dependenciesTable.SetValue(taskName, Value(childState._sharedState));
		}

		void LogActive()
		{
			_activeState.Log();
		}

	private:
		Extensions::BuildOperationList _rootOperations;
		ValueTable _activeState;
		ValueTable _sharedState;
	};
}
