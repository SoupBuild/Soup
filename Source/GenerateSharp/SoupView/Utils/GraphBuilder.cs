// <copyright file="GraphBuilder.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using GraphShape;
using GraphShape.Algorithms.Layout;
using QuikGraph;
using System.Collections.Generic;
using System.Linq;

namespace Soup.View;

public static class GraphBuilder
{
	public static IDictionary<TNode, Point> BuildDirectedAcyclicGraphView<TNode>(
		IEnumerable<(TNode Node, IEnumerable<TNode> Children)> graph,
		Size nodeSize) where TNode : class
	{
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
	}
}
