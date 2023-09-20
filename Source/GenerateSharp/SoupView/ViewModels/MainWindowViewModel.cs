using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using ReactiveUI;
using System.Windows.Input;

namespace Soup.View.ViewModels;

public class MainWindowViewModel : ViewModelBase
{
	private ViewModelBase content;

	public ICommand OpenCommand { get; }

	public ICommand ExitCommand { get; }

	public ViewModelBase Content
	{
		get => content;
		private set => this.RaiseAndSetIfChanged(ref content, value);
	}

	public MainWindowViewModel()
	{
		OpenCommand = ReactiveCommand.Create(OnOpen);
		ExitCommand = ReactiveCommand.Create(OnExit);

		content = new DependenciesViewModel();
	}

	private void OnOpen()
	{
	}

	private void OnExit()
	{
		if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktopApp)
		{
			desktopApp.Shutdown();
		}
	}
}