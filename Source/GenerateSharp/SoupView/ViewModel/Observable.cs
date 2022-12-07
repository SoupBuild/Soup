// <copyright file="Observable.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace SoupView.ViewModel
{
	internal class Observable : INotifyPropertyChanged
	{
		public event PropertyChangedEventHandler? PropertyChanged;

		protected void NotifyPropertyChanged([CallerMemberName] string? propertyName = null)
		{
			if (PropertyChanged != null)
			{
				PropertyChanged(this, new PropertyChangedEventArgs(propertyName));
			}
		}
	}
}
