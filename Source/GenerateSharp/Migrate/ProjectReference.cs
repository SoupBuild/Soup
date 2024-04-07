// <copyright file="CSProjParser.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Path = Opal.Path;

namespace Soup.Build.Migrate;

public class ProjectReference
{
	public Path? Include { get; set; }

	public ProjectReference()
	{
	}
}