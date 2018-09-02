// <copyright company="Soup" file="Constants.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

#define LOCAL

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

		public static string StoreIncludeRootFolderName => "Include";

		public static string StoreSoupIncludeFolderName => "Soup";


		public static string StorePackageFolderName => "Package";

		public static string StoreLibraryFolderName => "Library";

		public static string ProjectGenerateFolderName => ".soup";

		public static string RecipeFileName => "Recipe.json";
#if LOCAL
		public static string SoupIdentityUrl => "http://localhost:3002";
#else
		public static string SoupIdentityUrl => "https://soupid.azurewebsites.net";
#endif
#if LOCAL
		public static string SoupApiUrl => "http://localhost:3001";
#else
		public static string SoupApiUrl => "https://soupapi.azurewebsites.net";
#endif
		public static string SoupRESTEndpointV1 => $"{SoupApiUrl}/v1";

		public static string StagingFolderName => ".staging";

		public static string LibraryTargetNameFormat => "{0}_{1}_{2}_{3}";
	}
}
