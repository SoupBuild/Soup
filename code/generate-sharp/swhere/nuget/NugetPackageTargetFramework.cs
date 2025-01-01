// <copyright file="NugetPackageTargetFramework.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Swhere.Core.Nuget;

public class NugetPackageTargetFramework
{
	public string Name { get; set; } = string.Empty;

	public IList<NugetPackageDependency> Dependencies { get; init; } = [];

	public IList<string> Libraries { get; init; } = [];
}