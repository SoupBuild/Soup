// <copyright file="Recipe.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

#pragma once
#include "PackageReference.h"
#include "RecipeType.h"
#include "SemanticVersion.h"

namespace Soup
{
    /// <summary>
    /// The recipe container
    /// </summary>
    export class Recipe
    {
    public:
        /// <summary>
        /// Initializes a new instance of the <see cref="Recipe"/> class.
        /// </summary>
        Recipe() :
            m_isDirty(false),
            m_name(),
            m_version(),
            m_type(std::nullopt),
            m_dependencies(std::nullopt),
            m_public(std::nullopt),
            m_source(std::nullopt)
        {
        }


        /// <summary>
        /// Initializes a new instance of the <see cref="Recipe"/> class.
        /// </summary>
        Recipe(
            std::string name,
            SemanticVersion version) :
            m_isDirty(false),
            m_name(std::move(name)),
            m_version(version),
            m_type(),
            m_dependencies(),
            m_public(),
            m_source()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Recipe"/> class.
        /// </summary>
        Recipe(
            std::string name,
            SemanticVersion version,
            std::optional<RecipeType> type,
            std::optional<std::vector<PackageReference>> dependencies,
            std::optional<std::string> publicFile,
            std::optional<std::vector<std::string>> source) :
            m_isDirty(false),
            m_name(std::move(name)),
            m_version(version),
            m_type(std::move(type)),
            m_dependencies(std::move(dependencies)),
            m_public(std::move(publicFile)),
            m_source(std::move(source))
        {
        }

        /// <summary>
        /// Gets a value indicating whether the content has changed or not
        /// </summary>
        bool IsDirty() const
        {
            return m_isDirty;
        }

        /// <summary>
        /// Gets or sets the package name
        /// </summary>
        const std::string& GetName() const
        {
            return m_name;
        }

        void SetName(std::string value)
        {
            if (value != m_name)
            {
                m_name = std::move(value);
                m_isDirty = true;
            }
        }

        /// <summary>
        /// Gets or sets the package version
        /// </summary>
        SemanticVersion GetVersion() const
        {
            return m_version;
        }

        void SetVersion(SemanticVersion value)
        {
            if (value != m_version)
            {
                m_version = value;
                m_isDirty = true;
            }
        }

        /// <summary>
        /// Gets or sets the package type
        /// </summary>
        bool HasType() const
        {
            return m_type.has_value();
        }

        RecipeType GetType() const
        {
            if (HasType())
            {
                return m_type.value();
            }
            else
            {
                return DefaultRecipeType;
            }
        }

        void SetType(RecipeType value)
        {
            if (!HasType() || m_type.value() != value)
            {
                m_type = value;
                m_isDirty = true;
            }
        }

        /// <summary>
        /// Gets or sets the list of dependency packages
        /// TODO: Observable?
        /// </summary>
        bool HasDependencies() const
        {
            return m_dependencies.has_value();
        }

        const std::vector<PackageReference>& GetDependencies() const
        {
            if (!HasDependencies())
                throw std::runtime_error("No dependencies.");
            return m_dependencies.value();
        }

        void SetDependencies(const std::vector<PackageReference>& value)
        {
            if (!HasDependencies() || m_dependencies.value() != value)
            {
                m_dependencies = value;
                m_isDirty = true;
            }
        }

        /// <summary>
        /// Gets or sets the public file
        /// </summary>
        bool HasPublic() const
        {
            return m_public.has_value();
        }

        const std::string& GetPublic() const
        {
            if (!HasPublic())
                throw std::runtime_error("No public.");
            return m_public.value();
        }

        void SetPublic(const std::string& value)
        {
            if (!HasPublic() || m_public.value() != value)
            {
                m_public = value;
                m_isDirty = true;
            }
        }

        /// <summary>
        /// Gets or sets the source values
        /// TODO: Observable?
        /// </summary>
        bool HasSource() const
        {
            return m_source.has_value();
        }

        const std::vector<std::string>& GetSource() const
        {
            if (!HasSource())
                throw std::runtime_error("No source.");
            return m_source.value();
        }

        void SetSource(const std::vector<std::string>& value)
        {
            if (!HasSource() || m_source.value() != value)
            {
                m_source = value;
                m_isDirty = true;
            }
        }

        /// <summary>
        /// Equality operator
        /// </summary>
        bool operator ==(const Recipe& rhs) const
        {
            return m_name == rhs.m_name &&
                m_version == rhs.m_version &&
                m_type == rhs.m_type &&
                m_dependencies == rhs.m_dependencies &&
                m_public == rhs.m_public &&
                m_source == rhs.m_source;
        }

        /// <summary>
        /// Inequality operator
        /// </summary>
        bool operator !=(const Recipe& rhs) const
        {
            return !(*this == rhs);
        }

    private:
        static const RecipeType DefaultRecipeType = RecipeType::Library;
        bool m_isDirty;
        std::string m_name;
        SemanticVersion m_version;
        std::optional<RecipeType> m_type;
        std::optional<std::vector<PackageReference>> m_dependencies;
        std::optional<std::string> m_public;
        std::optional<std::vector<std::string>> m_source;
    };
}
