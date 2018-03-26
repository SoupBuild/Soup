// <copyright company="Soup" file="Singleton.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	/// <summary>
	/// A helper class for holding a singleton for the lifetime of the application
	/// </summary>
	/// <typeparam name="T">The singleton type</typeparam>
	public static class Singleton<T>
	{
		/// <summary>
		/// Gets or sets the singleton instance
		/// </summary>
		public static T Instance { get; set; }
	}
}
