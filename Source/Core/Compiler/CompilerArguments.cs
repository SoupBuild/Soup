// <copyright company="Soup" file="CompilerArguments.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.Collections.Generic;

	public class CompilerArguments
	{
		public string RootDirectory { get; set; }
		public string ObjectDirectory { get; set; }
		public IList<string> SourceFiles { get; set; }
	}
}
