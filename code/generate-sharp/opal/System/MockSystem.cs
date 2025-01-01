// <copyright file="MockSystem.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Opal.System;

/// <summary>
/// The mock system
/// TODO: Move into test project.
/// </summary>
public class MockSystem : ISystem
{
	private readonly List<string> requests;
	private readonly Dictionary<(string, string?), object?> registryValues;

	/// <summary>
	/// Initializes a new instance of the <see cref='MockProcessManager'/> class.
	/// </summary>
	public MockSystem()
	{
		this.requests = [];
		this.registryValues = [];
	}

	/// <summary>
	/// Register a value
	/// </summary>
	public void RegisterRegistryValue(string keyName, string? valueName, object? value)
	{
		this.registryValues.Add((keyName, valueName), value);
	}

	/// <summary>
	/// Get the load requests.
	/// </summary>
	public IReadOnlyList<string> Requests => this.requests;

	/// <summary>
	/// Gets the registry value
	/// </summary>
	public object? GetRegistryValue(string keyName, string? valueName, object? defaultValue)
	{
		var message = $"GetRegistryValue: {keyName} {valueName}";
		this.requests.Add(message);
		if (this.registryValues.TryGetValue((keyName, valueName), out var value))
		{
			return value;
		}
		else
		{
			return null;
		}
	}
}