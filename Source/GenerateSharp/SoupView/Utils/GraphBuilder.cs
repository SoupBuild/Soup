// <copyright file="GraphBuilder.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Linq;
using GraphShape;
using GraphShape.Algorithms.Layout;
using QuikGraph;

namespace Soup.View;

public static class GraphBuilder
{

	public static IDictionary<TNode, Point> BuildDirectedAcyclicGraphView<TNode>(IEnumerable<(TNode Node, IEnumerable<TNode> Children)> graph, Size nodeSize) where TNode : class
	{
#if USE_HACKY_DESIGN
		var activeGraph = new List<IList<uint>>();
		var knownIds = new HashSet<uint>();
		BuildGraphColumn(
			graph,
			rootNodes,
			activeGraph,
			knownIds);

		return activeGraph;
#else
		var positions = new Dictionary<TNode, Point>();
		var sizes = graph.ToDictionary(
			value => value.Node,
			value => nodeSize);
		var graph2 = new BidirectionalGraph<TNode, Edge<TNode>>();

		graph2.AddVertexRange(graph.Select(value => value.Node));
		graph2.AddEdgeRange(graph.SelectMany(value => value.Children.Select(child => new Edge<TNode>(value.Node, child))));

		var simpleContext = new LayoutContext<TNode, Edge<TNode>, BidirectionalGraph<TNode, Edge<TNode>>>(
			graph2,
			positions,
			sizes,
			LayoutMode.Simple);

		var factory = new StandardLayoutAlgorithmFactory<TNode, Edge<TNode>, BidirectionalGraph<TNode, Edge<TNode>>>();
		var layoutAlgorithm = factory.CreateAlgorithm(
			"Sugiyama",
			simpleContext,
			new SugiyamaLayoutParameters()
			{
				Direction = LayoutDirection.TopToBottom,
				OptimizeWidth = false,
				LayerGap = 30,
				SliceGap = 30,
			});

		layoutAlgorithm.Compute();

		return layoutAlgorithm.VerticesPositions;
#endif
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
