// <copyright file="SDKConfig.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Path = Opal.Path;

namespace Soup.Build.Utilities;

/// <summary>
/// The SDK config container
/// </summary>
public class SDKConfig
{
	private static string Property_Name => "Name";
	private static string Property_SourceDirectories => "SourceDirectories";
	private static string Property_Properties => "Properties";

	/// <summary>
	/// Initializes a new instance of the <see cref="SDKConfig"/> class.
	/// </summary>
	public SDKConfig(SMLTable table)
	{
		Table = table;
	}

	/// <summary>
	/// Gets or sets the Name
	/// </summary>
	public bool HasName()
	{
		return Table.Values.ContainsKey(Property_Name);
	}

	public string Name
	{
		get
		{
			if (Table.Values.TryGetValue(Property_Name, out var nameValue))
			{
				return nameValue.Value.AsString().Value;
			}
			else
			{
				throw new InvalidOperationException("No Name.");
			}
		}
		set => Table.AddItemWithSyntax(Property_Name, value, 2);
	}

	/// <summary>
	/// Gets or sets the list of Source Directories
	/// </summary>
	public bool HasSourceDirectories()
	{
		return Table.Values.ContainsKey(Property_SourceDirectories);
	}

	[SuppressMessage("Usage", "CA2227:Collection properties should be read only", Justification = "Setter ensures format")]
	public IList<Path> SourceDirectories
	{
		get
		{
			if (Table.Values.TryGetValue(Property_SourceDirectories, out var sourceDirectoriesValue))
			{
				var values = sourceDirectoriesValue.Value.AsArray();
				var result = new List<Path>();
				foreach (var value in values.Values)
				{
					result.Add(new Path(value.Value.AsString().Value));
				}

				return result;
			}
			else
			{
				throw new InvalidOperationException("No SourceDirectories.");
			}
		}
		set
		{
			SMLArray? values;
			if (Table.Values.TryGetValue(Property_SourceDirectories, out var sourceDirectoriesValue))
			{
				values = sourceDirectoriesValue.Value.AsArray();
			}
			else
			{
				values = Table.AddArrayWithSyntax(Property_SourceDirectories, 2);
			}

			// Add the new syntax to the parent table syntax
			values.Values.Clear();

			foreach (var item in value)
			{
				values.AddItemWithSyntax(item.ToString(), 3);
			}
		}
	}

	/// <summary>
	/// Gets or sets the properties table
	/// </summary>
	public bool HasProperties()
	{
		return Table.Values.ContainsKey(Property_Properties);
	}

	public SMLTable Properties
	{
		get
		{
			if (Table.Values.TryGetValue(Property_Properties, out var propertiesValue))
			{
				return propertiesValue.Value.AsTable();
			}
			else
			{
				throw new InvalidOperationException("No Properties.");
			}
		}
	}

	public void SetProperties(IDictionary<string, string> value)
	{
		SMLTable? values;
		if (Table.Values.TryGetValue(Property_Properties, out var propertiesValues))
		{
			values = propertiesValues.Value.AsTable();
		}
		else
		{
			values = Table.AddTableWithSyntax(Property_Properties, 2);
		}

		// Add the new syntax to the parent table syntax
		values.Values.Clear();
		foreach (var item in value)
		{
			values.AddItemWithSyntax(item.Key, item.Value, 3);
		}
	}

	/// <summary>
	/// Raw access
	/// </summary>
	public SMLTable Table { get; }
}