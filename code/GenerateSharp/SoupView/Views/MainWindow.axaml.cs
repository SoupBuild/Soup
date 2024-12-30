using Avalonia.Controls;
using Soup.View.ViewModels;

namespace Soup.View;

public partial class MainWindow : Window
{
	public MainWindow()
	{
		InitializeComponent();
		if (this.DataContext is MainWindowViewModel viewModel)
		{
			viewModel.StorageProvider = this.StorageProvider;
		}
	}
}