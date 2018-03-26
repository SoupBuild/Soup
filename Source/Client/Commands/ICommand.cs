// <copyright company="Soup" file="ICommand.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using System.Threading.Tasks;

	/// <summary>
	/// Command interface
	/// </summary>
	internal interface ICommand
	{
		string Name { get; }

		Task InvokeAsync(string[] args);
	}
}
