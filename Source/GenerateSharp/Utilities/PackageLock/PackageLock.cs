// <copyright file="PackageLock.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using Opal;

namespace Soup.Build.Utilities;

/// <summary>
/// The package lock container
/// </summary>
public class PackageLock
{
	public static string Property_Version => "Version";
	private static string Property_Closures => "Closures";
	private static string Property_Build => "Build";
	private static string Property_Tool => "Tool";

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageLock"/> class.
	/// </summary>
	public PackageLock()
	{
		Document = new SMLDocument();
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageLock"/> class.
	/// </summary>
	public PackageLock(SMLDocument table)
	{
		Document = table;
	}

	/// <summary>
	/// Gets or sets the version
	/// </summary>
	public bool HasVersion()
	{
		return HasValue(Document, Property_Version);
	}

	public long GetVersion()
	{
		if (!HasVersion())
			throw new InvalidOperationException("No version.");

		var value = GetValue(Document, Property_Version).AsInteger();
		return value.Value;
	}

	public void SetVersion(long value)
	{
		Document.AddItemWithSyntax(Property_Version, value);
	}

	/// <summary>
	/// Gets or sets the table of closures
	/// </summary>
	public bool HasClosures()
	{
		return HasValue(Document, Property_Closures);
	}

	public SMLTable GetClosures()
	{
		if (!HasClosures())
			throw new InvalidOperationException("No closures.");

		var values = GetValue(Document, Property_Closures).AsTable();
		return values;
	}

	public void EnsureClosure(string closure)
	{
		var closures = EnsureHasTable(Document, Property_Closures);
		_ = EnsureHasTable(closures, closure, 1);
	}

	public void AddProject(
		Path workingDirectory,
		string closure,
		string language,
		PackageName uniqueName,
		PackageReference package,
		string? buildClosure,
		string? toolClosure)
	{
		var closures = EnsureHasTable(Document, Property_Closures);
		var closureTable = EnsureHasTable(closures, closure, 1);
		var projectLanguageTable = EnsureHasTable(closureTable, language, 2);

		var projectTable = projectLanguageTable.AddInlineTableWithSyntax(uniqueName.ToString(), 3);

		if (package.IsLocal)
		{
			var relativePath = package.Path.GetRelativeTo(workingDirectory);
			projectTable.AddInlineItemWithSyntax(Property_Version, relativePath.ToString());
		}
		else
		{
			if (package.Version is null)
				throw new InvalidOperationException("Missing version on external package reference");
			projectTable.AddInlineItemWithSyntax(Property_Version, package.Version);
		}

		if (buildClosure != null)
		{
			projectTable.AddInlineItemWithSyntax(Property_Build, buildClosure);
		}

		if (toolClosure != null)
		{
			projectTable.AddInlineItemWithSyntax(Property_Tool, toolClosure);
		}
	}

	/// <summary>
	/// Raw access
	/// </summary>
	public SMLDocument Document { get; }

	private static bool HasValue(SMLDocument document, string key)
	{
		return document.Values.ContainsKey(key);
	}

	private static SMLValue GetValue(SMLDocument document, string key)
	{
		if (document.Values.TryGetValue(key, out var value))
		{
			return value.Value;
		}
		else
		{
			throw new InvalidOperationException("Requested recipe value does not exist in the table.");
		}
	}

	private static SMLTable EnsureHasTable(SMLDocument document, string name)
	{
		if (document.Values.TryGetValue(name, out var value))
		{
			if (value.Value.Type != SMLValueType.Table)
				throw new InvalidOperationException($"The package lock already has a non-table dependencies property: {name}");

			// Find the Syntax for the table
			return value.Value.AsTable();
		}
		else
		{
			// Create a new table
			return document.AddTableWithSyntax(name);
		}
	}

	private static SMLTable EnsureHasTable(SMLTable table, string name, int indentLevel)
	{
		if (table.Values.TryGetValue(name, out var value))
		{
			if (value.Value.Type != SMLValueType.Table)
				throw new InvalidOperationException($"The package lock already has a non-table dependencies property: {name}");

			// Find the Syntax for the table
			return value.Value.AsTable();
		}
		else
		{
			// Create a new table
			return table.AddTableWithSyntax(name, indentLevel);
		}
	}
}