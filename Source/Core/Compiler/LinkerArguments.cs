// <copyright company="Soup" file="LinkerArguments.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Compiler
{
	using System.Collections.Generic;

	public enum LinkTarget
	{
		ConsoleApp,
		DynamicLibrary,
		StaticLibrary
	};
	
	public class LinkerArguments
	{
		public LinkTarget Target { get; set; } = LinkTarget.DynamicLibrary;
		public string RootDirectory { get; set; } = "";
		public string OutputDirectory { get; set; } = "";
		public string Name { get; set; } = "";
		public IList<string> SourceFiles { get; set; } = new List<string>();
	}
}
