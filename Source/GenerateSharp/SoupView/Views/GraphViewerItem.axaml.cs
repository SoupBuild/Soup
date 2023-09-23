// <copyright file="GraphViewerItem.axaml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Avalonia;
using Avalonia.Controls;

namespace Soup.View.Views
{
	public sealed class GraphViewerItem : Button
	{
		/// <summary>
		/// Identifies the <see cref="Title"/> property.
		/// </summary>
		public static readonly StyledProperty<string> TitleProperty =
			AvaloniaProperty.Register<GraphViewerItem, string>(nameof(Title));

		/// <summary>
		/// Identifies the <see cref="ToolTip"/> property.
		/// </summary>
		public static readonly StyledProperty<string> ToolTipProperty =
			AvaloniaProperty.Register<GraphViewerItem, string>(nameof(ToolTip));

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

		/// <summary>
		/// Gets or sets the title
		/// </summary>
		public string ToolTip
		{
			get => GetValue(ToolTipProperty);
			set => SetValue(ToolTipProperty, value);
		}
	}
}
