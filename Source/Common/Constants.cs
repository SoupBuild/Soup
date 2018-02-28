// <copyright company="Soup">
//        Copyright (c) Soup.  All rights reserved.
// </copyright>

using System;
using System.IO;

namespace Soup
{
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
		public static string RecipeFileName => "recipe.json";
#if DEBUG
		public static string SoupSiteUrl => "http://localhost:64993";
		// public static string SoupSiteUrl => "http://localhost.fiddler:64993";
#else
		public static string SoupSiteUrl => "https://soup.azurewebsites.net";
#endif
		public static string SoupRESTEndpoint => $"{SoupSiteUrl}/api/v1";
		public static string SoupRESTPublish => $"{SoupRESTEndpoint}/packages/publish";
		public static string StagingFolderName => ".staging";
		public static string VS2017ProjectName => "VS2017.vcxproj";
	}
}
