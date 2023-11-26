using Avalonia.Controls;
using Avalonia.Controls.Templates;
using Soup.View.ViewModels;
using Soup.View.Views;
using System;

namespace Soup.View;

public class ViewLocator : IDataTemplate
{
	public Control Build(object? param)
	{
		switch (param)
		{
			case DependencyGraphViewModel:
				return new DependencyGraphView();
			case OperationGraphViewModel:
				return new OperationGraphView();
			case TaskGraphViewModel:
				return new TaskGraphView();
			default:
				throw new InvalidOperationException("Failed to get the view type");
		}
	}

	public bool Match(object? data)
	{
		return data is ViewModelBase;
	}
}