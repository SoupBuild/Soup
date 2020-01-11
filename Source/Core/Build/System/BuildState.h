// <copyright file="BuildState.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "BuildPropertyBag.h"

namespace Soup::Build
{
	/// <summary>
	/// Build State Extension interface
	/// </summary>
	class BuildState : public IBuildState
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
		IPropertyBag& GetActiveState() noexcept override final
		{
			return _activeState;
		}

		/// <summary>
		/// Get a reference to the child state. All of these properties will be 
		/// moved into the active state of any parent build that has a direct reference to this build.
		/// </summary>
		IPropertyBag& GetParentState() noexcept override final
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
		void CombineChildState(const BuildState& childState)
		{
			auto& childParentState = childState._parentState;
			auto activeState = PropertyBagWrapper(_activeState);
			for (auto& propertyList : childParentState.GetPropertyLists())
			{
				activeState.AppendPropertyStringList(propertyList.first, propertyList.second.GetValues());
			}
		}

		void LogActive()
		{
			_activeState.Log();
		}

	private:
		std::vector<Memory::Reference<BuildGraphNode>> _nodes;
		BuildPropertyBag _activeState;
		BuildPropertyBag _parentState;
	};
}
