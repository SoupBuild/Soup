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
		this.properties.Clear();
		this.properties.Add(new PropertyValueViewModel("Title", operation.Title));
		this.properties.Add(new PropertyValueViewModel("Id", operation.Id.ToString()));
		this.properties.Add(new PropertyValueViewModel("DependencyCount", operation.DependencyCount.ToString(CultureInfo.InvariantCulture)));
		this.properties.Add(new PropertyValueViewModel("Executable", operation.Command.Executable.ToString()));
		this.properties.Add(new PropertyValueViewModel("WorkingDirectory", operation.Command.WorkingDirectory.ToString()));
		this.properties.Add(new PropertyValueViewModel("Arguments", null)
		{
			Children = [.. operation.Command.Arguments.Select(value => new PropertyValueViewModel(string.Empty, value))],
		});

		var declaredInputFiles = fileSystemState.GetFilePaths(operation.DeclaredInput);
		var declaredOutputFiles = fileSystemState.GetFilePaths(operation.DeclaredOutput);
		var readAccessFiles = fileSystemState.GetFilePaths(operation.ReadAccess);
		var writeAccessFiles = fileSystemState.GetFilePaths(operation.WriteAccess);

		this.properties.Add(new PropertyValueViewModel("DeclaredInput", null)
		{
			Children = [.. declaredInputFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))],
		});
		this.properties.Add(new PropertyValueViewModel("DeclaredOutput", null)
		{
			Children = [.. declaredOutputFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))],
		});
		this.properties.Add(new PropertyValueViewModel("ReadAccess", null)
		{
			Children = [.. readAccessFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))],
		});
		this.properties.Add(new PropertyValueViewModel("WriteAccess", null)
		{
			Children = [.. writeAccessFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))],
		});

		if (operationResult != null)
		{
			this.properties.Add(new PropertyValueViewModel("WasSuccessfulRun", operationResult.WasSuccessfulRun.ToString()));
			this.properties.Add(new PropertyValueViewModel("EvaluateTime", operationResult.EvaluateTime.ToString(CultureInfo.InvariantCulture)));
			var observedInputFiles = fileSystemState.GetFilePaths(operationResult.ObservedInput);
			var observedOutputFiles = fileSystemState.GetFilePaths(operationResult.ObservedOutput);
			this.properties.Add(new PropertyValueViewModel("ObservedInput", null)
			{
				Children = [.. observedInputFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))],
			});
			this.properties.Add(new PropertyValueViewModel("ObservedOutput", null)
			{
				Children = [.. observedOutputFiles.Select(value => new PropertyValueViewModel(string.Empty, value.ToString()))],
			});
		}

		Properties = new HierarchicalTreeDataGridSource<PropertyValueViewModel>(this.properties)
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