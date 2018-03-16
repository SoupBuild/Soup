// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	public interface IBuildGenerator
	{
		void GenerateBuild(Recipe recipe, string packageDirectory, string targetDirectory);
		void GenerateDependencies(
			LocalUserConfig config,
			Recipe recipe,
			string packageDirectory,
			string targetDirectory,
			string outDir);
		void GenerateInclude(Recipe recipe, string packageDirectory, string targetDirectory);
	}
}
