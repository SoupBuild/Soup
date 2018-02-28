// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System.Threading.Tasks;

namespace Soup
{
	internal interface ICommand
	{
		string Name { get; }

		Task InvokeAsync(string[] args, LocalUserConfig userConfig);
	}
}
