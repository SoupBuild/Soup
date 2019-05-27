// <copyright file="SemanticVersion.hs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
#pragma once

namespace Soup
{
    /// <summary>
    /// A container for a path string
    /// </summary>
    export class Path
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Path"/> class.
        /// </summary>
        Path() :
            m_value()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Path"/> class.
        /// </summary>
        Path(std::string value) :
            m_value(std::move(value))
        {
            // Cleanup alternative path separators
            // std::replace(
            //     m_value.begin(),
            //     m_value.end(),
            //     AltDirectorySeparatorCharacter,
            //     DirectorySeparatorCharacter);
        }

        /// <summary>
        /// Gets a value indicating whether the path has a root
        /// </summary>
        bool HasRoot() const
        {
            return m_value.has_root_name();
        }

        /// <summary>
        /// Gets the path root
        /// </summary>
        std::string GetRoot() const
        {
            return m_value.root_name().string();
        }

        /// <summary>
        /// Gets a value indicating whether the path has a file name
        /// </summary>
        bool HasFileName() const
        {
            return m_value.has_filename();
        }

        /// <summary>
        /// Gets the file name
        /// </summary>
        std::string GetFileName() const
        {
            return m_value.filename().string();
        }

        /// <summary>
        /// Gets a value indicating whether the file name has an stem
        /// </summary>
        bool HasFileStem() const
        {
            return m_value.has_stem();
        }

        /// <summary>
        /// Gets the file name minus the extension
        /// </summary>
        std::string GetFileStem() const
        {
            return m_value.stem().string();
        }

        /// <summary>
        /// Gets a value indicating whether the file name has an extension
        /// </summary>
        bool HasFileExtension() const
        {
            return m_value.has_extension();
        }

        /// <summary>
        /// Gets the file extension
        /// </summary>
        std::string GetFileExtension() const
        {
            return m_value.extension().string();
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const Path& rhs) const
        {
            return m_value == rhs.m_value;
        }

        bool operator !=(const Path& rhs) const
        {
            return m_value != rhs.m_value;
        }

        /// <summary>
        /// Concatenate paths
        /// </summary>
        Path operator +(const Path& rhs) const
        {
            auto result = Path();
            result.m_value = m_value / rhs.m_value;
            return result;
        }

        /// <summary>
        /// Convert to string
        /// </summary>
        std::string ToString()
        {
            return m_value.string();
        }

    private:
        std::filesystem::path m_value;
    };
}
