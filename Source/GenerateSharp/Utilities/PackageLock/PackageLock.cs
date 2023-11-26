// <copyright file="PackageLock.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities;

/// <summary>
/// The package lock container
/// </summary>
public class PackageLock
{
	public static string Property_Version => "Version";
	public static string Property_Name => "Name";
	private static string Property_Closures => "Closures";
	private static string Property_Build => "Build";
	private static string Property_Tool => "Tool";

	private SMLDocument _document;

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageLock"/> class.
	/// </summary>
	public PackageLock()
	{
		_document = new SMLDocument();
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="PackageLock"/> class.
	/// </summary>
	public PackageLock(SMLDocument table)
	{
		_document = table;
	}

	/// <summary>
	/// Gets or sets the version
	/// </summary>
	public bool HasVersion()
	{
		return HasValue(_document, Property_Version);
	}

	public long GetVersion()
	{
		if (!HasVersion())
			throw new InvalidOperationException("No version.");

		var value = GetValue(_document, Property_Version).AsInteger();
		return value.Value;
	}

	public void SetVersion(long value)
	{
		_document.AddItemWithSyntax(Property_Version, value);
	}

	/// <summary>
	/// Gets or sets the table of closures
	/// </summary>
	public bool HasClosures()
	{
		return HasValue(_document, Property_Closures);
	}

	public SMLTable GetClosures()
	{
		if (!HasClosures())
			throw new InvalidOperationException("No closures.");

		var values = GetValue(_document, Property_Closures).AsTable();
		return values;
	}

	public void EnsureClosure(string closure)
	{
		var closures = EnsureHasTable(_document, Property_Closures);
		_ = EnsureHasTable(closures, closure, 1);
	}

	public void AddProject(
		string closure,
		string language,
		string name,
		string version,
		string? buildClosure,
		string? toolClosure)
	{
		var closures = EnsureHasTable(_document, Property_Closures);
		var closureTable = EnsureHasTable(closures, closure, 1);
		var projectLanguageList = EnsureHasList(closureTable, language, 2);

		var projectTable = projectLanguageList.AddInlineTableWithSyntax(3);
		projectTable.AddInlineItemWithSyntax(Property_Name, name);
		projectTable.AddInlineItemWithSyntax(Property_Version, version);
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
	public SMLDocument Document => _document;

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

	private static SMLArray EnsureHasList(SMLTable table, string name, int indentLevel)
	{
		if (table.Values.TryGetValue(name, out var value))
		{
			if (value.Value.Type != SMLValueType.Array)
				throw new InvalidOperationException($"The package lock already has a non-list {name} property");

			// Find the Syntax for the table
			return value.Value.AsArray();
		}
		else
		{
			// Create a new list
			return table.AddArrayWithSyntax(name, indentLevel);
		}
	}
}