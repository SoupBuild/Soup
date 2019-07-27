// <copyright file="PackageUpdateModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api
{
    using Newtonsoft.Json;

    /// <summary>
    /// A class representing the package update parameters
    /// </summary>
    public class PackageUpdateModel
    {
        /// <summary>
        /// Gets or sets the description
        /// </summary>
        [JsonProperty("description")]
        public string Description { get; set; }
    }
}
