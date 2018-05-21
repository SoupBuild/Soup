// <copyright company="Soup" file="IBuildEngine.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System.Threading.Tasks;

	public interface IBuildEngine
	{
		string Name { get; }

		Task GenerateBuildAsync(
			Recipe recipe,
			string targetDirectory,
			string packageDirectory,
			string binaryDirectory,
			string objectDirectory);

		Task GenerateDependenciesAsync(
			Recipe recipe,
			string targetDirectory);

		/// <summary>
		/// Build the provided package path
		/// Returns true if success, otherwise false
		/// </summary>s
		bool Build(
			string path,
			bool showOutput,
			bool debug);
	}
}
