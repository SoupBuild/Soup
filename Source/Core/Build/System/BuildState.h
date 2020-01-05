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
			_graph(),
			_activeState(),
			_parentState()
		{
		}

		/// <summary>
		/// Build Graph Access Methods
		/// </summary>
		void AddBuildNode(std::shared_ptr<BuildGraphNode> node) noexcept override final
		{
			_graph.GetNodes().push_back(std::move(node));
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
		/// Internal access to build nodes
		/// </summary>
		std::vector<std::shared_ptr<BuildGraphNode>>& GetBuildNodes()
		{
			return _graph.GetNodes();
		}

		const std::vector<std::shared_ptr<BuildGraphNode>>& GetBuildNodes() const
		{
			return _graph.GetNodes();
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

	private:
		BuildGraph _graph;
		BuildPropertyBag _activeState;
		BuildPropertyBag _parentState;
	};
}
