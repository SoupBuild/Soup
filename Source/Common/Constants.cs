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

		public static string StoreBuildFolderName => "Build";

		public static string StoreIncludeFolderName => "Include";

		public static string StorePackageFolderName => "Package";

		public static string StoreLibraryFolderName => "Library";

		public static string ProjectGenerateFolderName => ".soup";

		public static string RecipeFileName => "Recipe.json";
#if LOCAL
		public static string SoupSiteUrl => "http://localhost:3001";
#else
		public static string SoupSiteUrl => "https://soupapi.azurewebsites.net";
#endif
		public static string SoupRESTEndpointV1 => $"{SoupSiteUrl}/v1";

		public static string StagingFolderName => ".staging";

		public static string LibraryTargetNameFormat => "{0}_{1}_{2}_{3}";
	}
}
