using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Platform.Storage;
using ReactiveUI;
using System;
using System.Threading.Tasks;
using System.Windows.Input;
using Path = Opal.Path;

namespace Soup.View.ViewModels;

public class MainWindowViewModel : ViewModelBase
{
	private Path? _recipeFile;
	private readonly DependencyGraphViewModel _dependencyGraph;
	private readonly TaskGraphViewModel _taskGraph;
	private readonly OperationGraphViewModel _operationGraph;

	public IStorageProvider? StorageProvider { get; set; }

	public ICommand OpenCommand { get; }

	public ICommand ExitCommand { get; }

	public ICommand SelectRootCommand { get; }

	public ICommand SelectTasksCommand { get; }

	public ICommand SelectOperationsCommand { get; }

	private ViewModelBase content;

	public Path? RecipeFile
	{
		get => _recipeFile;
		private set
		{
			if (CheckRaiseAndSetIfChanged(ref _recipeFile, value))
			{
				if (_recipeFile is not null)
				{
					_ = _dependencyGraph.LoadProjectAsync(_recipeFile);
				}
			}
		}
	}

	public ViewModelBase Content
	{
		get => content;
		private set
		{
			if (CheckRaiseAndSetIfChanged(ref content, value))
			{
				this.RaisePropertyChanged(nameof(IsRootSelected));
				this.RaisePropertyChanged(nameof(IsTasksSelected));
				this.RaisePropertyChanged(nameof(IsOperationsSelected));
			}
		}
	}

	public MainWindowViewModel(string? packagePath)
	{
		OpenCommand = ReactiveCommand.Create(OnOpenAsync);
		ExitCommand = ReactiveCommand.Create(OnExit);

		SelectRootCommand = ReactiveCommand.Create(OnSelectRoot);
		SelectTasksCommand = ReactiveCommand.Create(OnSelectTasks);
		SelectOperationsCommand = ReactiveCommand.Create(OnSelectOperations);

		_dependencyGraph = new DependencyGraphViewModel();
		_taskGraph = new TaskGraphViewModel();
		_operationGraph = new OperationGraphViewModel();

		_dependencyGraph.PropertyChanged += DependencyGraph_PropertyChanged;

		content = _dependencyGraph;

		if (packagePath is not null)
		{
			RecipeFile = Path.Parse(packagePath);
		}
	}

	private void DependencyGraph_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
	{
		if (e.PropertyName == nameof(_dependencyGraph.SelectedProject))
		{
			_ = _taskGraph.LoadProjectAsync(_dependencyGraph.SelectedProject?.Path, _dependencyGraph.SelectedProject?.Owner);
			_ = _operationGraph.LoadProjectAsync(_dependencyGraph.SelectedProject?.Path);

			this.RaisePropertyChanged(nameof(SelectedPackageName));
		}
	}

	public string SelectedPackageName => _dependencyGraph.SelectedProject?.Name ?? "[Package]";

	public bool IsRootSelected => ReferenceEquals(content, _dependencyGraph);

	public bool IsTasksSelected => ReferenceEquals(content, _taskGraph);

	public bool IsOperationsSelected => ReferenceEquals(content, _operationGraph);

	private async Task OnOpenAsync()
	{
		if (StorageProvider is null)
			throw new InvalidOperationException("Missing storage provider");

		var filePickerResult = await StorageProvider.OpenFilePickerAsync(
			new FilePickerOpenOptions()
			{
				AllowMultiple = false,
				FileTypeFilter =
				[
					new FilePickerFileType("Recipe")
					{
						Patterns = ["Recipe.sml"],
					},
				]
			});

		// Use file picker like normal!
		var file = filePickerResult.Count > 0 ? filePickerResult[0] : null;

		if (file != null)
		{
			RecipeFile = new Path(file.Path.AbsolutePath);
		}
	}

	private void OnExit()
	{
		if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktopApp)
		{
			desktopApp.Shutdown();
		}
	}

	private void OnSelectRoot()
	{
		Content = _dependencyGraph;
	}

	private void OnSelectTasks()
	{
		Content = _taskGraph;
	}

	private void OnSelectOperations()
	{
		Content = _operationGraph;
	}
}