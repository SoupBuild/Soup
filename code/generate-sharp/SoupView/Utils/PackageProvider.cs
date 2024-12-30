// <copyright file="PackageProvider.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using PackageChildrenMap = System.Collections.Generic.IDictionary<string, System.Collections.Generic.IList<Soup.View.PackageChildInfo>>;
using PackageGraphId = System.Int32;
using PackageGraphLookupMap = System.Collections.Generic.IDictionary<int, Soup.View.PackageGraph>;
using PackageId = System.Int32;
using PackageLookupMap = System.Collections.Generic.IDictionary<int, Soup.View.PackageInfo>;

namespace Soup.View;

public class PackageChildInfo
{
	public string OriginalReference { get; set; } = string.Empty;
	public bool IsSubGraph { get; set; }
	public PackageId? PackageId { get; set; }
	public PackageGraphId? PackageGraphId { get; set; }
}

public class PackageInfo
{
	public PackageId Id { get; set; }
	public string Name { get; set; } = string.Empty;
	public string Owner { get; set; } = string.Empty;
	public bool IsPrebuilt { get; set; }
	public string PackageRoot { get; set; } = string.Empty;
	public string TargetDirectory { get; set; } = string.Empty;
	public PackageChildrenMap Dependencies { get; init; } = new Dictionary<string, IList<PackageChildInfo>>();
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
	public PackageGraphId RootPackageGraphId { get; set; }
	public PackageGraphLookupMap PackageGraphLookup { get; init; } = new Dictionary<int, PackageGraph>();
	public PackageLookupMap PackageLookup { get; init; } = new Dictionary<int, PackageInfo>();

	public PackageGraph GetRootPackageGraph()
	{
		return GetPackageGraph(this.RootPackageGraphId);
	}

	public PackageGraph GetPackageGraph(PackageGraphId packageGraphId)
	{
		// The PackageGraph must already be loaded
		if (this.PackageGraphLookup.TryGetValue(packageGraphId, out var result))
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
		if (this.PackageLookup.TryGetValue(packageId, out var result))
		{
			return result;
		}
		else
		{
			throw new InvalidOperationException($"packageId [{packageId}] not found in lookup");
		}
	}
}
