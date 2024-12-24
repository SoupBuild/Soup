// <copyright file="SourceSetUtilities.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using Path = Opal.Path;

namespace Soup.Build.Discover;

public static class SourceSetUtilities
{
	/// <summary>
	/// Add a new source path and cleanup sub folders
	/// </summary>
	public static void Add(HashSet<Path> sourceSet, Path value)
	{
		bool isIncluded = false;
		var current = value;
		while (true)
		{
			if (sourceSet.Contains(current))
			{
				isIncluded = true;
				break;
			}

			var next = current.GetParent();
			if (current == next)
				break;

			current = next;
		}

		if (!isIncluded)
		{
			// TODO: Remove already added values that this folder contains
			_ = sourceSet.Add(value);
		}
	}
}