// <copyright company="Soup" file="IBuildRunner.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	public interface IBuildRunner
	{
		/// <summary>
		/// Build the provided package path
		/// Returns true if success, otherwise false
		/// </summary>s
		bool Build(string path, bool showOutput);
	}
}
