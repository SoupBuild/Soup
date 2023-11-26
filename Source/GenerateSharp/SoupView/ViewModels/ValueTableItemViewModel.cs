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

	private ObservableCollection<ValueTableItemViewModel> m_children = new ObservableCollection<ValueTableItemViewModel>();
	public ObservableCollection<ValueTableItemViewModel> Children
	{
		get => m_children;
		init => m_children = value;
	}

	private bool m_isExpanded;
	public bool IsExpanded
	{
		get => m_isExpanded;
		set => this.RaiseAndSetIfChanged(ref m_isExpanded, value);
	}

	private bool m_isSelected;
	public bool IsSelected
	{
		get => m_isSelected;
		set => this.RaiseAndSetIfChanged(ref m_isSelected, value);
	}

	public ValueTableItemViewModel(string title)
	{
		Title = title;
	}
}