// <copyright file="GraphViewer.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Microsoft.UI.Xaml.Controls;
using Microsoft.UI.Xaml.Media;
using Microsoft.UI.Xaml.Shapes;
using System;
using System.Collections.Generic;
using Windows.UI;

namespace SoupView.View
{
    public sealed class GraphViewer : Control
    {
        private static int NodeWidth = 200;
        private static int NodeHeight = 100;

        private static int NodeSpacingHorizontal = 50;
        private static int NodeSpacingVertical = 10;

        private static int InternalPadding = 20;

        public GraphViewer()
        {
            this.DefaultStyleKey = typeof(GraphViewer);
        }

        protected override void OnApplyTemplate()
        {
            this.LayoutGraph();
            base.OnApplyTemplate();
        }

        private void LayoutGraph()
        {
            var canvas = this.GetTemplateChild("RootCanvas") as Canvas;

            var columns = new List<List<string>>()
            {
                new List<string>()
                {
                    "1",
                    "2",
                    "3",
                    "4",
                },
                new List<string>()
                {
                    "5",
                    "6",
                    "7",
                },
                new List<string>()
                {
                    "8",
                    "9",
                },
            };

            int maxHeight = 0;
            int currentOffsetX = 0;
            int currentOffsetY = 0;
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

                    var node = new Rectangle()
                    {
                        Width = NodeWidth,
                        Height = NodeHeight,
                        Fill = new SolidColorBrush() { Color = Color.FromArgb(255, 255, 4, 55) },
                    };

                    Canvas.SetLeft(node, currentOffsetX);
                    Canvas.SetTop(node, currentOffsetY);

                    canvas.Children.Add(node);

                    // Update for the next row location
                    currentOffsetY += NodeHeight;
                }

                // Update for the next column location
                currentOffsetX += NodeWidth;
            }

            // Add the final internal padding to get the total size
            canvas.Width = currentOffsetX + InternalPadding;
            canvas.Height = maxHeight + InternalPadding;
        }
    }
}
