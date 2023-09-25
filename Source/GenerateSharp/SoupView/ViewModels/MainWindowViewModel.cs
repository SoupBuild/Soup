using Avalonia;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Platform.Storage;
using Opal;
using ReactiveUI;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Soup.View.ViewModels;

public class MainWindowViewModel : ViewModelBase
{
	private IStorageProvider? storageProvider;
	private Path? recipeFile;
	private DependencyGraphViewModel dependencyGraph;
	private TaskGraphViewModel taskGraph;
	private OperationGraphViewModel operationGraph;

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
					_ = taskGraph.LoadProjectAsync(recipeFile);
					_ = operationGraph.LoadProjectAsync(recipeFile);
				}
			}
		}
	}

	public ViewModelBase Content
	{
		get => content;
		private set => this.RaiseAndSetIfChanged(ref content, value);
	}

	public MainWindowViewModel() : this(null)
	{
	}

	public MainWindowViewModel(IStorageProvider? storageProvider)
	{
		this.storageProvider = storageProvider;

		OpenCommand = ReactiveCommand.Create(OnOpenAsync);
		ExitCommand = ReactiveCommand.Create(OnExit);

		SelectRootCommand = ReactiveCommand.Create(OnSelectRoot);
		SelectTasksCommand = ReactiveCommand.Create(OnSelectTasks);
		SelectOperationsCommand = ReactiveCommand.Create(OnSelectOperations);

		dependencyGraph = new DependencyGraphViewModel();
		taskGraph = new TaskGraphViewModel();
		operationGraph = new OperationGraphViewModel();

		content = dependencyGraph;
	}

	private async Task OnOpenAsync()
	{
		if (this.storageProvider is null)
			throw new InvalidOperationException("Missing storage provider");

		var filePickerResult = await this.storageProvider.OpenFilePickerAsync(
			new FilePickerOpenOptions()
			{
				AllowMultiple = false,
				FileTypeFilter = new List<FilePickerFileType>()
				{
					new FilePickerFileType("Recipe")
					{
						Patterns = new List<string>() { "Recipe.sml" },
					},
				}
			});

		// Use file picker like normal!
		var file = filePickerResult.FirstOrDefault();

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