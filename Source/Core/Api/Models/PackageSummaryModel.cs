// <copyright file="PackageSummaryModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api
{
    using Newtonsoft.Json;

    /// <summary>
    /// A container for the package summary result
    /// </summary>
    public class PackageSummaryModel
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
