// <copyright file="RuntimeSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Microsoft.Win32;
using System.Runtime.Versioning;

namespace Opal.System;

/// <summary>
/// A windows platform specific process executable using system
/// </summary>
public class RuntimeSystem : ISystem
{
	/// <summary>
	/// Initializes a new instance of the <see cref='RuntimeProcessManager'/> class.
	/// </summary>
	public RuntimeSystem()
	{
	}

	/// <summary>
	/// Gets the registry value
	/// </summary>
	[SupportedOSPlatform("windows")]
	public object? GetRegistryValue(string keyName, string? valueName, object? defaultValue)
	{
		return Registry.GetValue(keyName, valueName, null);
	}
}