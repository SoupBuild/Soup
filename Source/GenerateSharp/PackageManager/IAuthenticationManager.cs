// <copyright file="IAuthenticationManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Threading.Tasks;

namespace Soup.Build.PackageManager;

/// <summary>
/// The authentication manager interface
/// Interface mainly used to allow for unit testing client code.
/// </summary>
public interface IAuthenticationManager
{
	/// <summary>
	/// Get the authentication token for the user
	/// </summary>
	public Task<string> EnsureSignInAsync();
}
