// <copyright file="PropertyValueViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.View.ViewModels
{
	public class PropertyValueViewModel
	{
		public PropertyValueViewModel(string name, string value)
		{
			Name = name;
			Value = value;
		}

		public string Name { get; set; } = string.Empty;
		public string Value { get; set; } = string.Empty;
	}
}
