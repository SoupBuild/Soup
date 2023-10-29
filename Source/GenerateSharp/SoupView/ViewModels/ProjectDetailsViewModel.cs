// <copyright file="ProductDetailsViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Avalonia.Controls;
using Avalonia.Controls.Models.TreeDataGrid;
using Opal;
using Soup.Build.Utilities;
using System.Collections.ObjectModel;

namespace Soup.View.ViewModels
{
	public class ProjectDetailsViewModel : ViewModelBase
	{
		private ObservableCollection<PropertyValueViewModel> properties = new ObservableCollection<PropertyValueViewModel>();

		public ProjectDetailsViewModel(string name, Path path)
		{
			Name = name;

			properties.Clear();

			properties.Add(new PropertyValueViewModel("Name", name));
			properties.Add(new PropertyValueViewModel("Path", path.ToString()));

			Path = path;

			Properties = new FlatTreeDataGridSource<PropertyValueViewModel>(properties)
			{
				Columns =
				{
					new TextColumn<PropertyValueViewModel, string>(
						"Name",
						x => x.Name),
					new TextColumn<PropertyValueViewModel, string>(
						"Value",
						x => x.Value),
				},
			};
		}

		public FlatTreeDataGridSource<PropertyValueViewModel>? Properties { get; }

		public string? Name { get; private set; }

		public Path Path { get; private set; }
	}
}
