// <copyright company="Soup" file="ICompiler.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.Threading.Tasks;

	public interface ICompiler
	{
		Task ExecuteAsync(CompilerArguments args);
	}
}
