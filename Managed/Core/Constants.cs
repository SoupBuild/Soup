// <copyright file="Constants.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
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
        /// <summary>
        /// Gets the app folder name
        /// </summary>
        public static string AppDataFolderName => "Soup";

        /// <summary>
        /// Gets the app data folder path
        /// </summary>
        public static string AppDataFolderPath => Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), AppDataFolderName);

        /// <summary>
        /// Gets the archive file extension
        /// </summary>
        public static string ArchiveFileExtension => ".tar.gz";

        /// <summary>
        /// Gets the store build folder name
        /// </summary>
        public static string StoreBuildFolderName => "Build";

        /// <summary>
        /// Gets the store include root folder name
        /// </summary>
        public static string StoreIncludeRootFolderName => "Include";

        /// <summary>
        /// Gets the store include folder name
        /// </summary>
        public static string StoreSoupIncludeFolderName => "Soup";

        /// <summary>
        /// Gets the store package folder name
        /// </summary>
        public static string StorePackageFolderName => "Package";

        /// <summary>
        /// Gets the store library folder name
        /// </summary>
        public static string StoreLibraryFolderName => "Library";

        /// <summary>
        /// Gets the project generation folder name
        /// </summary>
        public static string ProjectGenerateFolderName => ".soup";

        /// <summary>
        /// Gets the definition file name for a Soup Recipe
        /// </summary>
        public static string RecipeFileName => "Recipe.json";

        /// <summary>
        /// Gets the settings file name for a Build State
        /// </summary>
        public static string BuildStateFileName => "BuildState.json";

#if LOCAL
        /// <summary>
        /// Gets the local URL for the Soup identity service
        /// </summary>
        public static string SoupIdentityUrl => "http://localhost:3002";

#else
        /// <summary>
        /// Gets the production URL for the Soup identity service
        /// </summary>
        public static string SoupIdentityUrl => "https://soupid.azurewebsites.net";
#endif
#if LOCAL
        /// <summary>
        /// Gets the local version of the soup api url
        /// </summary>
        public static string SoupApiUrl => "http://localhost:3001";

#else
        /// <summary>
        /// Gets the production version of the soup api url
        /// </summary>
        public static string SoupApiUrl => "https://soupapi.azurewebsites.net";
#endif

        /// <summary>
        /// Gets the REST api endpoint version 1
        /// </summary>
        public static string SoupRESTEndpointV1 => $"{SoupApiUrl}/v1";

        /// <summary>
        /// Gets the staging folder name
        /// </summary>
        public static string StagingFolderName => ".staging";

        /// <summary>
        /// Gets the library target name format
        /// </summary>
        public static string LibraryTargetNameFormat => "{0}_{1}_{2}_{3}";
    }
}
