﻿// <copyright file="PackageLock.cs" company="Soup">
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
		private static string Property_Projects => "Projects";

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
		/// Gets or sets the table of projects
		/// </summary>
		public bool HasProjects()
		{
			return HasValue(_table, Property_Projects);
		}

		public IValueTable GetProjects()
		{
			if (!HasProjects())
				throw new InvalidOperationException("No projects.");

			var values = GetValue(_table, Property_Projects).AsTable();
			return values;
		}

		public void AddProject(string language, string name, string version)
		{
			var projects = EnsureHasTable(_table, Property_Projects);
			var projectLanguageList = EnsureHasList(projects, language);
			
			var projectTable = projectLanguageList.AddTableWithSyntax();
			projectTable.AddItemWithSyntax("Name", name);
			projectTable.AddItemWithSyntax("Version", version);
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
				var value = _table[name];
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
