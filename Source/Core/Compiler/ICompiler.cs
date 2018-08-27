// <copyright company="Soup" file="ICompiler.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Compiler
{
	using System.Threading.Tasks;

	public interface ICompiler
	{
		Task CompileAsync(CompilerArguments args);
		Task LinkAsync(LinkerArguments args);
	}
}
