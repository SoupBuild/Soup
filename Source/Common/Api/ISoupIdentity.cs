// <copyright company="Soup" file="ISoupIdentity.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using System.Threading.Tasks;

	/// <summary>
	/// Represents a collection of functions to interact with the Indentity endpoints
	/// </summary>
	public interface ISoupIdentity
	{
		/// <summary>
		/// Authenticate user
		/// </summary>
		Task<string> AuthenticateUserAsync();
	}
}
