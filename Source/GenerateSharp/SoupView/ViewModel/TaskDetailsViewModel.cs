// <copyright file="TaskDetailsViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using ValueType = Soup.Build.ValueType;

namespace SoupView.ViewModel
{
	internal class TaskDetailsViewModel : Observable
	{
		private ObservableCollection<PropertyValue> properties = new ObservableCollection<PropertyValue>();

		public TaskDetailsViewModel(string name, IValueTable taskInfo)
		{
			properties.Clear();
			properties.Add(new PropertyValue("Name", name));

			State = new ObservableCollection<ValueTableItemViewModel>();
			BuildValueTable(taskInfo, State);
		}

		public IList<PropertyValue> Properties => properties;


		public ObservableCollection<ValueTableItemViewModel> State { get; init; }

		private void BuildValueTable(
			IValueTable table,
			ObservableCollection<ValueTableItemViewModel> viewModelList)
		{
			viewModelList.Clear();
			foreach (var value in table)
			{
				string title;
				ValueTableItemType type;
				var children = new ObservableCollection<ValueTableItemViewModel>();
				switch (value.Value.Type)
				{
					case ValueType.Boolean:
						title = $"{value.Key} - {value.Value.AsBoolean()}";
						type = ValueTableItemType.Value;
						break;
					case ValueType.String:
						title = $"{value.Key} - {value.Value.AsString()}";
						type = ValueTableItemType.Value;
						break;
					case ValueType.Float:
						title = $"{value.Key} - {value.Value.AsFloat()}";
						type = ValueTableItemType.Value;
						break;
					case ValueType.Integer:
						title = $"{value.Key} - {value.Value.AsInteger()}";
						type = ValueTableItemType.Value;
						break;
					case ValueType.List:
						title = $"{value.Key}";
						type = ValueTableItemType.List;
						BuildValueList(value.Value.AsList(), children);
						break;
					case ValueType.Table:
						title = $"{value.Key}";
						type = ValueTableItemType.Table;
						BuildValueTable(value.Value.AsTable(), children);
						break;
					default:
						throw new InvalidOperationException("Unknown Value type");
				}

				var viewModel = new ValueTableItemViewModel(title)
				{
					Type = type,
					Children = children,
				};

				viewModelList.Add(viewModel);
			}
		}

		private void BuildValueList(
			IValueList list,
			ObservableCollection<ValueTableItemViewModel> viewModelList)
		{
			viewModelList.Clear();
			foreach (var value in list)
			{
				string title;
				ValueTableItemType type;
				var children = new ObservableCollection<ValueTableItemViewModel>(); ;
				switch (value.Type)
				{
					case ValueType.Boolean:
						title = $"{value.AsBoolean()}";
						type = ValueTableItemType.Value;
						break;
					case ValueType.String:
						title = $"{value.AsString()}";
						type = ValueTableItemType.Value;
						break;
					case ValueType.Float:
						title = $"{value.AsFloat()}";
						type = ValueTableItemType.Value;
						break;
					case ValueType.Integer:
						title = $"{value.AsInteger()}";
						type = ValueTableItemType.Value;
						break;
					case ValueType.List:
						title = string.Empty;
						type = ValueTableItemType.List;
						BuildValueList(value.AsList(), children);
						break;
					case ValueType.Table:
						title = string.Empty;
						type = ValueTableItemType.Table;
						BuildValueTable(value.AsTable(), children);
						break;
					default:
						throw new InvalidOperationException("Unknown Value type");
				}

				var viewModel = new ValueTableItemViewModel(title)
				{
					Type = type,
					Children = children,
				};

				viewModelList.Add(viewModel);
			}
		}
	}
}
