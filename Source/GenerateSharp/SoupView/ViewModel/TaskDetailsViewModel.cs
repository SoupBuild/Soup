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
				var viewModel = new ValueTableItemViewModel();

				switch (value.Value.Type)
				{
					case ValueType.Boolean:
						viewModel.Title = $"{value.Key} - {value.Value.AsBoolean()}";
						viewModel.Type = ValueTableItemType.Value;
						break;
					case ValueType.String:
						viewModel.Title = $"{value.Key} - {value.Value.AsString()}";
						viewModel.Type = ValueTableItemType.Value;
						break;
					case ValueType.Float:
						viewModel.Title = $"{value.Key} - {value.Value.AsFloat()}";
						viewModel.Type = ValueTableItemType.Value;
						break;
					case ValueType.Integer:
						viewModel.Title = $"{value.Key} - {value.Value.AsInteger()}";
						viewModel.Type = ValueTableItemType.Value;
						break;
					case ValueType.List:
						viewModel.Title = $"{value.Key}";
						viewModel.Type = ValueTableItemType.List;
						BuildValueList(value.Value.AsList(), viewModel.Children);
						break;
					case ValueType.Table:
						viewModel.Title = $"{value.Key}";
						viewModel.Type = ValueTableItemType.Table;
						BuildValueTable(value.Value.AsTable(), viewModel.Children);
						break;
					default:
						throw new InvalidOperationException("Unknown Value type");
				}

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
				var viewModel = new ValueTableItemViewModel();

				switch (value.Type)
				{
					case ValueType.Boolean:
						viewModel.Title = $"{value.AsBoolean()}";
						viewModel.Type = ValueTableItemType.Value;
						break;
					case ValueType.String:
						viewModel.Title = $"{value.AsString()}";
						viewModel.Type = ValueTableItemType.Value;
						break;
					case ValueType.Float:
						viewModel.Title = $"{value.AsFloat()}";
						viewModel.Type = ValueTableItemType.Value;
						break;
					case ValueType.Integer:
						viewModel.Title = $"{value.AsInteger()}";
						viewModel.Type = ValueTableItemType.Value;
						break;
					case ValueType.List:
						viewModel.Title = string.Empty;
						viewModel.Type = ValueTableItemType.List;
						BuildValueList(value.AsList(), viewModel.Children);
						break;
					case ValueType.Table:
						viewModel.Title = string.Empty;
						viewModel.Type = ValueTableItemType.Table;
						BuildValueTable(value.AsTable(), viewModel.Children);
						break;
					default:
						throw new InvalidOperationException("Unknown Value type");
				}

				viewModelList.Add(viewModel);
			}
		}
	}
}
