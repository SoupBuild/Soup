// <copyright file="SearchModel.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Api
{
    using System.Collections.Generic;
    using Newtonsoft.Json;

    /// <summary>
    /// A container for the search result
    /// </summary>
    public class SearchModel
    {
        /// <summary>
        /// Gets or sets the total value
        /// </summary>
        [JsonProperty("total")]
        public int Total { get; set; }

        /// <summary>
        /// Gets or sets the list of packages
        /// </summary>
        [JsonProperty("packages")]
        public IList<PackageSummaryModel> Packages { get; set; }
    }
}
