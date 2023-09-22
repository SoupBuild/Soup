using Avalonia.Controls;
using Soup.View.ViewModels;

namespace Soup.View;

public partial class MainWindow : Window
{
	public MainWindow()
	{
		InitializeComponent();


		DataContext = new MainWindowViewModel(StorageProvider);
	}
}