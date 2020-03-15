// <copyright file="PackageCreateModel.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Api
{
    /// <summary>
    /// A class representing the package creation parameters
    /// </summary>
    export class PackageCreateModel
    {
    public:
        PackageCreateModel(
            std::string name) :
            _name(std::move(name)),
            _description()
        {
        }

        PackageCreateModel(
            std::string name,
            std::optional<std::string> description) :
            _name(std::move(name)),
            _description(std::move(description))
        {
        }

        /// <summary>
        /// Gets the name
        /// </summary>
        const std::string& GetName() const
        {
            return _name;
        }

        /// <summary>
        /// Gets a value indicating whether there is a description
        /// </summary>
        bool HasDescription() const
        {
            return _description.has_value();
        }

        /// <summary>
        /// Gets the description
        /// </summary>
        std::string GetDescription() const
        {
            if (!HasDescription())
                throw std::runtime_error("No description.");
            return _description.value();
        }

    private:
        std::string _name;
        std::optional<std::string> _description;
    };
}
