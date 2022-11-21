// <copyright file="ValueTableItemTemplateSelector.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using SoupView.ViewModel;

namespace SoupView.View
{
	internal class ValueTableItemTemplateSelector : DataTemplateSelector
	{
		public DataTemplate? TableTemplate { get; set; }
		public DataTemplate? ListTemplate { get; set; }
		public DataTemplate? ValueTemplate { get; set; }

		protected override DataTemplate? SelectTemplateCore(object item)
		{
			var explorerItem = (ValueTableItemViewModel)item;
			switch (explorerItem.Type)
			{
				case ValueTableItemType.Table:
					return TableTemplate;
				case ValueTableItemType.List:
					return ListTemplate;
				case ValueTableItemType.Value:
					return ValueTemplate;
				default:
					throw new InvalidOperationException("Unknown ValueTableItemType");
			}
		}
	}
}
