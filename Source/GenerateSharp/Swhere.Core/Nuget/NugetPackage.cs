// <copyright file="NugetPackage.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Swhere.Core.Nuget;

public class NugetPackage
{
	public string Id { get; set; } = string.Empty;

	public IList<NugetPackageVersion> Versions { get; init; } = [];
}
