// <copyright file="LifetimeManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Opal;

/// <summary>
/// The lifetime manager helper.
/// </summary>
public static class LifetimeManager
{
	private static readonly Dictionary<Type, object> singletons = [];

	/// <summary>
	/// Register a Singleton with a specified type.
	/// </summary>
	/// <typeparam name="TInterface">The interface type.</typeparam>
	/// <typeparam name="TImplementation">The implemenation type.</typeparam>
	public static void RegisterSingleton<TInterface, TImplementation>()
		where TInterface : notnull
		where TImplementation : TInterface, new()
	{
		RegisterSingleton<TInterface>(new TImplementation());
	}

	/// <summary>
	/// Register a Singleton with a specified type.
	/// </summary>
	/// <typeparam name="TInterface">The interface type.</typeparam>
	/// <param name="instance">The instance.</param>
	public static void RegisterSingleton<TInterface>(TInterface instance)
		where TInterface : notnull
	{
		singletons.Add(typeof(TInterface), instance);
	}

	/// <summary>
	/// Clear a Singleton with a specified type.
	/// </summary>
	/// <typeparam name="TInterface">The interface type.</typeparam>
	public static void ClearSingleton<TInterface>()
		where TInterface : notnull
	{
		_ = singletons.Remove(typeof(TInterface));
	}

	/// <summary>
	/// Get an instance of a managed type.
	/// </summary>
	/// <typeparam name="TInterface">The interface type.</typeparam>
	public static TInterface Get<TInterface>()
	{
		if (singletons.TryGetValue(typeof(TInterface), out var instance))
		{
			return (TInterface)instance;
		}
		else
		{
			throw new InvalidOperationException($"No instance of type {typeof(TInterface)} registered");
		}
	}

	/// <summary>
	/// Get an instance of a managed type.
	/// </summary>
	/// <typeparam name="TInterface">The interface type.</typeparam>
	public static bool Has<TInterface>()
	{
		return singletons.ContainsKey(typeof(TInterface));
	}
}