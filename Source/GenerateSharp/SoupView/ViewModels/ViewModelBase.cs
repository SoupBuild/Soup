using ReactiveUI;
using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace Soup.View.ViewModels;

public class ViewModelBase : ReactiveObject
{
	public bool CheckRaiseAndSetIfChanged<TRet>(
		ref TRet backingField,
		TRet newValue,
		[CallerMemberName] string? propertyName = null)
	{
		if (propertyName is null)
		{
			throw new ArgumentNullException(nameof(propertyName));
		}

		if (EqualityComparer<TRet>.Default.Equals(backingField, newValue))
		{
			return false;
		}

		this.RaisePropertyChanging(propertyName);
		backingField = newValue;
		this.RaisePropertyChanged(propertyName);
		return true;
	}
}
