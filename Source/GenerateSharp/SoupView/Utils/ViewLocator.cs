using Avalonia.Controls;
using Avalonia.Controls.Templates;
using Soup.View.ViewModels;
using System;

namespace Soup.View;

public class ViewLocator : IDataTemplate
{
	public Control Build(object? data)
	{
		var name = data?.GetType().FullName?.Replace("ViewModel", "View") ??
			throw new InvalidOperationException("Failed to get the view name");

		var type = Type.GetType(name) ??
			throw new InvalidOperationException("Failed to get the view type");

		return (Control?)Activator.CreateInstance(type) ??
			throw new InvalidOperationException("Failed to get the create the view type");
	}

	public bool Match(object? data)
	{
		return data is ViewModelBase;
	}
}