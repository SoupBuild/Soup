// <copyright file="ProductDetailsViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Avalonia.Controls;
using Avalonia.Controls.Models.TreeDataGrid;
using System.Collections.ObjectModel;
using Path = Opal.Path;

namespace Soup.View.ViewModels;

public class ProjectDetailsViewModel : ViewModelBase
{
	private readonly ObservableCollection<PropertyValueViewModel> properties = [];

	public ProjectDetailsViewModel(string name, Path path, string? owner)
	{
		Name = name;

		this.properties.Clear();

		this.properties.Add(new PropertyValueViewModel("Name", name));
		this.properties.Add(new PropertyValueViewModel("Path", path.ToString()));
		this.properties.Add(new PropertyValueViewModel("Owner", owner?.ToString()));

		Path = path;
		Owner = owner;

		Properties = new FlatTreeDataGridSource<PropertyValueViewModel>(this.properties)
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

	public string? Owner { get; private set; }
}