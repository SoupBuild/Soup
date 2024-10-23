// <copyright file="Recipe.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;

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

	/// <summary>
	/// Initializes a new instance of the <see cref="Recipe"/> class.
	/// </summary>
	public Recipe()
	{
		Document = new SMLDocument();
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
		Document = new SMLDocument();
		Name = name;
		Language = language;
	}

	/// <summary>
	/// Initializes a new instance of the <see cref="Recipe"/> class.
	/// </summary>
	public Recipe(SMLDocument document)
	{
		Document = document;

		if (!HasValue(Document, Property_Name))
			throw new ArgumentException("Missing required property Name");
		if (!HasValue(Document, Property_Language))
			throw new ArgumentException("Missing required property Language");
	}

	/// <summary>
	/// Gets or sets the package name
	/// </summary>
	public SMLValue NameValue => GetValue(Document, Property_Name);

	public string Name
	{
		get => NameValue.AsString().Value;
		set => Document.EnsureValueWithSyntax(Property_Name, value);
	}

	/// <summary>
	/// Gets or sets the package language
	/// </summary>
	public SMLValue LanguageValue => GetValue(Document, Property_Language);

	[SuppressMessage("Style", "IDE0072:Add missing cases", Justification = "Check specific types")]
	public LanguageReference Language
	{
		get => LanguageValue.Type switch
		{
			SMLValueType.String => LanguageReference.Parse(LanguageValue.AsString().Value),
			SMLValueType.LanguageReference => LanguageValue.AsLanguageReference().Value,
			_ => throw new InvalidOperationException("Language must be string or LanguageReference"),
		};
		set => Document.EnsureValueWithSyntax(Property_Language, value);
	}

	/// <summary>
	/// Gets or sets the package version
	/// </summary>
	public bool HasVersion => HasValue(Document, Property_Version);
	public SMLValue VersionValue => GetValue(Document, Property_Version);

	[SuppressMessage("Style", "IDE0072:Add missing cases", Justification = "Check specific types")]
	public SemanticVersion Version
	{
		get => VersionValue.Type switch
		{
			SMLValueType.String => SemanticVersion.Parse(VersionValue.AsString().Value),
			SMLValueType.Version => VersionValue.AsVersion().Value,
			_ => throw new InvalidOperationException("Version must be string or Version"),
		};
		set => Document.EnsureValueWithSyntax(Property_Version, value);
	}

	/// <summary>
	/// Gets or sets the package type
	/// </summary>
	public bool HasType => HasValue(Document, Property_Type);

	public string Type
	{
		get
		{
			if (!HasType)
				throw new InvalidOperationException("No type.");

			return GetValue(Document, Property_Type).AsString().Value;
		}
		set => Document.EnsureValueWithSyntax(Property_Type, value);
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
			else if (value.Value.Type == SMLValueType.PackageReference)
			{
				result.Add(value.Value.AsPackageReference().Value);
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
				throw new InvalidOperationException($"Unknown Recipe dependency type {value.Value.Type}.");
			}
		}

		return result;
	}

	public void AddSource(string value)
	{
		var sourceList = Document.EnsureArrayWithSyntax(Property_Source);
		sourceList.AddItemWithSyntax(value, 1);
	}

	public void AddNamedDependency(string type, string value)
	{
		var dependencies = Document.EnsureTableWithSyntax(Property_Dependencies);
		var runtimeDependencies = dependencies.EnsureArrayWithSyntax(type);

		runtimeDependencies.AddItemWithSyntax(value, 2);
	}

	public void AddBuildDependency(string value)
	{
		AddNamedDependency(Property_Build, value);
	}

	public void AddRuntimeDependency(string value)
	{
		AddNamedDependency(Property_Runtime, value);
	}

	/// <summary>
	/// Gets or sets the list of runtime dependency packages
	/// </summary>
	public bool HasRuntimeDependencies => HasNamedDependencies(Property_Runtime);

	public IList<PackageReference> RuntimeDependencies => GetNamedDependencies(Property_Runtime);

	/// <summary>
	/// Gets or sets the list of build dependency packages
	/// </summary>
	public bool HasBuildDependencies => HasNamedDependencies(Property_Build);

	public IList<PackageReference> BuildDependencies => GetNamedDependencies(Property_Build);

	/// <summary>
	/// Gets or sets the list of tool dependency packages
	/// </summary>
	public bool HasToolDependencies => HasNamedDependencies(Property_Tool);

	public IList<PackageReference> ToolDependencies => GetNamedDependencies(Property_Tool);

	/// <summary>
	/// Raw access
	/// </summary>
	public SMLDocument Document { get; }

	/// <summary>
	/// Gets or sets the table of dependency packages
	/// </summary>
	private bool HasDependencies()
	{
		return HasValue(Document, Property_Dependencies);
	}

	private SMLTable GetDependencies()
	{
		if (!HasDependencies())
			throw new InvalidOperationException("No dependencies.");

		var values = GetValue(Document, Property_Dependencies).AsTable();
		return values;
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