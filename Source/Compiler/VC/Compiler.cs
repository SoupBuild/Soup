// <copyright company="Soup" file="Compiler.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Compiler.VC
{
	using System.Threading.Tasks;

	public class Compiler : ICompiler
	{
		public Task ExecuteAsync()
		{
			return Task.CompletedTask;
		}
	}
}
