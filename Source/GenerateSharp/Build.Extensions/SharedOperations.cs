// <copyright file="SharedOperations.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Opal.System;
using System;
using System.Collections.Generic;
using System.Reflection;

namespace Soup.Build
{
	/// <summary>
	/// The Shared Operations class
	/// </summary>
	public static class SharedOperations
	{
		/// <summary>
		/// Create a build operation that will copy a file
		/// </summary>
		public static BuildOperation CreateCopyFileOperation(
			Path workingDirectory,
			Path source,
			Path destination)
		{
			var title = $"Copy [{source}] -> [{destination}]";

			// TODO: var moduleName = LifetimeManager.Get<IProcessManager>().GetCurrentProcessFileName();
			var moduleFolder = new Path("C:/Program Files/SoupBuild/Soup/"); // TODO: moduleName.GetParent();

			var program = moduleFolder + new Path("copy.exe");
			var inputFiles = new List<Path>()
			{
				source,
			};
			var outputFiles = new List<Path>()
			{
				destination,
			};

			// Build the arguments
			var arguments = $"\"{source}\" \"{destination}\"";

			return new BuildOperation(
				title,
				workingDirectory,
				program,
				arguments,
				inputFiles,
				outputFiles);
		}

		/// <summary>
		/// Create a build operation that will create a directory
		/// </summary>
		public static BuildOperation CreateCreateDirectoryOperation(
			Path workingDirectory,
			Path directory)
		{
			if (directory.HasFileName())
				throw new ArgumentException("Cannot create a directory with a filename.");

			var title = $"MakeDir [{directory}]";

			// TODO: var moduleName = LifetimeManager.Get<IProcessManager>().GetCurrentProcessFileName();
			var moduleFolder = new Path("C:/Program Files/SoupBuild/Soup/"); // TODO: moduleName.GetParent();

			var program = moduleFolder + new Path("mkdir.exe");
			var inputFiles = new List<Path>();
			var outputFiles = new List<Path>()
			{
				directory,
			};

			// Build the arguments
			var arguments = $"\"{directory}\"";

			return new BuildOperation(
				title,
				workingDirectory,
				program,
				arguments,
				inputFiles,
				outputFiles);
		}

		/// <summary>
		/// Create a build operation that will write the content to a file
		/// </summary>
		public static BuildOperation CreateWriteFileOperation(
			Path workingDirectory,
			Path destination,
			string content)
		{
			if (!destination.HasFileName())
				throw new ArgumentException("Cannot create a file with from a directory.");

			var title = $"WriteFile [{destination}]";

			// Create the fake write file executable that will be executed in process
			var program = new Path("writefile.exe");
			var inputFiles = new List<Path>();
			var outputFiles = new List<Path>()
			{
				destination,
			};

			// Build the arguments
			var arguments = $"\"{destination}\" \"{content}\"";

			return new BuildOperation(
				title,
				workingDirectory,
				program,
				arguments,
				inputFiles,
				outputFiles);
		}
	}
}
