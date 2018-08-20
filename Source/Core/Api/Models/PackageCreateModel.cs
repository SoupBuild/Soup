// <copyright company="Soup" file="PackageCreateModel.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using Newtonsoft.Json;

	/// <summary>
	/// A class representing the package creation parameters
	/// </summary>
	public class PackageCreateModel
	{
		[JsonProperty("name")]
		public string Name { get; set; }

		[JsonProperty("description")]
		public string Description { get; set; }
	}
}
