// <copyright company="Soup" file="IBuildGenerator.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.Threading.Tasks;

	public interface IBuildGenerator
	{
		string Name { get; }

		void GenerateBuild(
			Recipe recipe,
			string targetDirectory,
			string packageDirectory,
			string binaryDirectory,
			string objectDirectory);

		Task GenerateDependenciesAsync(
			Recipe recipe,
			string targetDirectory);
	}
}
