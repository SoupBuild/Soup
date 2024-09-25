using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using Soup.View.ViewModels;
using System.Linq;

namespace Soup.View;

public partial class App : Application
{
	public override void Initialize()
	{
		AvaloniaXamlLoader.Load(this);
	}

	public override void OnFrameworkInitializationCompleted()
	{
		if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
		{
			desktop.MainWindow = new MainWindow()
			{
				DataContext = new MainWindowViewModel(desktop.Args?.FirstOrDefault()),
			};
		}

		base.OnFrameworkInitializationCompleted();
	}
}