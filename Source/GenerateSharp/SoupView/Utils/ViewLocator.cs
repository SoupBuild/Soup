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
		return param switch
		{
			DependencyGraphViewModel => new DependencyGraphView(),
			OperationGraphViewModel => new OperationGraphView(),
			TaskGraphViewModel => new TaskGraphView(),
			_ => throw new InvalidOperationException("Failed to get the view type"),
		};
	}

	public bool Match(object? data)
	{
		return data is ViewModelBase;
	}
}