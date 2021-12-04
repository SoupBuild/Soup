// <copyright file="OperationDetailsViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Runtime;
using System.Collections.Generic;
using System.Collections.ObjectModel;

namespace SoupView.ViewModel
{
	internal class OperationDetailsViewModel : Observable
	{
		private ObservableCollection<PropertyValue> properties = new ObservableCollection<PropertyValue>();

		public OperationDetailsViewModel(OperationInfo operation)
		{
			properties.Clear();
			properties.Add(new PropertyValue("Title", operation.Title));
			properties.Add(new PropertyValue("Id", operation.Id.ToString()));
			properties.Add(new PropertyValue("DependencyCount", operation.DependencyCount.ToString()));
			properties.Add(new PropertyValue("Executable", operation.Command.Executable.ToString()));
			properties.Add(new PropertyValue("WorkingDirectory", operation.Command.WorkingDirectory.ToString()));
			properties.Add(new PropertyValue("Arguments", operation.Command.Arguments));
		}

		public IList<PropertyValue> Properties => properties;
	}
}
