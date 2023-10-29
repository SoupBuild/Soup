// <copyright file="GraphBuilder.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.View;

public static class GraphBuilder
{
	public static IList<IList<uint>> BuildDirectedAcyclicGraphView(IDictionary<uint, IList<uint>> graph, IList<uint> rootNodes)
	{
		var activeGraph = new List<IList<uint>>();
		var knownIds = new HashSet<uint>();
		BuildGraphColumn(
			graph,
			rootNodes,
			activeGraph,
			knownIds);

		return activeGraph;
	}

	private static void BuildGraphColumn(
		IDictionary<uint, IList<uint>> graph,
		IList<uint> activeNodes,
		IList<IList<uint>> activeGraph,
		ISet<uint> knownIds)
	{
		// Build up the total set of nodes in the next level
		var nextIds = new List<uint>();
		foreach (var id in activeNodes)
		{
			foreach (var childId in graph[id])
			{
				nextIds.Add(childId);
			}
		}

		// Find the deepest level first
		if (nextIds.Count > 0)
		{
			BuildGraphColumn(
				graph,
				nextIds,
				activeGraph,
				knownIds);
		}

		// Build up all the nodes at this level that have not already been added
		var column = new List<uint>();
		foreach (var id in activeNodes)
		{
			if (!knownIds.Contains(id))
			{
				knownIds.Add(id);
				column.Add(id);
			}
		}

		// Add the new column at the start
		activeGraph.Insert(0, column);
	}
}
