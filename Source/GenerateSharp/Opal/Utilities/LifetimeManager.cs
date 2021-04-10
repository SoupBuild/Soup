// <copyright file="LifetimeManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Opal
{
	/// <summary>
	/// The lifetime manager helper
	/// </summary>
	public static class LifetimeManager
	{
		private static Dictionary<Type, object> _singletons = new Dictionary<Type, object>();

		/// <summary>
		/// Register a Singleton with a specified type
		/// </summary>
		public static void RegisterSingleton<TInterface, TImplementation>() where TInterface : notnull where TImplementation : TInterface, new()
		{
			RegisterSingleton<TInterface>(new TImplementation());
		}

		/// <summary>
		/// Register a Singleton with a specified type
		/// </summary>
		public static void RegisterSingleton<TInterface>(TInterface instance) where TInterface : notnull
		{
			_singletons.Add(typeof(TInterface), instance);
		}

		/// <summary>
		/// Clear a Singleton with a specified type
		/// </summary>
		public static void ClearSingleton<TInterface>() where TInterface : notnull
		{
			_singletons.Remove(typeof(TInterface));
		}

		/// <summary>
		/// Get an instance of a managed type
		/// </summary>
		public static TInterface Get<TInterface>()
		{
			if (_singletons.TryGetValue(typeof(TInterface), out var instance))
            {
				return (TInterface)instance;
			}
			else
            {
				throw new InvalidOperationException($"No instance of type {typeof(TInterface)} registered");
            }
		}
	}
}
