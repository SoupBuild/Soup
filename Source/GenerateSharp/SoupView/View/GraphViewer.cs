// <copyright file="GraphViewer.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Shapes;
using Windows.Foundation;
using Windows.UI;

namespace SoupView.View
{
    public class GraphNode
    {
        public GraphNode(string title, int id)
        {
            this.Title = title;
            this.Id = id;
        }

        public string Title { get; init; } = string.Empty;
        public int Id { get; init; } = -1;
        public IList<int> ChildNodes { get; init;} = new List<int>();
    }

    public sealed class GraphViewer : Control
    {
        private static int NodeWidth = 120;
        private static int NodeHeight = 40;

        private static int NodeSpacingHorizontal = 80;
        private static int NodeSpacingVertical = 50;

        private static int InternalPadding = 20;

        public GraphViewer()
        {
            this.DefaultStyleKey = typeof(GraphViewer);
            this.SizeChanged += GraphViewer_SizeChanged;
        }

        private void GraphViewer_SizeChanged(object sender, Microsoft.UI.Xaml.SizeChangedEventArgs e)
        {
            this.LayoutGraph();
        }

        protected override void OnApplyTemplate()
        {
            this.LayoutGraph();
            base.OnApplyTemplate();
        }

        private void LayoutGraph()
        {
            var canvas = this.GetTemplateChild("RootCanvas") as Canvas;

            var columns = new List<List<GraphNode>>()
            {
                new List<GraphNode>()
                {
                    new GraphNode("1", 1)
                    {
                        ChildNodes = new List<int>() { 5, 6, },
                    },
                    new GraphNode("2", 2)
                    {
                        ChildNodes = new List<int>() { 6, },
                    },
                    new GraphNode("3", 3)
                    {
                        ChildNodes = new List<int>() { 6, },
                    },
                    new GraphNode("4", 4)
                    {
                        ChildNodes = new List<int>() { 6, },
                    },
                },
                new List<GraphNode>()
                {
                    new GraphNode("5", 5)
                    {
                        ChildNodes = new List<int>() { 8, },
                    },
                    new GraphNode("6", 6)
                    {
                        ChildNodes = new List<int>() { 8, },
                    },
                    new GraphNode("7", 7)
                    {
                        ChildNodes = new List<int>() { 9, },
                    },
                    null,
                },
                new List<GraphNode>()
                {
                    null,
                    new GraphNode("8", 8),
                    new GraphNode("9", 9),
                    null,
                },
            };

            canvas.Children.Clear();

            int maxHeight = 0;
            int currentOffsetX = 0;
            int currentOffsetY = 0;
            var nodeState = new Dictionary<int, (GraphViewerItem Item, Point InConnect, Point OutConnect)>();
            foreach (var column in columns)
            {
                // Reset vertical offset for each column
                maxHeight = Math.Max(maxHeight, currentOffsetY);
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
                            Width = NodeWidth,
                            Height = NodeHeight,
                        };

                        Canvas.SetLeft(node, currentOffsetX);
                        Canvas.SetTop(node, currentOffsetY);

                        canvas.Children.Add(node);

                        // Save the node state
                        var inConnect = new Point(currentOffsetX, currentOffsetY + (NodeHeight / 2));
                        var outConnect = new Point(currentOffsetX + NodeWidth, currentOffsetY + (NodeHeight / 2));
                        nodeState.Add(value.Id, (node, inConnect, outConnect));
                    }

                    // Update for the next row location
                    currentOffsetY += NodeHeight;
                }

                // Update for the next column location
                currentOffsetX += NodeWidth;
            }

            // Connect all the known nodes
            foreach (var column in columns)
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
                            canvas.Children.Add(path);
                        }
                    }
                }
            }

            // Add the final internal padding to get the total size
            canvas.Width = currentOffsetX + InternalPadding;
            canvas.Height = maxHeight + InternalPadding;
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
                Stroke = new SolidColorBrush((Color)Resources["SystemBaseLowColor"]),
                StrokeThickness = 1,
                Data = new PathGeometry()
                {
                    Figures = new PathFigureCollection()
                    {
                        new PathFigure()
                        {
                            StartPoint = start,
                            Segments = new PathSegmentCollection()
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

            return path;
        }
    }
}
