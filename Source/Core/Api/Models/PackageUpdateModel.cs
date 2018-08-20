// <copyright company="Soup" file="PackageUpdateModel.cs">
//   Copyright (c) Soup.  All rights reserved.
// </copyright>

namespace Soup.Api
{
	using Newtonsoft.Json;

	/// <summary>
	/// A class representing the package update parameters
	/// </summary>
	public class PackageUpdateModel
	{
		[JsonProperty("description")]
		public string Description { get; set; }
	}
}
