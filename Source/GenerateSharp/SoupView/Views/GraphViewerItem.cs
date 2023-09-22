// <copyright file="GraphViewerItem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Avalonia;
using Avalonia.Controls.Primitives;

namespace Soup.View.Views
{
	public sealed class GraphViewerItem : TemplatedControl
	{
		/// <summary>
		/// Identifies the <see cref="Title"/> property.
		/// </summary>
		public static readonly StyledProperty<string> TitleProperty =
			AvaloniaProperty.Register<GraphViewerItem, string>(nameof(Title));

		public GraphViewerItem()
		{
		}

		/// <summary>
		/// Gets or sets the title
		/// </summary>
		public string Title
		{
			get => GetValue(TitleProperty);
			set => SetValue(TitleProperty, value);
		}
	}
}
