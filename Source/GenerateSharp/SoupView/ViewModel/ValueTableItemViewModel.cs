// <copyright file="ValueTableItemViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.ObjectModel;

namespace SoupView.ViewModel
{
	class ValueTableItemViewModel : Observable
	{
		public enum ExplorerItemType { Folder, File };

		public string Title { get; set; }

		public ExplorerItemType Type { get; set; }

		private ObservableCollection<ValueTableItemViewModel> m_children = new ObservableCollection<ValueTableItemViewModel>();
		public ObservableCollection<ValueTableItemViewModel> Children
		{
			get { return m_children; }
			set { m_children = value; }
		}

		private bool m_isExpanded;
		public bool IsExpanded
		{
			get { return m_isExpanded; }
			set
			{
				if (m_isExpanded != value)
				{
					m_isExpanded = value;
					NotifyPropertyChanged();
				}
			}
		}

		private bool m_isSelected;
		public bool IsSelected
		{
			get { return m_isSelected; }
			set
			{
				if (m_isSelected != value)
				{
					m_isSelected = value;
					NotifyPropertyChanged();
				}
			}
		}
	}
}
