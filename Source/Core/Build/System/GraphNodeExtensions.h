// <copyright file="GraphNodeExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "StringList.h"

namespace Soup::Build
{
	/// <summary>
	/// Build graph node extension methods
	/// </summary>
	export class GraphNodeExtensions
	{
	public:
		static void AddLeafChild(
			GraphNodeWrapper parent,
			GraphNodeWrapper child)
		{
			auto parentChildrenSize = parent.GetChildList().GetSize();
			if (parentChildrenSize == 0)
			{
				// Check if this child was added through a different path
				if (parent.GetId() == child.GetId())
				{
					// The node has been added through a different path
					return;
				}
				else
				{
					// Add the new leaf node
					parent.GetChildList().Append(child);
				}
			}
			else
			{
				// Continue on to the current parents children
				AddLeafChild(parent.GetChildList(), child);
			}
		}

		static void AddLeafChild(
			GraphNodeListWrapper parents,
			GraphNodeWrapper child)
		{
			auto size = parents.GetSize();
			for (size_t i = 0; i < size; i++)
			{
				AddLeafChild(parents.GetValueAt(i), child);
			}
		}

		static void AddLeafChild(
			std::vector<GraphNodeWrapper>& parents,
			GraphNodeWrapper child)
		{
			for (auto& parent : parents)
			{
				AddLeafChild(parent, child);
			}
		}

		static void AddLeafChildren(
			GraphNodeWrapper parent,
			GraphNodeListWrapper children)
		{
			auto parentChildrenSize = parent.GetChildList().GetSize();
			if (parentChildrenSize == 0)
			{
				// Add the new leaf nodes
				parent.GetChildList().Append(children);
			}
			else
			{
				// Continue on to the current parents children
				AddLeafChildren(parent.GetChildList(), children);
			}
		}

		static void AddLeafChildren(
			GraphNodeWrapper parent,
			std::vector<GraphNodeWrapper>& children)
		{
			auto parentChildrenSize = parent.GetChildList().GetSize();
			if (parentChildrenSize == 0)
			{
				// Add the new leaf nodes
				parent.GetChildList().Append(children);
			}
			else
			{
				// Continue on to the current parents children
				AddLeafChildren(parent.GetChildList(), children);
			}
		}

		static void AddLeafChildren(
			GraphNodeListWrapper parents,
			GraphNodeListWrapper children)
		{
			auto size = parents.GetSize();
			for (size_t i = 0; i < size; i++)
			{
				AddLeafChildren(parents.GetValueAt(i), children);
			}
		}

		static void AddLeafChildren(
			GraphNodeListWrapper parents,
			std::vector<GraphNodeWrapper>& children)
		{
			auto size = parents.GetSize();
			
			// Check if this is the same set we trying to add, from a different path
			if (size > 0 && size == children.size())
			{
				if (parents.GetValueAt(0).GetId() == children[0].GetId())
				{
					// The children match the parent set
					return;
				}
			}

			for (size_t i = 0; i < size; i++)
			{
				AddLeafChildren(parents.GetValueAt(i), children);
			}
		}

		static void AddLeafChildren(
			std::vector<GraphNodeWrapper>& parents,
			std::vector<GraphNodeWrapper>& children)
		{
			for (auto& parent : parents)
			{
				AddLeafChildren(parent, children);
			}
		}
	};
}
