// <copyright file="SelectedButton.axaml.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Avalonia;
using Avalonia.Controls;

namespace Soup.View.Views;

public class SelectedButton : Button
{
	/// <summary>
	/// Identifies the <see cref="IsSelected"/> property.
	/// </summary>
	public static readonly StyledProperty<bool> IsSelectedProperty =
		AvaloniaProperty.Register<SelectedButton, bool>(nameof(IsSelected));

	public SelectedButton()
	{
	}

	/// <summary>
	/// Gets or sets the is selected value
	/// </summary>
	public bool IsSelected
	{
		get => GetValue(IsSelectedProperty);
		set => SetValue(IsSelectedProperty, value);
	}

	protected override void OnPropertyChanged(AvaloniaPropertyChangedEventArgs change)
	{
		if (change.Property.Name == nameof(this.IsSelected) && change.NewValue is bool boolValue)
		{
			this.PseudoClasses.Set(":selected", boolValue);
		}

		base.OnPropertyChanged(change);
	}
}