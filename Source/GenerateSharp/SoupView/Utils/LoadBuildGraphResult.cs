// <copyright file="LoadBuildGraphResult.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.View;

public class LoadBuildGraph
{
	public required int RootPackageGraphId { get; init; }
	public required IList<PackageGraph> PackageGraphs { get; init; }
	public required IList<PackageInfo> Packages { get; init; }
}

public class LoadBuildGraphResult
{
	public bool IsSuccess { get; set; } = false;
	public string? Message { get; set; }
	public required LoadBuildGraph? Graph { get; set; }
}
