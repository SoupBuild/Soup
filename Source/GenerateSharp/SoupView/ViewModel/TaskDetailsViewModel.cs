// <copyright file="TaskDetailsViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace SoupView.ViewModel
{
	internal class TaskDetailsViewModel : Observable
	{
		private ObservableCollection<PropertyValue> properties = new ObservableCollection<PropertyValue>();

		public TaskDetailsViewModel(string name, IValueTable taskInfo)
		{
			properties.Clear();
			properties.Add(new PropertyValue("Name", name));
		}

		public IList<PropertyValue> Properties => properties;
	}
}
