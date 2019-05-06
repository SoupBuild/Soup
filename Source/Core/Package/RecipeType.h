// <copyright file="RecipeType.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup
{
    /// <summary>
    /// The enumeration of recipe types
    /// </summary>
    export enum class RecipeType
    {
        /// <summary>
        /// Executable
        /// </summary>
        Executable,

        /// <summary>
        /// Library
        /// </summary>
        Library,
    };

    export std::string ToString(RecipeType value)
    {
        switch (value)
        {
            case RecipeType::Executable:
                return "Executable";
            case RecipeType::Library:
                return "Library";
            default:
                throw std::runtime_error("Unknown recipe type.");
        }
    }
}
