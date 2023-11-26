using Avalonia;
using Avalonia.ReactiveUI;
using Opal;
using Opal.System;
using Projektanker.Icons.Avalonia;
using Projektanker.Icons.Avalonia.FontAwesome;
using System;

namespace Soup.View;

public static class Program
{
	/// <summary>
	/// Initialization code. Don't use any Avalonia, third-party APIs or any
	/// SynchronizationContext-reliant code before AppMain is called: things aren't initialized
	/// yet and stuff might break.
	/// </summary>
	[STAThread]
	public static void Main(string[] args)
	{
		BuildAvaloniaApp().StartWithClassicDesktopLifetime(args);
	}

	/// <summary>
	/// Avalonia configuration, also used by visual designer.
	/// </summary>
	public static AppBuilder BuildAvaloniaApp()
	{
		// Register the runtime services
		Log.RegisterListener(new ConsoleTraceListener());
		LifetimeManager.RegisterSingleton<IFileSystem, RuntimeFileSystem>();

		IconProvider.Current.Register<FontAwesomeIconProvider>();

		return AppBuilder.Configure<App>()
			.UsePlatformDetect()
			.WithInterFont()
			.LogToTrace()
			.UseReactiveUI();
	}
}
