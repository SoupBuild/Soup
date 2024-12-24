// <copyright file="PropertyValueViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.ObjectModel;

namespace Soup.View.ViewModels;

public class PropertyValueViewModel
{
	public PropertyValueViewModel(string name, string? value)
	{
		this.Name = name;
		this.Value = value;
	}

	public string Name { get; set; }
	public string? Value { get; set; }

	public ObservableCollection<PropertyValueViewModel>? Children { get; init; }
}