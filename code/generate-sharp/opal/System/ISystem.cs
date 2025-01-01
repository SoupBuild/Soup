// <copyright file="ISystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Runtime.Versioning;

namespace Opal.System;

/// <summary>
/// The system access interface
/// Interface mainly used to allow for unit testing client code.
/// </summary>
public interface ISystem
{
	/// <summary>
	/// Gets the registry value
	/// </summary>
	[SupportedOSPlatform("windows")]
	object? GetRegistryValue(string keyName, string? valueName, object? defaultValue);
}
