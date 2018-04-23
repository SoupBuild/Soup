// <copyright company="Soup" file="IBuildGenerator.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.Threading.Tasks;

	public interface IBuildGenerator
	{
		string Name { get; }

		void GenerateBuild(Recipe recipe, string packageDirectory, string targetDirectory);

		Task GenerateDependenciesAsync(
			Recipe recipe,
			string packageDirectory,
			string targetDirectory,
			string binaryDirectory,
			string objectDirectory);
	}
}
