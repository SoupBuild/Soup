// <copyright file="PackageProvider.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using Soup.Build;
using Opal;

namespace Soup.View;

using PackageId = Int32;
using PackageGraphId = Int32;
using PackageChildrenMap = IDictionary<string, IList<PackageChildInfo>>;
using PackageGraphLookupMap = IDictionary<int, PackageGraph>;
using PackageLookupMap = IDictionary<int, PackageInfo>;

public class PackageChildInfo
{
	public required string OriginalReference { get; set; }
	public bool IsSubGraph { get; set; } = false;
	public PackageId? PackageId { get; set; }
	public PackageGraphId? PackageGraphId { get; set; }
}

public class PackageInfo
{
	public PackageId Id { get; set; }
	public required string Name { get; set; }
	public required bool IsPrebuilt { get; set; }
	public required string PackageRoot { get; set; }
	public required string TargetDirectory { get; set; }
	public required PackageChildrenMap Dependencies { get; set; }
}

public class PackageGraph
{
	public PackageGraphId Id { get; set; }
	public PackageId RootPackageId { get; set; }
	// TODO: public ValueTable GlobalParameters { get; set; }
}

/// <summary>
/// The package provider that maintains the in memory representation of all build state
/// </summary>
public class PackageProvider
{
	public required PackageGraphId RootPackageGraphId { get; init; }
	public required PackageGraphLookupMap PackageGraphLookup { get; init; }
	public required PackageLookupMap PackageLookup { get; init; }

	public PackageGraph GetRootPackageGraph()
	{
		return GetPackageGraph(RootPackageGraphId);
	}

	public PackageGraph GetPackageGraph(PackageGraphId packageGraphId)
	{
		// The PackageGraph must already be loaded
		if (PackageGraphLookup.TryGetValue(packageGraphId, out var result))
		{
			return result;
		}
		else
		{
			throw new InvalidOperationException($"PackageGraphId [{packageGraphId}] not found in lookup");
		}
	}

	public PackageInfo GetPackageInfo(PackageId packageId)
	{
		// The PackageInfo must already be loaded
		if (PackageLookup.TryGetValue(packageId, out var result))
		{
			return result;
		}
		else
		{
			throw new InvalidOperationException($"packageId [{packageId}] not found in lookup");
		}
	}
}
