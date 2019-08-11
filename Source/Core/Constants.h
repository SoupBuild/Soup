﻿// <copyright file="Constants.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
// #define LOCAL

namespace Soup
{
    /// <summary>
    /// The constants used throughout the application
    /// </summary>
    export class Constants
    {
    public:
        /// <summary>
        /// Gets the app folder name
        /// </summary>
        static constexpr std::string_view AppDataFolderName = "Soup";

        /// <summary>
        /// Gets the Recipe file name
        /// </summary>
        static constexpr std::string_view RecipeFileName = "Recipe.json";

        /// <summary>
        /// Gets the app data folder path
        /// </summary>
        // static constexpr std::string_view AppDataFolderPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), AppDataFolderName);

        /// <summary>
        /// Gets the archive file extension
        /// </summary>
        static constexpr std::string_view ArchiveFileExtension = ".tar.gz";

        /// <summary>
        /// Gets the store build folder name
        /// </summary>
        static constexpr std::string_view StoreBuildFolderName = "Build";

        /// <summary>
        /// Gets the store include root folder name
        /// </summary>
        static constexpr std::string_view StoreIncludeRootFolderName = "Include";

        /// <summary>
        /// Gets the store include folder name
        /// </summary>
        static constexpr std::string_view StoreSoupIncludeFolderName = "Soup";

        /// <summary>
        /// Gets the store package folder name
        /// </summary>
        static constexpr std::string_view StorePackageFolderName = "Package";

        /// <summary>
        /// Gets the store library folder name
        /// </summary>
        static constexpr std::string_view StoreLibraryFolderName = "Library";

        /// <summary>
        /// Gets the project generation folder name
        /// </summary>
        static constexpr std::string_view ProjectGenerateFolderName = ".soup";

        /// <summary>
        /// Gets the project generation build folder name
        /// </summary>
        static constexpr std::string_view ProjectGenerateBuildFolderName = "Build";

        /// <summary>
        /// Gets the settings file name for a Soup Recipe
        /// </summary>

        /// <summary>
        /// Gets the settings file name for a Build State
        /// </summary>
        static constexpr std::string_view BuildStateFileName = "BuildState.json";

#if LOCAL
        /// <summary>
        /// Gets the local URL for the Soup identity service
        /// </summary>
        static constexpr std::string_view SoupIdentityUrl = "http://localhost:3002";

#else
        /// <summary>
        /// Gets the production URL for the Soup identity service
        /// </summary>
        static constexpr std::string_view SoupIdentityUrl = "https://soupid.azurewebsites.net";
#endif
#if LOCAL
        /// <summary>
        /// Gets the local version of the soup api url
        /// </summary>
        static constexpr std::string_view SoupApiUrl = "http://localhost:3001";

#else
        /// <summary>
        /// Gets the production version of the soup api url
        /// </summary>
        static constexpr std::string_view SoupApiUrl = "https://soupapi.azurewebsites.net";
#endif

        /// <summary>
        /// Gets the REST api endpoint version 1
        /// </summary>
        static constexpr std::string_view SoupRESTEndpointV1 = "{SoupApiUrl}/v1";

        /// <summary>
        /// Gets the staging folder name
        /// </summary>
        static constexpr std::string_view StagingFolderName = ".staging";

        /// <summary>
        /// Gets the library target name format
        /// </summary>
        static constexpr std::string_view LibraryTargetNameFormat = "{0}_{1}_{2}_{3}";
    };
}
