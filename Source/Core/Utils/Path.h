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
        static constexpr char DirectorySeparatorCharacter = '/';
        static constexpr char VolumeSeparatorCharacter = ':';

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
        }

        /// <summary>
        /// Gets or sets the version major
        /// </summary>
        std::string GetFileName() const
        {
            return m_value;
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const Path& rhs) const
        {
            return m_value == rhs.m_value;
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        bool operator !=(const Path& rhs) const
        {
            return m_value != rhs.m_value;
        }

        /// <summary>
        /// Convert to string
        /// </summary>
        std::string ToString()
        {
            return m_value;
        }

    private:
        std::string m_value;
    };
}
