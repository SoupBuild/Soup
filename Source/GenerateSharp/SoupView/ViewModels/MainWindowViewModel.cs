using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Platform.Storage;
using ReactiveUI;
using System.Windows.Input;
using Path = Opal.Path;

namespace Soup.View.ViewModels;

public class MainWindowViewModel : ViewModelBase
{
	private Path? recipeFile;
	private readonly DependencyGraphViewModel dependencyGraph;
	private readonly TaskGraphViewModel taskGraph;
	private readonly OperationGraphViewModel operationGraph;

	public IStorageProvider? StorageProvider { get; set; }

	public ICommand OpenCommand { get; }

	public ICommand ExitCommand { get; }

	public ICommand SelectRootCommand { get; }

	public ICommand SelectTasksCommand { get; }

	public ICommand SelectOperationsCommand { get; }

	private ViewModelBase content;

	public Path? RecipeFile
	{
		get => recipeFile;
		private set
		{
			if (this.CheckRaiseAndSetIfChanged(ref recipeFile, value))
			{
				if (recipeFile is not null)
				{
					_ = dependencyGraph.LoadProjectAsync(recipeFile);
				}
			}
		}
	}

	public ViewModelBase Content
	{
		get => content;
		private set
		{
			if (this.CheckRaiseAndSetIfChanged(ref content, value))
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

		dependencyGraph = new DependencyGraphViewModel();
		taskGraph = new TaskGraphViewModel();
		operationGraph = new OperationGraphViewModel();

		dependencyGraph.PropertyChanged += DependencyGraph_PropertyChanged;

		content = dependencyGraph;

		if (packagePath is not null)
		{
			RecipeFile = new Path(packagePath);
		}
	}

	private void DependencyGraph_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
	{
		if (e.PropertyName == nameof(dependencyGraph.SelectedProject))
		{
			_ = taskGraph.LoadProjectAsync(dependencyGraph.SelectedProject?.Path);
			_ = operationGraph.LoadProjectAsync(dependencyGraph.SelectedProject?.Path);

			this.RaisePropertyChanged(nameof(SelectedPackageName));
		}
	}

	public string SelectedPackageName => this.dependencyGraph.SelectedProject?.Name ?? "[Package]";

	public bool IsRootSelected => ReferenceEquals(content, dependencyGraph);

	public bool IsTasksSelected => ReferenceEquals(content, taskGraph);

	public bool IsOperationsSelected => ReferenceEquals(content, operationGraph);

	private async Task OnOpenAsync()
	{
		if (this.StorageProvider is null)
			throw new InvalidOperationException("Missing storage provider");

		var filePickerResult = await this.StorageProvider.OpenFilePickerAsync(
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
		Content = dependencyGraph;
	}

	private void OnSelectTasks()
	{
		Content = taskGraph;
	}

	private void OnSelectOperations()
	{
		Content = operationGraph;
	}
}