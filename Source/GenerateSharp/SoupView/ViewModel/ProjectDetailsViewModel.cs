// <copyright file="ProductDetailsViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace SoupView.ViewModel
{
	internal class ProjectDetailsViewModel : Observable
	{
		private ObservableCollection<PropertyValue> properties = new ObservableCollection<PropertyValue>();

		public ProjectDetailsViewModel(string name, string version, string path)
		{
			properties.Clear();
			properties.Add(new PropertyValue("Name", name));
			properties.Add(new PropertyValue("Version", version));
			properties.Add(new PropertyValue("Path", path));
		}

		public IList<PropertyValue> Properties => properties;
	}
}
