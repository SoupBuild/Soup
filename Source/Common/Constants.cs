// <copyright company="Soup" file="Constants.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup
{
	using System;
	using System.IO;

	/// <summary>
	/// The constants used throughout the application
	/// </summary>
	public static class Constants
	{
		public static string AppDataFolderName => "Soup";

		public static string AppDataFolderPath => Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), AppDataFolderName);

		public static string ArchiveFileExtension => ".tar.gz";

		public static string BuildFolderName => "Build";

		public static string IncludeFolderName => "Include";

		public static string PackageFolderName => "Package";

		public static string PackageIncludeFileName => "Include.props";

		public static string PackagePropertiesFileName => "Package.props";

		public static string ProjectGenerateFolderName => ".soup";

		public static string RecipeFileName => "Recipe.json";
#if DEBUG
		public static string SoupSiteUrl => "http://localhost:3001";
#else
		public static string SoupSiteUrl => "https://soupapi.azurewebsites.net";
#endif
		public static string SoupRESTEndpointV1 => $"{SoupSiteUrl}/v1";

		public static string StagingFolderName => ".staging";
	}
}
