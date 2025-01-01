// <copyright file="ValueTableItemViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using ReactiveUI;
using System.Collections.ObjectModel;

namespace Soup.View.ViewModels;

public enum ValueTableItemType
{
	Table,
	List,
	Value,
};

public class ValueTableItemViewModel : ViewModelBase
{
	public string Title { get; set; }

	public ValueTableItemType Type { get; set; }
	public ObservableCollection<ValueTableItemViewModel> Children { get; init; } = [];

	private bool isExpanded;
	public bool IsExpanded
	{
		get => this.isExpanded;
		set => this.RaiseAndSetIfChanged(ref this.isExpanded, value);
	}

	private bool isSelected;
	public bool IsSelected
	{
		get => this.isSelected;
		set => this.RaiseAndSetIfChanged(ref this.isSelected, value);
	}

	public ValueTableItemViewModel(string title)
	{
		this.Title = title;
	}
}