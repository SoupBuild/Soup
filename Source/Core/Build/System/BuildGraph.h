// <copyright file="BuildGraph.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once

namespace Soup::Build
{
	/// <summary>
	/// A directed acyclic graph (DAG) that represents the entire build
	/// </summary>
	export class BuildGraph
	{
	public:
		BuildGraph() :
			_nodes()
		{
		}

		std::vector<std::shared_ptr<BuildGraphNode>>& GetNodes()
		{
			return _nodes;
		}

		const std::vector<std::shared_ptr<BuildGraphNode>>& GetNodes() const
		{
			return _nodes;
		}

	private:
		std::vector<std::shared_ptr<BuildGraphNode>> _nodes;
	};
}
