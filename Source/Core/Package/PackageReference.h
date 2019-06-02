// <copyright file="PackageReference.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "Path.h"
#include "SemanticVersion.h"

namespace Soup
{
    /// <summary>
    /// A package reference object which will consist of a name version pair that
    /// refers to a published package or a path to a local recipe
    /// </summary>
    export class PackageReference
    {
    public:
        /// <summary>
        /// Try parse a package reference from the provided string
        /// </summary>
        static bool TryParse(const std::string& value, PackageReference& result)
        {
            // TODO: Invert try parse to be the default and parse to add the exception
            // Way faster on the failed case and this could eat OOM
            try
            {
                result = Parse(value);
                return true;
            }
            catch (...)
            {
            }

            result = PackageReference();
            return false;
        }

        /// <summary>
        /// Parse a package reference from the provided string.
        /// </summary>
        static PackageReference Parse(const std::string& value)
        {
            // Check if there is the @ separator
            auto separatorLocation = value.find('@');
            if (separatorLocation != std::string::npos)
            {
                // The package is a published reference
                auto name = value.substr(0, separatorLocation);
                auto versionString = value.substr(separatorLocation + 1);
                auto version = SemanticVersion::Parse(versionString);
                return PackageReference(std::move(name), version);
            }
            else
            {
                // Assume that this package is a relative path reference
                return PackageReference(Path(value));
            }
        }

    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="PackageReference"/> class.
        /// </summary>
        PackageReference() :
            m_name(""),
            m_version(),
            m_path("")
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PackageReference"/> class.
        /// </summary>
        PackageReference(std::string name, SemanticVersion version) :
            m_name(std::move(name)),
            m_version(version),
            m_path()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="PackageReference"/> class.
        /// </summary>
        PackageReference(Path path) :
            m_name(""),
            m_version(),
            m_path(std::move(path))
        {
        }

        /// <summary>
        /// Gets a value indicating whether the reference is local or not
        /// </summary>
        bool IsLocal() const
        {
            return m_name.empty();
        }

        /// <summary>
        /// Gets or sets the Name.
        /// </summary>
        const std::string& GetName() const
        {
            return m_name;
        }

        /// <summary>
        /// Gets or sets the Version.
        /// </summary>
        const SemanticVersion& GetVersion() const
        {
            return m_version;
        }

        /// <summary>
        /// Gets or sets the Path.
        /// </summary>
        const Path& GetPath() const
        {
            return m_path;
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const PackageReference& rhs) const
        {
            return m_name == rhs.m_name &&
                m_version == rhs.m_version &&
                m_path == rhs.m_path;
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        bool operator !=(const PackageReference& rhs) const
        {
            return m_name != rhs.m_name ||
                m_version != rhs.m_version ||
                m_path != rhs.m_path;
        }

        /// <summary>
        /// Convert to string
        /// </summary>
        std::string ToString() const
        {
            // If the reference is a path then just return that
            if (IsLocal())
            {
                return m_path.ToString();
            }
            else
            {
                // Build up the name/version reference
                // "{Name}@{Version}"
                std::stringstream stringBuilder;
                stringBuilder << m_name << '@';
                stringBuilder << m_version.ToString();
                return stringBuilder.str();
            }
        }

    private:
        std::string m_name;
        SemanticVersion m_version;
        Path m_path;
    };
}
