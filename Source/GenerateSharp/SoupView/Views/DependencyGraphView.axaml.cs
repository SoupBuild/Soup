using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.Notifications;
using Soup.View.ViewModels;

namespace Soup.View.Views;

public partial class DependencyGraphView : UserControl
{
	public DependencyGraphView()
	{
		InitializeComponent();
	}

	protected override void OnAttachedToVisualTree(VisualTreeAttachmentEventArgs e)
	{
		base.OnAttachedToVisualTree(e);

		var dataContext = (ContentPaneViewModel?)DataContext;
		if (dataContext is not null)
		{
			dataContext.NotificationManager = new WindowNotificationManager(TopLevel.GetTopLevel(this)!);
		}
	}
}