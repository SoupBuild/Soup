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
		/// Gets or sets the table of projects
		/// </summary>
		public bool HasProjects()
		{
			return HasValue(_table, Property_Projects);
		}

		public IValueList GetProjects()
		{
			if (!HasProjects())
				throw new InvalidOperationException("No projects.");

			var values = GetValue(_table, Property_Projects).AsList();
			return values;
		}

		public void AddProject(string value)
		{
			var projects = EnsureHasList(_table, Property_Projects);

			projects.AddItemWithSyntax(value);
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
				table.Add(key, value);
			}
			else
			{
				table.Add(key, value);
			}

			return value;
		}

		private ValueList EnsureHasList(ValueTable table, string name)
		{
			if (table.ContainsKey(name))
			{
				var value = _table[name];
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
