// <copyright company="Soup" file="BuildEngine.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.MSBuild
{
	using System;
	using System.Collections.Generic;
	using System.IO;
	using System.Text;
	using System.Threading.Tasks;
	using System.Xml.Serialization;

	/// <summary>
	/// The MSBuild Engine
	/// </summary>
	public class BuildEngine : IBuildEngine
	{
		public string Name => "MSBuild";

		public Task GenerateBuildAsync(
			Recipe recipe,
			string targetDirectory,
			string packageDirectory,
			string binaryDirectory,
			string objectDirectory)
		{
			return MSBuildGenerator.GenerateBuildAsync(
				recipe,
				targetDirectory,
				packageDirectory,
				binaryDirectory,
				objectDirectory);
		}

		public Task GenerateDependenciesAsync(
			Recipe recipe,
			string targetDirectory)
		{
			return MSBuildGenerator.GenerateDependenciesAsync(
				recipe,
				targetDirectory);
		}

		public bool Build(
			string buildPath,
			bool showOutput,
			bool debug)
		{
			return MSBuildRunner.Build(
				buildPath,
				showOutput,
				debug);
		}
	}
}
