// <copyright company="Soup" file="ViewOptions.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Client
{
	using CommandLine;

	/// <summary>
	/// View Command Options
	/// </summary>
	[Verb("version")]
	internal class ViewOptions
	{
		/// <summary>
		/// Show the usage details for this command
		/// </summary>
		private static void ShowUsage()
		{
			Log.Message("");
			Log.Message("Usage: soup view <package>");
			Log.Message("\tpackage: The unique package name.");
		}
	}
}
