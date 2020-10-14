// <copyright file="PackageCreateOrUpdateModel.h" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup::Api
{
    /// <summary>
    /// A class representing the package create or update parameters
    /// </summary>
    export class PackageCreateOrUpdateModel
    {
    public:
        PackageCreateOrUpdateModel() :
            _description()
        {
        }

        PackageCreateOrUpdateModel(
            std::optional<std::string> description) :
            _description(std::move(description))
        {
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
        std::optional<std::string> _description;
    };
}
