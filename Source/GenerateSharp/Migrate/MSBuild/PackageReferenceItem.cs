// <copyright file="PackageReference.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build.Migrate;

public class PackageReferenceItem
{
	public string? Include { get; set; }

	public string? Version { get; set; }
}