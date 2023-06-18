// <copyright file="NugetPackageVersion.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Swhere.Core.Nuget
{
	internal class NugetPackageVersion
	{
		public string Version { get; set; } = string.Empty;

		public IList<NugetPackageTargetFramework> TargetFrameworks { get; set; } = new List<NugetPackageTargetFramework>();
	}
}
