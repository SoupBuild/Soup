// <copyright file="Recipe.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using System;
using System.Collections.Generic;
using Tomlyn.Syntax;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The recipe container
	/// </summary>
	public class Recipe
	{
		private static string Property_Dependencies => "Dependencies";
		private static string Property_Runtime => "Runtime";
		private static string Property_Build => "Build";
		private static string Property_Test => "Test";
		private static string Property_Language => "Language";
		private static string Property_Name => "Name";
		private static string Property_Version => "Version";
		private static string Property_Reference => "Reference";

		private ValueTable _table;

		private DocumentSyntax? _mirrorSyntax;

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe()
		{
			_table = new ValueTable();
			_mirrorSyntax = null;
			Name = string.Empty;
			Language = string.Empty;
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe(
			string name,
			string language)
		{
			_table = new ValueTable();
			_mirrorSyntax = null;
			Name = name;
			Language = language;
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe(ValueTable table)
		{
			_table = table;
			_mirrorSyntax = table.MirrorSyntax as DocumentSyntax;

			if (!HasValue(_table, Property_Name))
				throw new ArgumentException("Missing required property Name");
			if (!HasValue(_table, Property_Language))
				throw new ArgumentException("Missing required property Language");
		}

		/// <summary>
		/// Gets or sets the package name
		/// </summary>
		public IValue NameValue => GetValue(_table, Property_Name);

		public string Name
		{
			get { return NameValue.AsString(); }
			set { EnsureValue(_table, Property_Name, new Value(value)); }
		}

		/// <summary>
		/// Gets or sets the package language
		/// </summary>
		public IValue LanguageValue => GetValue(_table, Property_Language);

		public string Language
		{
			get { return LanguageValue.AsString(); }
			set { EnsureValue(_table, Property_Language, new Value(value)); }
		}

		/// <summary>
		/// Gets or sets the package version
		/// </summary>
		public bool HasVersion => HasValue(_table, Property_Version);

		public SemanticVersion Version
		{
			get
			{
				if (!HasVersion)
					throw new InvalidOperationException("No version.");

				return SemanticVersion.Parse(
					GetValue(_table, Property_Version).AsString());
			}
			set
			{
				EnsureValue(_table, Property_Version, new Value(value.ToString()));
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
				foreach (var value in GetDependencies())
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

			var values = GetValue(GetDependencies(), name).AsList();
			var result = new List<PackageReference>();
			foreach (var value in values)
			{
				// A dependency can either be a string or a table with reference key
				if (value.IsString())
				{
					result.Add(PackageReference.Parse(value.AsString()));
				}
				else if (value.IsTable())
				{
					var valueTable = value.AsTable();
					if (!HasValue(valueTable, Property_Reference))
						throw new InvalidOperationException("Recipe dependency table missing required Reference value.");
					var referenceValue = GetValue(valueTable, Property_Reference);
					if (referenceValue.IsString())
					{
						result.Add(PackageReference.Parse(referenceValue.AsString()));
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

		public void AddRuntimeDependency(string value)
		{
			var dependencies = EnsureHasTable(_table, Property_Dependencies);
			var runtimeDependencies = EnsureHasList(dependencies, Property_Runtime);

			runtimeDependencies.AddItemWithSyntax(value);
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
		/// Gets or sets the list of test dependency packages
		/// </summary>
		public bool HasTestDependencies => HasNamedDependencies(Property_Test);

		public IList<PackageReference> TestDependencies
		{
			get { return GetNamedDependencies(Property_Test); }
		}

		/// <summary>
		/// Raw access
		/// </summary>
		public ValueTable Table => _table;

		public DocumentSyntax? MirrorSyntax => _mirrorSyntax;

		/// <summary>
		/// Gets or sets the table of dependency packages
		/// </summary>
		private bool HasDependencies()
		{
			return HasValue(_table, Property_Dependencies);
		}

		private IValueTable GetDependencies()
		{
			if (!HasDependencies())
				throw new InvalidOperationException("No dependencies.");

			var values = GetValue(_table, Property_Dependencies).AsTable();
			return values;
		}

		private ValueTable EnsureHasTable(ValueTable table, string name)
		{
			if (table.ContainsKey(name))
			{
				var value = _table[name];
				if (value.Type != ValueType.Table)
					throw new InvalidOperationException("The recipe already has a non-table dependencies property");

				// Find the Syntax for the table
				return (ValueTable)value.AsTable();
			}
			else
			{
				// Create a new table
				return table.AddTableWithSyntax(name);
			}
		}

		private ValueList EnsureHasList(ValueTable table, string name)
		{
			if (table.ContainsKey(name))
			{
				var value = _table[name];
				if (value.Type != ValueType.List)
					throw new InvalidOperationException("The recipe already has a non-list dependencies property");

				// Find the Syntax for the table
				return (ValueList)value.AsList();
			}
			else
			{
				// Create a new list
				return table.AddListWithSyntax(name);
			}
		}

		private bool HasValue(IValueTable table, string key)
		{
			return table.ContainsKey(key);
		}

		private IValue GetValue(IValueTable table, string key)
		{
			if (table.TryGetValue(key, out var value))
			{
				return value;
			}
			else
			{
				throw new InvalidOperationException("Requested recipe value does not exist in the table.");
			}
		}

		private IValue EnsureValue(IValueTable table, string key, IValue value)
		{
			if (table.ContainsKey(key))
			{
				table.Add(key, value);
			}
			else
			{
				table.Add(key, value);
			}

			return value;
		}
	}
}
