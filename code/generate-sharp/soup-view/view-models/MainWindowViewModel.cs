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
		get => this.recipeFile;
		private set
		{
			if (CheckRaiseAndSetIfChanged(ref this.recipeFile, value))
			{
				if (this.recipeFile is not null)
				{
					_ = this.dependencyGraph.LoadProjectAsync(this.recipeFile);
				}
			}
		}
	}

	public ViewModelBase Content
	{
		get => this.content;
		private set
		{
			if (CheckRaiseAndSetIfChanged(ref this.content, value))
			{
				this.RaisePropertyChanged(nameof(this.IsRootSelected));
				this.RaisePropertyChanged(nameof(this.IsTasksSelected));
				this.RaisePropertyChanged(nameof(this.IsOperationsSelected));
			}
		}
	}

	public MainWindowViewModel(string? packagePath)
	{
		this.OpenCommand = ReactiveCommand.Create(OnOpenAsync);
		this.ExitCommand = ReactiveCommand.Create(OnExit);

		this.SelectRootCommand = ReactiveCommand.Create(OnSelectRoot);
		this.SelectTasksCommand = ReactiveCommand.Create(OnSelectTasks);
		this.SelectOperationsCommand = ReactiveCommand.Create(OnSelectOperations);

		this.dependencyGraph = new DependencyGraphViewModel();
		this.taskGraph = new TaskGraphViewModel();
		this.operationGraph = new OperationGraphViewModel();

		this.dependencyGraph.PropertyChanged += DependencyGraph_PropertyChanged;

		this.content = this.dependencyGraph;

		if (packagePath is not null)
		{
			this.RecipeFile = Path.Parse(packagePath);
		}
	}

	private void DependencyGraph_PropertyChanged(object? sender, System.ComponentModel.PropertyChangedEventArgs e)
	{
		if (e.PropertyName == nameof(this.dependencyGraph.SelectedProject))
		{
			_ = this.taskGraph.LoadProjectAsync(this.dependencyGraph.SelectedProject?.Path, this.dependencyGraph.SelectedProject?.Owner);
			_ = this.operationGraph.LoadProjectAsync(this.dependencyGraph.SelectedProject?.Path);

			this.RaisePropertyChanged(nameof(this.SelectedPackageName));
		}
	}

	public string SelectedPackageName => this.dependencyGraph.SelectedProject?.Name ?? "[Package]";

	public bool IsRootSelected => ReferenceEquals(this.content, this.dependencyGraph);

	public bool IsTasksSelected => ReferenceEquals(this.content, this.taskGraph);

	public bool IsOperationsSelected => ReferenceEquals(this.content, this.operationGraph);

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
			this.RecipeFile = new Path(file.Path.AbsolutePath);
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
		this.Content = this.dependencyGraph;
	}

	private void OnSelectTasks()
	{
		this.Content = this.taskGraph;
	}

	private void OnSelectOperations()
	{
		this.Content = this.operationGraph;
	}
}