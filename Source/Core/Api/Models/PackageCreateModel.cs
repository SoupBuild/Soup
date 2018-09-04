// <copyright file="PackageCreateModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api
{
    using Newtonsoft.Json;

    /// <summary>
    /// A class representing the package creation parameters
    /// </summary>
    public class PackageCreateModel
    {
        /// <summary>
        /// Gets or sets the name
        /// </summary>
        [JsonProperty("name")]
        public string Name { get; set; }

        /// <summary>
        /// Gets or sets the description
        /// </summary>
        [JsonProperty("description")]
        public string Description { get; set; }
    }
}
