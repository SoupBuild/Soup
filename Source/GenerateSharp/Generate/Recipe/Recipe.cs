// <copyright file="Recipe.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using Soup.Build.Utilities;
using System;
using System.Collections.Generic;

namespace Soup.Build.Generate
{
	/// <summary>
	/// The recipe container
	/// </summary>
	internal class Recipe
	{
		private static string Property_Dependencies => "Dependencies";
		private static string Property_Runtime => "Runtime";
		private static string Property_Build => "Build";
		private static string Property_Test => "Test";
		private static string Property_Language => "Language";
		private static string Property_Name => "Name";
		private static string Property_Version => "Version";

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe()
		{
			_table = new ValueTable();
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
			Name = name;
			Language = language;
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe(
			string name,
			string language,
			SemanticVersion? version,
			IList<PackageReference>? runtimeDependencies,
			IList<PackageReference>? buildDependencies,
			IList<PackageReference>? testDependencies)
		{
			_table = new ValueTable();
			Name = name;
			Language = language;

			if (!ReferenceEquals(version, null))
				Version = version;

			if (!ReferenceEquals(runtimeDependencies, null))
				RuntimeDependencies = runtimeDependencies;

			if (!ReferenceEquals(buildDependencies, null))
				BuildDependencies = buildDependencies;

			if (!ReferenceEquals(testDependencies, null))
				TestDependencies = testDependencies;
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="Recipe"/> class.
		/// </summary>
		public Recipe(ValueTable table)
		{
			_table = table;

			if (!HasValue(_table, Property_Name))
				throw new ArgumentException("Missing required property Name");
			if (!HasValue(_table, Property_Language))
				throw new ArgumentException("Missing required property Language");
		}

		/// <summary>
		/// Gets or sets the package name
		/// </summary>
		public Value NameValue => GetValue(_table, Property_Name);

		public string Name
		{
			get { return NameValue.AsString(); }
			set { EnsureValue(_table, Property_Name, new Value(value)); }
		}

		/// <summary>
		/// Gets or sets the package language
		/// </summary>
		public Value LanguageValue => GetValue(_table, Property_Language);

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
				result.Add(PackageReference.Parse(value.AsString()));
			}

			return result;
		}

		public void SetNamedDependencies(string name, IList<PackageReference> values)
		{
			var stringValues = new ValueList();
			foreach (var value in values)
			{
				stringValues.Add(new Value(value.ToString()));
			}

			EnsureValue(EnsureDependencies(), name, new Value(stringValues));
		}

		/// <summary>
		/// Gets or sets the list of runtime dependency packages
		/// </summary>
		public bool HasRuntimeDependencies => HasNamedDependencies(Property_Runtime);

		public IList<PackageReference> RuntimeDependencies
		{
			get { return GetNamedDependencies(Property_Runtime); }
			set { SetNamedDependencies(Property_Runtime, value); }
		}

		/// <summary>
		/// Gets or sets the list of build dependency packages
		/// </summary>
		public bool HasBuildDependencies => HasNamedDependencies(Property_Build);

		public IList<PackageReference> BuildDependencies
		{
			get { return GetNamedDependencies(Property_Build); }
			set { SetNamedDependencies(Property_Build, value); }
		}

		/// <summary>
		/// Gets or sets the list of test dependency packages
		/// </summary>
		public bool HasTestDependencies => HasNamedDependencies(Property_Test);

		public IList<PackageReference> TestDependencies
		{
			get { return GetNamedDependencies(Property_Test); }
			set { SetNamedDependencies(Property_Test, value); }
		}

		/// <summary>
		/// Raw access
		/// </summary>
		public ValueTable GetTable()
		{
			return _table;
		}

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

		private IValueTable EnsureDependencies()
		{
			if (_table.ContainsKey(Property_Dependencies))
			{
				var value = _table[Property_Dependencies];
				switch (value.Type)
				{
					case ValueType.Table:
						// All good.
						return value.AsTable();
					case ValueType.Empty:
						var newTable = new ValueTable();
						_table.Add(Property_Dependencies, new Value(newTable));
						return newTable;
					default:
						throw new InvalidOperationException("The recipe already has a non-table dependencies property");
				}
			}
			else
            {
				var newTable = new ValueTable();
				_table.Add(Property_Dependencies, new Value(newTable));
				return newTable;
			}
		}

		private bool HasValue(IValueTable table, string key)
		{
			return table.ContainsKey(key);
		}

		private Value GetValue(IValueTable table, string key)
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

		private Value EnsureValue(IValueTable table, string key, Value value)
		{
			if (table.ContainsKey(key))
			{
				table[key] = value;
			}
			else
			{
				table.Add(key, value);
			}

			return value;
		}

		private ValueTable _table;
	}
}
