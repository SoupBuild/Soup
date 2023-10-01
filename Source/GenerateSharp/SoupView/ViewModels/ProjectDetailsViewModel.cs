// <copyright file="ProductDetailsViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Utilities;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace Soup.View.ViewModels
{
	public class ProjectDetailsViewModel : ViewModelBase
	{
		private ObservableCollection<PropertyValueViewModel> properties = new ObservableCollection<PropertyValueViewModel>();

		public ProjectDetailsViewModel(Recipe? recipe, Path path)
		{
			properties.Clear();
			if (recipe is not null)
			{
				properties.Add(new PropertyValueViewModel("Name", recipe.Name));
				properties.Add(new PropertyValueViewModel("Version", recipe.Version.ToString()));
				properties.Add(new PropertyValueViewModel("Language", recipe.Language.ToString()));

				Name = recipe.Name;
			}

			properties.Add(new PropertyValueViewModel("Path", path.ToString()));

			Path = path;
		}

		public IList<PropertyValueViewModel> Properties => properties;

		public string? Name { get; private set; }

		public Path Path { get; private set; }
	}
}
