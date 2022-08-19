// <copyright file="PackageLock.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Runtime;
using System;
using Tomlyn.Syntax;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The package lock container
	/// </summary>
	public class PackageLock
	{
		private static string Property_Version => "Version";
		private static string Property_Closures => "Closures";

		private ValueTable _table;

		private DocumentSyntax? _mirrorSyntax;

		/// <summary>
		/// Initializes a new instance of the <see cref="PackageLock"/> class.
		/// </summary>
		public PackageLock()
		{
			_table = new ValueTable();
			_mirrorSyntax = new DocumentSyntax();
			_table.MirrorSyntax = _mirrorSyntax;
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="PackageLock"/> class.
		/// </summary>
		public PackageLock(ValueTable table)
		{
			_table = table;
			_mirrorSyntax = table.MirrorSyntax as DocumentSyntax;
		}

		/// <summary>
		/// Gets or sets the version
		/// </summary>
		public bool HasVersion()
		{
			return HasValue(_table, Property_Version);
		}

		public long GetVersion()
		{
			if (!HasVersion())
				throw new InvalidOperationException("No version.");

			var value = GetValue(_table, Property_Version).AsInteger();
			return value;
		}

		public void SetVersion(long value)
		{
			_table.AddItemWithSyntax(Property_Version, value);
		}

		/// <summary>
		/// Gets or sets the table of closures
		/// </summary>
		public bool HasClosures()
		{
			return HasValue(_table, Property_Closures);
		}

		public IValueTable GetClosures()
		{
			if (!HasClosures())
				throw new InvalidOperationException("No closures.");

			var values = GetValue(_table, Property_Closures).AsTable();
			return values;
		}

		public void EnsureClosure(string closure)
		{
			var closures = EnsureHasTable(_table, Property_Closures);
			_ = EnsureHasTable(closures, closure);
		}

		public void AddProject(string closure, string language, string name, string version, string buildClosure)
		{
			var closures = EnsureHasTable(_table, Property_Closures);
			var closureTable = EnsureHasTable(closures, closure);
			var projectLanguageList = EnsureHasList(closureTable, language);
			
			var projectTable = projectLanguageList.AddTableWithSyntax();
			projectTable.AddItemWithSyntax("Name", name);
			projectTable.AddItemWithSyntax("Version", version);
			if (buildClosure != null)
			{
				projectTable.AddItemWithSyntax("Build", buildClosure);
			}
		}

		/// <summary>
		/// Raw access
		/// </summary>
		public ValueTable Table => _table;

		public DocumentSyntax? MirrorSyntax => _mirrorSyntax;

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
				table[key] = value;
			}
			else
			{
				table.Add(key, value);
			}

			return value;
		}

		private ValueTable EnsureHasTable(ValueTable table, string name)
		{
			if (table.ContainsKey(name))
			{
				var value = table[name];
				if (value.Type != ValueType.Table)
					throw new InvalidOperationException($"The package lock already has a non-table dependencies property: {name}");

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
				var value = table[name];
				if (value.Type != ValueType.List)
					throw new InvalidOperationException($"The package lock already has a non-list {name} property");

				// Find the Syntax for the table
				return (ValueList)value.AsList();
			}
			else
			{
				// Create a new list
				return table.AddListWithSyntax(name);
			}
		}
	}
}
