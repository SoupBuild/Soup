// <copyright company="Soup" file="BuildGenerator.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Make
{
	using System;
	using System.Threading.Tasks;

	public class BuildGenerator : IBuildGenerator
	{
		public string Name => throw new NotImplementedException();

		public void GenerateBuild(Recipe recipe, string targetDirectory, string packageDirectory, string binaryDirectory, string objectDirectory)
		{
			throw new NotImplementedException();
		}

		public Task GenerateDependenciesAsync(Recipe recipe, string targetDirectory)
		{
			throw new NotImplementedException();
		}
	}
}
