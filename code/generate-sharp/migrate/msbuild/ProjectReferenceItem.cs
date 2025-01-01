// <copyright file="ProjectReference.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Path = Opal.Path;

namespace Soup.Build.Migrate;

public class ProjectReferenceItem
{
	public Path? Include { get; set; }
}