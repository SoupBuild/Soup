// <copyright file="BuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildGraphNode.h"
#include "ValueTable.h"

namespace Soup::Build
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
			_nodes(),
			_activeState(),
			_parentState()
		{
		}

		/// <summary>
		/// Initializes a new instance of the BuildState class
		/// </summary>
		BuildState(ValueTable recipeState) :
			_nodes(),
			_activeState(),
			_parentState()
		{
			// Initialize the Recipe state
			_activeState.SetValue("Recipe", Value(std::move(recipeState)));
		}

		/// <summary>
		/// Build Graph Access Methods
		/// </summary>
		OperationResult TryRegisterRootNode(IGraphNode* node) noexcept override final
		{
			try
			{
				auto internalNode = dynamic_cast<BuildGraphNode*>(node);
				if (node == nullptr)
					return -2;

				_nodes.push_back(internalNode);
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		OperationResult TryCreateNode(IGraphNode*& node) noexcept override final
		{
			try
			{
				node = new BuildGraphNode();
				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		/// <summary>
		/// Get a reference to the active state
		/// </summary>
		IValueTable& GetActiveState() noexcept override final
		{
			return _activeState;
		}

		/// <summary>
		/// Get a reference to the child state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		IValueTable& GetParentState() noexcept override final
		{
			return _parentState;
		}

		/// <summary>
		/// Log a message to the build system
		/// </summary>
		OperationResult TryLogTrace(TraceLevel level, const char* message) noexcept override final
		{
			try
			{
				switch (level)
				{
				case TraceLevel::Diagnostic:
					Log::Diag(message);
					break;
				case TraceLevel::Information:
					Log::Info(message);
					break;
				case TraceLevel::HighPriority:
					Log::HighPriority(message);
					break;
				case TraceLevel::Warning:
					Log::Warning(message);
					break;
				case TraceLevel::Error:
					Log::Error(message);
					break;
				default:
					return -2;
				}

				return 0;
			}
			catch (...)
			{
				// Unknown error
				return -1;
			}
		}

		/// <summary>
		/// Internal access to build nodes
		/// </summary>
		std::vector<Memory::Reference<BuildGraphNode>>& GetBuildNodes()
		{
			return _nodes;
		}

		const std::vector<Memory::Reference<BuildGraphNode>>& GetBuildNodes() const
		{
			return _nodes;
		}

		/// <summary>
		/// Get a reference to the child state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		void CombineChildState(BuildState& childState)
		{
			CombineListState(childState._parentState, ValueTableWrapper(_activeState));
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
		void CombineListState(ValueTable& input, ValueTableWrapper target)
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
							ValueListWrapper(value.AsList()));
						break;
					default:
						// Ignore all other types
						break;
				}

			}
		}

	private:
		std::vector<Memory::Reference<BuildGraphNode>> _nodes;
		ValueTable _activeState;
		ValueTable _parentState;
	};
}
