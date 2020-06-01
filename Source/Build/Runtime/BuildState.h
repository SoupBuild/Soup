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
			_parentState()
		{
		}

		/// <summary>
		/// Initializes a new instance of the BuildState class
		/// </summary>
		BuildState(ValueTable recipeState) :
			_rootOperations(),
			_activeState(),
			_parentState()
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
			return _parentState;
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

		const IList<IBuildOperation*>& GetBuildOperations() const
		{
			return _rootOperations;
		}

		/// <summary>
		/// Get a reference to the child state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		void CombineChildState(BuildState& childState)
		{
			CombineListState(childState._parentState, Extensions::ValueTableWrapper(_activeState));
		}

		void LogActive()
		{
			_activeState.Log();
		}

	private:
		/// <summary>
		/// Combine the table and list structure from the input state into the target
		/// Note: Ignores primitive value properties on a table
		/// </summary>
		void CombineListState(ValueTable& input, Extensions::ValueTableWrapper target)
		{
			// Enumerate over all property values
			// Recursively combine tables and concatenate lists
			for (auto& valueIter : input.GetValues())
			{
				auto& name = valueIter.first;
				auto& value = valueIter.second;
				switch (value.GetType())
				{
					case ValueType::Table:
						// Attempt to create the table in the target and recurse the merge
						CombineListState(
							value.AsTable(),
							target.EnsureValue(name).EnsureTable());
						break;
					case ValueType::List:
						// Attempt to create the list on the target and concatenate the input
						target.EnsureValue(name).EnsureList().Append(
							Extensions::ValueListWrapper(value.AsList()));
						break;
					default:
						// Ignore all other types
						break;
				}

			}
		}

	private:
		Extensions::BuildOperationList _rootOperations;
		ValueTable _activeState;
		ValueTable _parentState;
	};
}
