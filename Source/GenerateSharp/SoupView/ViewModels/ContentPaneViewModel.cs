// <copyright file="ContentPaneViewModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Avalonia.Controls.Notifications;
using Avalonia.Threading;

namespace Soup.View.ViewModels;

public class ContentPaneViewModel : ViewModelBase
{
	public WindowNotificationManager? NotificationManager { get; set; }

	protected void NotifyError(string message)
	{
		Dispatcher.UIThread.Post(() =>
		{
			this.NotificationManager?.Show(
				new Notification(
					"Error",
					message,
					NotificationType.Error));
		});
	}
}