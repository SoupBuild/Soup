// <copyright file="ProductDetailsViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Utilities;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace SoupView.ViewModel
{
	internal class ProjectDetailsViewModel : Observable
	{
		private ObservableCollection<PropertyValue> properties = new ObservableCollection<PropertyValue>();

		public ProjectDetailsViewModel(Recipe recipe, Path path)
		{
			properties.Clear();
			if (recipe is not null)
			{
				properties.Add(new PropertyValue("Name", recipe.Name));
				properties.Add(new PropertyValue("Version", recipe.Version.ToString()));
				properties.Add(new PropertyValue("Language", recipe.Language.ToString()));
			}

			properties.Add(new PropertyValue("Path", path.ToString()));
		}

		public IList<PropertyValue> Properties => properties;
	}
}
