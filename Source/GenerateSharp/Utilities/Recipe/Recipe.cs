// <copyright file="Recipe.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;

namespace Soup.Build.Utilities;

/// <summary>
/// The recipe container
/// </summary>
public class Recipe
{
	public static string Property_Build => "Build";
	public static string Property_Tool => "Tool";
	private static string Property_Dependencies => "Dependencies";
	private static string Property_Runtime => "Runtime";
	private static string Property_Language => "Language";
	private static string Property_Name => "Name";
	private static string Property_Version => "Version";
	private static string Property_Reference => "Reference";
	private static string Property_Type => "Type";
	private static string Property_Source => "Source";

	private readonly SMLDocument _document;

	/// <summary>
	/// Initializes a new instance of the <see cref="Recipe"/> class.
	/// </summary>
	public Recipe()
	{
		_document = new SMLDocument();
		Name = string.Empty;
		Language = new LanguageReference();
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="Recipe"/> class.
	/// </summary>
	public Recipe(
		string name,
		LanguageReference language)
	{
		_document = new SMLDocument();
		Name = name;
		Language = language;
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="Recipe"/> class.
	/// </summary>
	public Recipe(SMLDocument document)
	{
		_document = document;

		if (!HasValue(_document, Property_Name))
			throw new ArgumentException("Missing required property Name");
		if (!HasValue(_document, Property_Language))
			throw new ArgumentException("Missing required property Language");
	}

	/// <summary>
	/// Gets or sets the package name
	/// </summary>
	public SMLValue NameValue => GetValue(_document, Property_Name);

	public string Name
	{
		get { return NameValue.AsString().Value; }
		set { EnsureHasValue(_document, Property_Name, value); }
	}

	/// <summary>
	/// Gets or sets the package language
	/// </summary>
	public SMLValue LanguageValue => GetValue(_document, Property_Language);

	public LanguageReference Language
	{
		get { return LanguageReference.Parse(LanguageValue.AsString().Value); }
		set { EnsureHasValue(_document, Property_Language, value.ToString()); }
	}

	/// <summary>
	/// Gets or sets the package version
	/// </summary>
	public bool HasVersion => HasValue(_document, Property_Version);

	public SemanticVersion Version
	{
		get
		{
			if (!HasVersion)
				throw new InvalidOperationException("No version.");

			return SemanticVersion.Parse(
				GetValue(_document, Property_Version).AsString().Value);
		}
		set
		{
			EnsureHasValue(_document, Property_Version, value.ToString());
		}
	}

	/// <summary>
	/// Gets or sets the package type
	/// </summary>
	public bool HasType => HasValue(_document, Property_Type);

	public string Type
	{
		get
		{
			if (!HasType)
				throw new InvalidOperationException("No type.");

			return GetValue(_document, Property_Type).AsString().Value;
		}
		set
		{
			EnsureHasValue(_document, Property_Type, value);
		}
	}

	/// <summary>
	/// Gets or sets the list of named dependency packages
	/// </summary>
	public IList<string> GetDependencyTypes()
	{
		var result = new List<string>();
		if (HasDependencies())
		{
			foreach (var value in GetDependencies().Values)
			{
				result.Add(value.Key);
			}
		}

		return result;
	}

	public bool HasNamedDependencies(string name)
	{
		return HasDependencies() && HasValue(GetDependencies(), name);
	}

	public IList<PackageReference> GetNamedDependencies(string name)
	{
		if (!HasNamedDependencies(name))
			throw new InvalidOperationException("No named dependencies.");

		var values = GetValue(GetDependencies(), name).AsArray();
		var result = new List<PackageReference>();
		foreach (var value in values.Values)
		{
			// A dependency can either be a string or a table with reference key
			if (value.Value.Type == SMLValueType.String)
			{
				result.Add(PackageReference.Parse(value.Value.AsString().Value));
			}
			else if (value.Value.Type == SMLValueType.Table)
			{
				var valueTable = value.Value.AsTable();
				if (!HasValue(valueTable, Property_Reference))
					throw new InvalidOperationException("Recipe dependency table missing required Reference value.");
				var referenceValue = GetValue(valueTable, Property_Reference);
				if (referenceValue.Type == SMLValueType.String)
				{
					result.Add(PackageReference.Parse(referenceValue.AsString().Value));
				}
				else
				{
					throw new InvalidOperationException("Recipe dependency Reference must be type String.");
				}
			}
			else
			{
				throw new InvalidOperationException("Unknown Recipe dependency type.");
			}
		}

		return result;
	}

	public void AddSource(string value)
	{
		var sourceList = EnsureHasList(_document, Property_Source);
		sourceList.AddItemWithSyntax(value, 1);
	}

	public void AddNamedDependency(string type, string value)
	{
		var dependencies = EnsureHasTable(_document, Property_Dependencies);
		var runtimeDependencies = EnsureHasList(dependencies, type);

		runtimeDependencies.AddItemWithSyntax(value, 2);
	}

	public void AddRuntimeDependency(string value)
	{
		AddNamedDependency(Property_Runtime, value);
	}

	/// <summary>
	/// Gets or sets the list of runtime dependency packages
	/// </summary>
	public bool HasRuntimeDependencies => HasNamedDependencies(Property_Runtime);

	public IList<PackageReference> RuntimeDependencies
	{
		get { return GetNamedDependencies(Property_Runtime); }
	}

	/// <summary>
	/// Gets or sets the list of build dependency packages
	/// </summary>
	public bool HasBuildDependencies => HasNamedDependencies(Property_Build);

	public IList<PackageReference> BuildDependencies
	{
		get { return GetNamedDependencies(Property_Build); }
	}

	/// <summary>
	/// Gets or sets the list of tool dependency packages
	/// </summary>
	public bool HasToolDependencies => HasNamedDependencies(Property_Tool);

	public IList<PackageReference> ToolDependencies
	{
		get { return GetNamedDependencies(Property_Tool); }
	}

	/// <summary>
	/// Raw access
	/// </summary>
	public SMLDocument Document => _document;

	/// <summary>
	/// Gets or sets the table of dependency packages
	/// </summary>
	private bool HasDependencies()
	{
		return HasValue(_document, Property_Dependencies);
	}

	private SMLTable GetDependencies()
	{
		if (!HasDependencies())
			throw new InvalidOperationException("No dependencies.");

		var values = GetValue(_document, Property_Dependencies).AsTable();
		return values;
	}

	private static void EnsureHasValue(SMLDocument document, string name, string value)
	{
		if (document.Values.TryGetValue(name, out var existingValue))
		{
			if (existingValue.Value.Type != SMLValueType.String)
				throw new InvalidOperationException("The recipe already has a non-string property");

			// Find the Syntax for the table
			var stringValue = existingValue.Value.AsString();
			stringValue.Value = value;
			stringValue.Content.Text = value;
		}
		else
		{
			// Create a new table
			document.AddItemWithSyntax(name, value);
		}
	}

	private static SMLTable EnsureHasTable(SMLDocument document, string name)
	{
		if (document.Values.TryGetValue(name, out var value))
		{
			if (value.Value.Type != SMLValueType.Table)
				throw new InvalidOperationException("The recipe already has a non-table dependencies property");

			// Find the Syntax for the table
			return value.Value.AsTable();
		}
		else
		{
			// Create a new table
			return document.AddTableWithSyntax(name);
		}
	}

	private static SMLArray EnsureHasList(SMLTable table, string name)
	{
		if (table.Values.TryGetValue(name, out var value))
		{
			if (value.Value.Type != SMLValueType.Array)
				throw new InvalidOperationException("The recipe already has a non-list dependencies property");

			// Find the Syntax for the table
			return value.Value.AsArray();
		}
		else
		{
			// Create a new list
			return table.AddArrayWithSyntax(name, 1);
		}
	}

	private static SMLArray EnsureHasList(SMLDocument document, string name)
	{
		if (document.Values.TryGetValue(name, out var value))
		{
			if (value.Value.Type != SMLValueType.Array)
				throw new InvalidOperationException("The recipe already has a non-list dependencies property");

			// Find the Syntax for the table
			return value.Value.AsArray();
		}
		else
		{
			// Create a new list
			return document.AddArrayWithSyntax(name);
		}
	}

	private static bool HasValue(SMLTable table, string key)
	{
		return table.Values.ContainsKey(key);
	}

	private static bool HasValue(SMLDocument document, string key)
	{
		return document.Values.ContainsKey(key);
	}

	private static SMLValue GetValue(SMLTable table, string key)
	{
		if (table.Values.TryGetValue(key, out var value))
		{
			return value.Value;
		}
		else
		{
			throw new InvalidOperationException("Requested recipe value does not exist in the table.");
		}
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
}