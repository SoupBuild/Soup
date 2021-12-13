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
						break;
					case ValueType.String:
						viewModel.Title = $"{value.Key} - {value.Value.AsString()}";
						break;
					case ValueType.Float:
						viewModel.Title = $"{value.Key} - {value.Value.AsFloat()}";
						break;
					case ValueType.Integer:
						viewModel.Title = $"{value.Key} - {value.Value.AsInteger()}";
						break;
					case ValueType.List:
						viewModel.Title = $"{value.Key}";
						BuildValueList(value.Value.AsList(), viewModel.Children);
						break;
					case ValueType.Table:
						viewModel.Title = $"{value.Key}";
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
						break;
					case ValueType.String:
						viewModel.Title = $"{value.AsString()}";
						break;
					case ValueType.Float:
						viewModel.Title = $"{value.AsFloat()}";
						break;
					case ValueType.Integer:
						viewModel.Title = $"{value.AsInteger()}";
						break;
					case ValueType.List:
						viewModel.Title = $"LIST";
						BuildValueList(value.AsList(), viewModel.Children);
						break;
					case ValueType.Table:
						viewModel.Title = $"TABLE";
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
