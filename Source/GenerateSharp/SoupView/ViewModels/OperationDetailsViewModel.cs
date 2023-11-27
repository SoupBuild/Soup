// <copyright file="OperationDetailsViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Avalonia.Controls;
using Avalonia.Controls.Models.TreeDataGrid;
using Soup.Build.Utilities;
using System.Collections.ObjectModel;
using System.Globalization;
using System.Linq;

namespace Soup.View.ViewModels;

public class OperationDetailsViewModel : ViewModelBase
{
	private readonly ObservableCollection<PropertyValueViewModel> properties = [];

	public OperationDetailsViewModel(
		FileSystemState fileSystemState,
		OperationInfo operation,
		OperationResult? operationResult)
	{
		properties.Clear();
		properties.Add(new PropertyValueViewModel("Title", operation.Title));
		properties.Add(new PropertyValueViewModel("Id", operation.Id.ToString()));
		properties.Add(new PropertyValueViewModel("DependencyCount", operation.DependencyCount.ToString(CultureInfo.InvariantCulture)));
		properties.Add(new PropertyValueViewModel("Executable", operation.Command.Executable.ToString()));
		properties.Add(new PropertyValueViewModel("WorkingDirectory", operation.Command.WorkingDirectory.ToString()));
		properties.Add(new PropertyValueViewModel("Arguments", null)
		{
			Children = new ObservableCollection<PropertyValueViewModel>(
				operation.Command.Arguments.Select(value => new PropertyValueViewModel(string.Empty, value))),
		});

		var declaredInputFiles = fileSystemState.GetFilePaths(operation.DeclaredInput);
		var declaredOutputFiles = fileSystemState.GetFilePaths(operation.DeclaredOutput);
		var readAccessFiles = fileSystemState.GetFilePaths(operation.ReadAccess);
		var writeAccessFiles = fileSystemState.GetFilePaths(operation.WriteAccess);

		properties.Add(new PropertyValueViewModel("DeclaredInput", null)
		{
			Children = new ObservableCollection<PropertyValueViewModel>(
				declaredInputFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))),
		});
		properties.Add(new PropertyValueViewModel("DeclaredOutput", null)
		{
			Children = new ObservableCollection<PropertyValueViewModel>(
				declaredOutputFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))),
		});
		properties.Add(new PropertyValueViewModel("ReadAccess", null)
		{
			Children = new ObservableCollection<PropertyValueViewModel>(
				readAccessFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))),
		});
		properties.Add(new PropertyValueViewModel("WriteAccess", null)
		{
			Children = new ObservableCollection<PropertyValueViewModel>(
				writeAccessFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))),
		});

		if (operationResult != null)
		{
			properties.Add(new PropertyValueViewModel("WasSuccessfulRun", operationResult.WasSuccessfulRun.ToString()));
			properties.Add(new PropertyValueViewModel("EvaluateTime", operationResult.EvaluateTime.ToString(CultureInfo.InvariantCulture)));
			var observedInputFiles = fileSystemState.GetFilePaths(operationResult.ObservedInput);
			var observedOutputFiles = fileSystemState.GetFilePaths(operationResult.ObservedOutput);
			properties.Add(new PropertyValueViewModel("ObservedInput", null)
			{
				Children = new ObservableCollection<PropertyValueViewModel>(
				observedInputFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))),
			});
			properties.Add(new PropertyValueViewModel("ObservedOutput", null)
			{
				Children = new ObservableCollection<PropertyValueViewModel>(
				observedOutputFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))),
			});
		}

		Properties = new HierarchicalTreeDataGridSource<PropertyValueViewModel>(properties)
		{
			Columns =
			{
				new HierarchicalExpanderColumn<PropertyValueViewModel>(
					new TextColumn<PropertyValueViewModel, string>("Name", x => x.Name),
					x => x.Children),
				new TextColumn<PropertyValueViewModel, string>(
					"Value",
					x => x.Value),
			},
		};
	}

	public HierarchicalTreeDataGridSource<PropertyValueViewModel> Properties { get; }
}