// <copyright file="GraphViewer.axaml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.Primitives;
using Avalonia.Controls.Shapes;
using Avalonia.Controls.Templates;
using Avalonia.Interactivity;
using Avalonia.Media;
using Soup.View.ViewModels;

namespace Soup.View.Views
{
	public sealed class GraphViewer : TemplatedControl
	{
		private static int NodeWidth = 200;
		private static int NodeHeight = 50;

		private static int NodeSpacingHorizontal = 80;
		private static int NodeSpacingVertical = 50;

		private static int InternalPadding = 20;

		private Canvas? root;
		private IDictionary<uint, GraphViewerItem> itemLookup = new Dictionary<uint, GraphViewerItem>();

		/// <summary>
		/// Identifies the <see cref="Graph"/> property.
		/// </summary>
		public static readonly StyledProperty<IList<IList<GraphNodeViewModel>>> GraphProperty =
			AvaloniaProperty.Register<GraphViewer, IList<IList<GraphNodeViewModel>>>(nameof(Graph));

		/// <summary>
		/// Identifies the <see cref="SelectedNode"/> property.
		/// </summary>
		/// 
		public static readonly StyledProperty<GraphNodeViewModel?> SelectedNodeProperty =
			AvaloniaProperty.Register<GraphViewer, GraphNodeViewModel?>(nameof(SelectedNode));

		public GraphViewer()
		{
			this.PropertyChanged += GraphViewer_PropertyChanged;
			this.SizeChanged += GraphViewer_SizeChanged;
		}

		/// <summary>
		/// Gets or sets the Graph
		/// </summary>
		public IList<IList<GraphNodeViewModel>> Graph
		{
			get => GetValue(GraphProperty);
			set => SetValue(GraphProperty, value);
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
				case nameof(Graph):
					((GraphViewer?)sender)?.LayoutGraph();
					break;
				case nameof(SelectedNode):
					// Deselect the previous item
					if (e.OldValue is GraphNodeViewModel oldNode &&
						itemLookup.TryGetValue(oldNode.Id, out var oldItem))
					{
						oldItem.IsSelected = false;
					}

					// Select the new one
					if (e.NewValue is GraphNodeViewModel newNode &&
						itemLookup.TryGetValue(newNode.Id, out var newItem))
					{
						newItem.IsSelected = true;
					}

					break;
			}
		}

		private void GraphViewer_SizeChanged(object? sender, SizeChangedEventArgs e)
		{
			this.LayoutGraph();
		}

		protected override void OnApplyTemplate(TemplateAppliedEventArgs e)
		{
			this.LayoutGraph();

			root = e.NameScope.Find<Canvas>("RootCanvas");

			base.OnApplyTemplate(e);
		}

		private void LayoutGraph()
		{
			if (root is null)
				return;

			root.Children.Clear();

			if (this.Graph is null)
				return;

			int maxHeight = 0;
			int currentOffsetX = 0;
			int currentOffsetY;
			var nodeState = new Dictionary<uint, (GraphViewerItem Item, Point InConnect, Point OutConnect)>();
			itemLookup.Clear();
			foreach (var column in this.Graph)
			{
				// Reset vertical offset for each column
				currentOffsetY = 0;

				// Update X offset for the current column location
				if (currentOffsetX == 0)
					currentOffsetX += InternalPadding;
				else
					currentOffsetX += NodeSpacingHorizontal;

				foreach (var value in column)
				{
					// Update Y for the current row location
					if (currentOffsetY == 0)
						currentOffsetY += InternalPadding;
					else
						currentOffsetY += NodeSpacingVertical;

					if (value is not null)
					{
						var node = new GraphViewerItem()
						{
							Title = value.Title,
							ToolTip = value.ToolTip,
							Width = NodeWidth,
							Height = NodeHeight,
						};
						node.DataContext = value;
						node.Click += Node_Click;

						Canvas.SetLeft(node, currentOffsetX);
						Canvas.SetTop(node, currentOffsetY);

						root.Children.Add(node);

						// Save the node state
						var inConnect = new Point(currentOffsetX, currentOffsetY + (NodeHeight / 2));
						var outConnect = new Point(currentOffsetX + NodeWidth, currentOffsetY + (NodeHeight / 2));
						nodeState.Add(value.Id, (node, inConnect, outConnect));

						itemLookup.Add(value.Id, node);
					}

					// Update for the next row location
					currentOffsetY += NodeHeight;
				}

				// Update for the next column location
				currentOffsetX += NodeWidth;

				// Check the max height
				maxHeight = Math.Max(maxHeight, currentOffsetY);
			}

			// Connect all the known nodes
			foreach (var column in Graph)
			{
				foreach (var value in column)
				{
					if (value is not null)
					{
						var startNode = nodeState[value.Id];
						foreach (var child in value.ChildNodes)
						{
							var endNode = nodeState[child];
							var path = ConnectNodes(startNode.OutConnect, endNode.InConnect);
							root.Children.Add(path);
						}
					}
				}
			}

			// Add the final internal padding to get the total size
			root.Width = currentOffsetX + InternalPadding;
			root.Height = maxHeight + InternalPadding;

			// Ensure the current selected item is notified
			if (SelectedNode is not null && itemLookup.TryGetValue(SelectedNode.Id, out var selectedItem))
			{
				selectedItem.IsSelected = true;
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

		private Path ConnectNodes(Point start, Point end)
		{
			var control1 = new Point(
				start.X + (NodeSpacingHorizontal / 2),
				start.Y);
			var control2 = new Point(
				end.X - (NodeSpacingHorizontal / 2),
				end.Y);

			return CreateBezier(start, control1, control2, end);
		}

		private Path CreateBezier(Point start, Point control1, Point control2, Point end)
		{
			var path = new Path()
			{
				Data = new PathGeometry()
				{
					Figures = new PathFigures()
					{
						new PathFigure()
						{
							IsClosed = false,
							StartPoint = start,
							Segments = new PathSegments()
							{
								new BezierSegment()
								{
									Point1 = control1,
									Point2 = control2,
									Point3 = end,
								}
							},
						},
					},
				},
			};

			path.Classes.Add("graphViewerPath");

			return path;
		}
	}
}
