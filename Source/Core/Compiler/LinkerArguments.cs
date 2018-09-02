// <copyright company="Soup" file="LinkerArguments.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Compiler
{
	using System.Collections.Generic;

	public class LinkerArguments
	{
		public string RootDirectory { get; set; } = "";
		public string OutputDirectory { get; set; } = "";
		public string Name { get; set; } = "";
		public IList<string> SourceFiles { get; set; } = new List<string>();
		public IList<string> LibraryFiles { get; set; } = new List<string>();
	}
}
