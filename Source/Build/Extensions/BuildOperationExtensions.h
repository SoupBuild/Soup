// <copyright file="BuildOperationExtensions.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build::Extensions
{
	/// <summary>
	/// Build operation extension methods
	/// </summary>
	export class BuildOperationExtensions
	{
	public:
		static void AddLeafChild(
			BuildOperationWrapper parent,
			BuildOperationWrapper child)
		{
			auto parentChildrenSize = parent.GetChildList().GetSize();
			if (parentChildrenSize == 0)
			{
				// Check if this child was added through a different path
				if (parent.GetRaw() == child.GetRaw())
				{
					// The operation has been added through a different path
					return;
				}
				else
				{
					// Add the new leaf operation
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
			BuildOperationListWrapper parents,
			BuildOperationWrapper child)
		{
			auto size = parents.GetSize();
			for (size_t i = 0; i < size; i++)
			{
				AddLeafChild(parents.GetValueAt(i), child);
			}
		}

		static void AddLeafChild(
			std::vector<BuildOperationWrapper>& parents,
			BuildOperationWrapper child)
		{
			for (auto& parent : parents)
			{
				AddLeafChild(parent, child);
			}
		}

		static void AddLeafChildren(
			BuildOperationWrapper parent,
			BuildOperationListWrapper children)
		{
			auto parentChildrenSize = parent.GetChildList().GetSize();
			if (parentChildrenSize == 0)
			{
				// Add the new leaf operations
				parent.GetChildList().Append(children);
			}
			else
			{
				// Continue on to the current parents children
				AddLeafChildren(parent.GetChildList(), children);
			}
		}

		static void AddLeafChildren(
			BuildOperationWrapper parent,
			std::vector<BuildOperationWrapper>& children)
		{
			auto parentChildrenSize = parent.GetChildList().GetSize();
			if (parentChildrenSize == 0)
			{
				// Add the new leaf operations
				parent.GetChildList().Append(children);
			}
			else
			{
				// Continue on to the current parents children
				AddLeafChildren(parent.GetChildList(), children);
			}
		}

		static void AddLeafChildren(
			BuildOperationListWrapper parents,
			BuildOperationListWrapper children)
		{
			auto size = parents.GetSize();
			for (size_t i = 0; i < size; i++)
			{
				AddLeafChildren(parents.GetValueAt(i), children);
			}
		}

		static void AddLeafChildren(
			BuildOperationListWrapper parents,
			std::vector<BuildOperationWrapper>& children)
		{
			auto size = parents.GetSize();
			
			// Check if this is the same set we trying to add, from a different path
			if (size > 0 && size == children.size())
			{
				if (parents.GetValueAt(0).GetRaw() == children[0].GetRaw())
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
			std::vector<BuildOperationWrapper>& parents,
			std::vector<BuildOperationWrapper>& children)
		{
			for (auto& parent : parents)
			{
				AddLeafChildren(parent, children);
			}
		}
	};
}
