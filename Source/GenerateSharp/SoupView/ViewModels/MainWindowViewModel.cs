using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Platform.Storage;
using Opal;
using ReactiveUI;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Input;

namespace Soup.View.ViewModels;

public class MainWindowViewModel : ViewModelBase
{
	private IStorageProvider storageProvider;
	private Path? recipeFile;
	private DependencyGraphViewModel dependencyGraph;

	public ICommand OpenCommand { get; }

	public ICommand ExitCommand { get; }

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
		private set => this.RaiseAndSetIfChanged(ref content, value);
	}

	public MainWindowViewModel(IStorageProvider storageProvider)
	{
		this.storageProvider = storageProvider;

		OpenCommand = ReactiveCommand.Create(OnOpenAsync);
		ExitCommand = ReactiveCommand.Create(OnExit);

		dependencyGraph = new DependencyGraphViewModel();
		content = dependencyGraph;
	}

	private async Task OnOpenAsync()
	{
		var filePickerResult = await this.storageProvider.OpenFilePickerAsync(
			new FilePickerOpenOptions()
			{
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
			RecipeFile = new Path(file.Path.ToString());
		}
	}

	private void OnExit()
	{
		if (Application.Current?.ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktopApp)
		{
			desktopApp.Shutdown();
		}
	}
}