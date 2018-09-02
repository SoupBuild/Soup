// <copyright company="Soup" file="CompilerArguments.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Compiler
{
	using System.Collections.Generic;

	public enum LanguageStandard
	{
		CPP11,
		CPP14,
		CPP17,
		Latest
	};

	public class CompilerArguments
	{
		public LanguageStandard Standard { get; set; } = LanguageStandard.CPP14;
		public string RootDirectory { get; set; } = "";
		public string OutputDirectory { get; set; } = "";
		public IList<string> PreprocessorDefinitions { get; set; } = new List<string>();
		public IList<string> SourceFiles { get; set; } = new List<string>();
		public IList<string> Modules { get; set; } = new List<string>();
		public bool ExportModule { get; set; } = false;
	}
}
