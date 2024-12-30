// <copyright file="GraphViewer.axaml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.Primitives;
using Avalonia.Interactivity;
using Avalonia.Media;
using Soup.View.ViewModels;
using System.Collections.Generic;
using System.Linq;
using Path = Avalonia.Controls.Shapes.Path;

namespace Soup.View.Views;

public sealed class GraphViewer : TemplatedControl
{
	public static int NodeWidth => 200;
	public static int NodeHeight => 50;

	private static int NodeSpacingVertical => 50;

	private static int InternalPadding => 20;

	private ScrollViewer? scroller;
	private Canvas? root;
	private readonly Dictionary<uint, GraphViewerItem> itemLookup = [];

	/// <summary>
	/// Identifies the <see cref="Graph"/> property.
	/// </summary>
	public static readonly StyledProperty<IList<GraphNodeViewModel>> GraphProperty =
		AvaloniaProperty.Register<GraphViewer, IList<GraphNodeViewModel>>(nameof(Graph));

	/// <summary>
	/// Identifies the <see cref="SelectedNode"/> property.
	/// </summary>
	/// 
	public static readonly StyledProperty<GraphNodeViewModel?> SelectedNodeProperty =
		AvaloniaProperty.Register<GraphViewer, GraphNodeViewModel?>(nameof(SelectedNode));

	public GraphViewer()
	{
		PropertyChanged += GraphViewer_PropertyChanged;
		SizeChanged += GraphViewer_SizeChanged;
	}

	/// <summary>
	/// Gets or sets the Graph
	/// </summary>
	public IList<GraphNodeViewModel> Graph
	{
		get => GetValue(GraphProperty);
		private set => SetValue(GraphProperty, value);
	}

	/// <summary>
	/// Gets or sets the Selected Node
	/// </summary>
	public GraphNodeViewModel? SelectedNode
	{
		get => GetValue(SelectedNodeProperty);
		set => SetValue(SelectedNodeProperty, value);
	}

	private void GraphViewer_PropertyChanged(object? sender, AvaloniaPropertyChangedEventArgs e)
	{
		switch (e.Property.Name)
		{
			case nameof(this.Graph):
				((GraphViewer?)sender)?.LayoutGraph();
				break;
			case nameof(this.SelectedNode):
				// Deselect the previous item
				if (e.OldValue is GraphNodeViewModel oldNode &&
					this.itemLookup.TryGetValue(oldNode.Id, out var oldItem))
				{
					oldItem.IsSelected = false;
				}

				// Select the new one
				if (e.NewValue is GraphNodeViewModel newNode &&
					this.itemLookup.TryGetValue(newNode.Id, out var newItem))
				{
					newItem.IsSelected = true;
				}

				break;
			default:
				break;
		}
	}

	private void GraphViewer_SizeChanged(object? sender, SizeChangedEventArgs e)
	{
		LayoutGraph();
	}

	protected override void OnApplyTemplate(TemplateAppliedEventArgs e)
	{
		LayoutGraph();

		this.scroller = e.NameScope.Find<ScrollViewer>("Scroller");
		this.root = e.NameScope.Find<Canvas>("RootCanvas");

		base.OnApplyTemplate(e);
	}

	private void LayoutGraph()
	{
		if (this.root is null)
			return;

		this.root.Children.Clear();

		if (this.Graph is null)
			return;

		var minPosition = new GraphShape.Point(
			this.Graph.Min(node => node.Position.X),
			this.Graph.Min(node => node.Position.Y));

		var maxPosition = new GraphShape.Point(
			this.Graph.Max(node => node.Position.X),
			this.Graph.Max(node => node.Position.Y));

		var size = new GraphShape.Size(
			maxPosition.X - minPosition.X + NodeWidth + InternalPadding + InternalPadding,
			maxPosition.Y - minPosition.Y + NodeHeight + InternalPadding + InternalPadding);

		var nodeState = new Dictionary<uint, (GraphViewerItem Item, Point InConnect, Point OutConnect)>();
		this.itemLookup.Clear();

		foreach (var node in this.Graph)
		{
			var nodeItem = new GraphViewerItem()
			{
				Title = node.Title,
				ToolTip = node.ToolTip,
				Width = NodeWidth,
				Height = NodeHeight,
				DataContext = node
			};
			nodeItem.Click += Node_Click;

			var normalizedPosition = node.Position - minPosition;
			normalizedPosition.X += InternalPadding;
			normalizedPosition.Y += InternalPadding;

			Canvas.SetLeft(nodeItem, normalizedPosition.X);
			Canvas.SetTop(nodeItem, normalizedPosition.Y);

			this.root.Children.Add(nodeItem);

			// Save the node state
			var inConnect = new Point(normalizedPosition.X + (NodeWidth / 2), normalizedPosition.Y);
			var outConnect = new Point(normalizedPosition.X + +(NodeWidth / 2), normalizedPosition.Y + NodeHeight);
			nodeState.Add(node.Id, (nodeItem, inConnect, outConnect));

			this.itemLookup.Add(node.Id, nodeItem);
		}

		// Connect all the known nodes
		foreach (var node in this.Graph)
		{
			var startNode = nodeState[node.Id];
			foreach (var child in node.ChildNodes)
			{
				var (item, inConnect, outConnect) = nodeState[child];
				var path = ConnectNodes(startNode.OutConnect, inConnect);
				this.root.Children.Add(path);
			}
		}

		// Add the final internal padding to get the total size
		this.root.Width = size.Width;
		this.root.Height = size.Height;

		// Ensure the current selected item is notified
		if (this.SelectedNode is not null && this.itemLookup.TryGetValue(this.SelectedNode.Id, out var selectedItem))
		{
			selectedItem.IsSelected = true;
		}

		// Center the view
		if (this.scroller is not null)
		{
			// TODO: Pick a better X offset
			// scroller.Offset = new Vector(size.Width / 3, scroller.Offset.Y);
		}
	}

	private void Node_Click(object? sender, RoutedEventArgs e)
	{
		if (sender is not null)
		{
			var node = (GraphViewerItem)sender;
			this.SelectedNode = (GraphNodeViewModel?)node.DataContext;
		}
	}

	private static Path ConnectNodes(Point start, Point end)
	{
		var control1 = new Point(
			start.X,
			start.Y + (NodeSpacingVertical / 2));
		var control2 = new Point(
			end.X,
			end.Y - (NodeSpacingVertical / 2));

		return CreateBezier(start, control1, control2, end);
	}

	private static Path CreateBezier(Point start, Point control1, Point control2, Point end)
	{
		var path = new Path()
		{
			Data = new PathGeometry()
			{
				Figures =
					[
						new PathFigure()
						{
							IsClosed = false,
							StartPoint = start,
							Segments =
							[
								new BezierSegment()
								{
									Point1 = control1,
									Point2 = control2,
									Point3 = end,
								}
							],
						},
					],
			},
		};

		path.Classes.Add("graphViewerPath");

		return path;
	}
}