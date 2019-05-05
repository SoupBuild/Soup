// <copyright file="SemanticVersion.hs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>
#pragma once

namespace Soup
{
    /// <summary>
    /// The semantic version class
    /// </summary>
    export class SemanticVersion
    {
    public:
        /// <summary>
        /// Try parse the value
        /// </summary>
        static bool TryParse(const std::string& value, SemanticVersion& result)
        {
            // TODO: Invert this so TryParse is the default and Parse adds an exception
            // it is way faster in the fail case
            try
            {
                result = Parse(value);
                return true;
            }
            catch (...)
            {
            }

            result = SemanticVersion();
            return false;
        }

        /// <summary>
        /// Parse the value
        /// </summary>
        static SemanticVersion Parse(const std::string& value)
        {
            // Parse the integer values
            std::vector<int> values;
            auto stream = std::istringstream(value);
            std::string stringValue;
            while (std::getline(stream, stringValue, '.'))
            {
                auto intValue = std::stoi(stringValue);
                values.push_back(intValue);
            }

            if (values.size() != 3)
            {
                throw std::runtime_error("The version string must have three values.");
            }

            return SemanticVersion(
                values[0],
                values[1],
                values[2]);
        }

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="SemanticVersion"/> class.
        /// </summary>
        SemanticVersion() :
            m_major(0),
            m_minor(0),
            m_patch(0)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="SemanticVersion"/> class.
        /// </summary>
        SemanticVersion(short major, short minor, short patch) :
            m_major(major),
            m_minor(minor),
            m_patch(patch)
        {
        }

        /// <summary>
        /// Gets or sets the version major
        /// </summary>
        int GetMajor() const
        {
            return m_major;
        }

        /// <summary>
        /// Gets or sets the version minor
        /// </summary>
        int GetMinor() const
        {
            return m_minor;
        }

        /// <summary>
        /// Gets or sets the version patch
        /// </summary>
        int GetPatch() const
        {
            return m_patch;
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const SemanticVersion& rhs) const
        {
            return m_major == rhs.m_major &&
                m_minor == rhs.m_minor &&
                m_patch == rhs.m_patch;
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        bool operator !=(const SemanticVersion& rhs) const
        {
            return m_major != rhs.m_major ||
                m_minor != rhs.m_minor ||
                m_patch != rhs.m_patch;
        }

        /// <summary>
        /// Convert to string
        /// </summary>
        std::string ToString()
        {
            // "{Major}.{Minor}.{Patch}"
            std::stringstream stringBuilder;
            stringBuilder << m_major << ".";
            stringBuilder << m_minor << ".";
            stringBuilder << m_patch;
            return stringBuilder.str();
        }

    private:
        int m_major;
        int m_minor;
        int m_patch;
    };
}
