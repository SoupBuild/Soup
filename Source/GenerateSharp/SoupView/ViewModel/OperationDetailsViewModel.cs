// <copyright file="OperationDetailsViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace SoupView.ViewModel
{
	internal class OperationDetailsViewModel : Observable
	{
		private ObservableCollection<PropertyValue> properties = new ObservableCollection<PropertyValue>();

		public OperationDetailsViewModel(
			FileSystemState fileSystemState,
			OperationInfo operation,
			OperationResult? operationResult)
		{
			properties.Clear();
			properties.Add(new PropertyValue("Title", operation.Title));
			properties.Add(new PropertyValue("Id", operation.Id.ToString()));
			properties.Add(new PropertyValue("DependencyCount", operation.DependencyCount.ToString()));
			properties.Add(new PropertyValue("Executable", operation.Command.Executable.ToString()));
			properties.Add(new PropertyValue("WorkingDirectory", operation.Command.WorkingDirectory.ToString()));
			properties.Add(new PropertyValue("Arguments", operation.Command.Arguments));

			var declaredInputFiles = fileSystemState.GetFilePaths(operation.DeclaredInput);
			var declaredOutputFiles = fileSystemState.GetFilePaths(operation.DeclaredOutput);
			var readAccessFiles = fileSystemState.GetFilePaths(operation.ReadAccess);
			var writeAccessFiles = fileSystemState.GetFilePaths(operation.WriteAccess);
			properties.Add(new PropertyValue("DeclaredInput", string.Concat(declaredInputFiles)));
			properties.Add(new PropertyValue("DeclaredOutput", string.Concat(declaredOutputFiles)));
			properties.Add(new PropertyValue("ReadAccess", string.Concat(readAccessFiles)));
			properties.Add(new PropertyValue("WriteAccess", string.Concat(writeAccessFiles)));

			if (operationResult != null)
			{
				properties.Add(new PropertyValue("WasSuccessfulRun", operationResult.WasSuccessfulRun.ToString()));
				properties.Add(new PropertyValue("EvaluateTime", operationResult.EvaluateTime.ToString()));
				var observedInputFiles = fileSystemState.GetFilePaths(operationResult.ObservedInput);
				var observedOutputFiles = fileSystemState.GetFilePaths(operationResult.ObservedOutput);
				properties.Add(new PropertyValue("ObservedInput", string.Concat(observedInputFiles)));
				properties.Add(new PropertyValue("OvservedOutput", string.Concat(observedOutputFiles)));
			}
		}

		public IList<PropertyValue> Properties => properties;
	}
}
