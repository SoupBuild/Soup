// <copyright company="Soup" file="IBuildGenerator.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	public interface IBuildGenerator
	{
		string Name { get; }

		void GenerateBuild(Recipe recipe, string packageDirectory, string targetDirectory);

		void GenerateDependencies(
			Recipe recipe,
			string packageDirectory,
			string targetDirectory,
			string outDir);

		void GenerateInclude(Recipe recipe, string packageDirectory, string targetDirectory);
	}
}
