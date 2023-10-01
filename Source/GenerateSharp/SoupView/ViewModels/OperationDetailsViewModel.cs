// <copyright file="OperationDetailsViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Utilities;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace Soup.View.ViewModels
{
	public class OperationDetailsViewModel : ViewModelBase
	{
		private ObservableCollection<PropertyValueViewModel> properties = new ObservableCollection<PropertyValueViewModel>();

		public OperationDetailsViewModel(
			FileSystemState fileSystemState,
			OperationInfo operation,
			OperationResult? operationResult)
		{
			properties.Clear();
			properties.Add(new PropertyValueViewModel("Title", operation.Title));
			properties.Add(new PropertyValueViewModel("Id", operation.Id.ToString()));
			properties.Add(new PropertyValueViewModel("DependencyCount", operation.DependencyCount.ToString()));
			properties.Add(new PropertyValueViewModel("Executable", operation.Command.Executable.ToString()));
			properties.Add(new PropertyValueViewModel("WorkingDirectory", operation.Command.WorkingDirectory.ToString()));
			properties.Add(new PropertyValueViewModel("Arguments", string.Concat(operation.Command.Arguments)));

			var declaredInputFiles = fileSystemState.GetFilePaths(operation.DeclaredInput);
			var declaredOutputFiles = fileSystemState.GetFilePaths(operation.DeclaredOutput);
			var readAccessFiles = fileSystemState.GetFilePaths(operation.ReadAccess);
			var writeAccessFiles = fileSystemState.GetFilePaths(operation.WriteAccess);
			properties.Add(new PropertyValueViewModel("DeclaredInput", string.Concat(declaredInputFiles)));
			properties.Add(new PropertyValueViewModel("DeclaredOutput", string.Concat(declaredOutputFiles)));
			properties.Add(new PropertyValueViewModel("ReadAccess", string.Concat(readAccessFiles)));
			properties.Add(new PropertyValueViewModel("WriteAccess", string.Concat(writeAccessFiles)));

			if (operationResult != null)
			{
				properties.Add(new PropertyValueViewModel("WasSuccessfulRun", operationResult.WasSuccessfulRun.ToString()));
				properties.Add(new PropertyValueViewModel("EvaluateTime", operationResult.EvaluateTime.ToString()));
				var observedInputFiles = fileSystemState.GetFilePaths(operationResult.ObservedInput);
				var observedOutputFiles = fileSystemState.GetFilePaths(operationResult.ObservedOutput);
				properties.Add(new PropertyValueViewModel("ObservedInput", string.Concat(observedInputFiles)));
				properties.Add(new PropertyValueViewModel("OvservedOutput", string.Concat(observedOutputFiles)));
			}
		}

		public IList<PropertyValueViewModel> Properties => properties;
	}
}
