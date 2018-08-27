// <copyright company="Soup" file="InstallOptions.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using CommandLine;

	/// <summary>
	/// Install Command
	/// </summary>
	[Verb("install")]
	internal class InstallOptions
	{
		public string Package { get; set; }

		/// <summary>
		/// Show the usage details for this command
		/// </summary>
		private void ShowUsage()
		{
			Log.Info("");
			Log.Info("Usage: soup install <package_file>");
			Log.Info("\tpackage_file: Must be a zip file.");
		}
	}
}
