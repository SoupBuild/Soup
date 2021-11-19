// <copyright file="GraphViewerItem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;

namespace SoupView.View
{
    public sealed class GraphViewerItem : Control
    {
        /// <summary>
        /// Identifies the <see cref="Title"/> dependency property.
        /// </summary>
        public static readonly DependencyProperty TitleProperty = DependencyProperty.Register(
            nameof(Title),
            typeof(string),
            typeof(GraphViewerItem),
            new PropertyMetadata(string.Empty));

        public GraphViewerItem()
        {
            this.DefaultStyleKey = typeof(GraphViewerItem);
        }

        private readonly Canvas containerCanvas = new Canvas();

        /// <summary>
        /// Gets or sets the title
        /// </summary>
        public string Title
        {
            get { return (string)GetValue(TitleProperty); }
            set { SetValue(TitleProperty, value); }
        }
    }
}
