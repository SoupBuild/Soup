// <copyright file="Constants.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

// #define LOCAL

namespace Soup
{
    /// <summary>
    /// The constants used throughout the application
    /// </summary>
    class Constants
    {
    public:
        /// <summary>
        /// Gets the app folder name
        /// </summary>
        static constexpr const char* AppDataFolderName = "Soup";

        /// <summary>
        /// Gets the app data folder path
        /// </summary>
        // static constexpr const char* AppDataFolderPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), AppDataFolderName);

        /// <summary>
        /// Gets the archive file extension
        /// </summary>
        static constexpr const char* ArchiveFileExtension = ".tar.gz";

        /// <summary>
        /// Gets the store build folder name
        /// </summary>
        static constexpr const char* StoreBuildFolderName = "Build";

        /// <summary>
        /// Gets the store include root folder name
        /// </summary>
        static constexpr const char* StoreIncludeRootFolderName = "Include";

        /// <summary>
        /// Gets the store include folder name
        /// </summary>
        static constexpr const char* StoreSoupIncludeFolderName = "Soup";

        /// <summary>
        /// Gets the store package folder name
        /// </summary>
        static constexpr const char* StorePackageFolderName = "Package";

        /// <summary>
        /// Gets the store library folder name
        /// </summary>
        static constexpr const char* StoreLibraryFolderName = "Library";

        /// <summary>
        /// Gets the project generation folder name
        /// </summary>
        static constexpr const char* ProjectGenerateFolderName = ".soup";

        /// <summary>
        /// Gets the settings file name for a Soup Recipe
        /// </summary>
        static constexpr const char* RecipeFileName = "Recipe.json";

        /// <summary>
        /// Gets the settings file name for a Build State
        /// </summary>
        static constexpr const char* BuildStateFileName = "BuildState.json";

#if LOCAL
        /// <summary>
        /// Gets the local URL for the Soup identity service
        /// </summary>
        static constexpr const char* SoupIdentityUrl = "http://localhost:3002";

#else
        /// <summary>
        /// Gets the production URL for the Soup identity service
        /// </summary>
        static constexpr const char* SoupIdentityUrl = "https://soupid.azurewebsites.net";
#endif
#if LOCAL
        /// <summary>
        /// Gets the local version of the soup api url
        /// </summary>
        static constexpr const char* SoupApiUrl = "http://localhost:3001";

#else
        /// <summary>
        /// Gets the production version of the soup api url
        /// </summary>
        static constexpr const char* SoupApiUrl = "https://soupapi.azurewebsites.net";
#endif

        /// <summary>
        /// Gets the REST api endpoint version 1
        /// </summary>
        static constexpr const char* SoupRESTEndpointV1 = "{SoupApiUrl}/v1";

        /// <summary>
        /// Gets the staging folder name
        /// </summary>
        static constexpr const char* StagingFolderName = ".staging";

        /// <summary>
        /// Gets the library target name format
        /// </summary>
        static constexpr const char* LibraryTargetNameFormat = "{0}_{1}_{2}_{3}";
    };
}
