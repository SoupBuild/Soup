// <copyright file="GraphNode.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using GraphShape;
using System.Collections.Generic;

namespace Soup.View.ViewModels;

public class GraphNodeViewModel
{
	public required string Title { get; set; }
	public required string ToolTip { get; set; }
	public required uint Id { get; set; }
	public IList<uint> ChildNodes { get; init; } = new List<uint>();

	public required Point Position { get; set; }
}