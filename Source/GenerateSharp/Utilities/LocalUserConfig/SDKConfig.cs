// <copyright file="SDKConfig.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The SDK config container
	/// </summary>
	public class SDKConfig
	{
		private static string Property_Name => "Name";
		private static string Property_SourceDirectories => "SourceDirectories";
		private static string Property_Properties => "Properties";

		private SMLTable _table;

		/// <summary>
		/// Initializes a new instance of the <see cref="SDKConfig"/> class.
		/// </summary>
		public SDKConfig() :
			this(new SMLTable())
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="SDKConfig"/> class.
		/// </summary>
		public SDKConfig(SMLTable table)
		{
			_table = table;
		}

		/// <summary>
		/// Gets or sets the Name
		/// </summary>
		public bool HasName()
		{
			return _table.Values.ContainsKey(Property_Name);
		}

		public string Name
		{
			get
			{
				if (_table.Values.TryGetValue(Property_Name, out var nameValue))
				{
					return nameValue.Value.AsString();
				}
				else
				{
					throw new InvalidOperationException("No Name.");
				}
			}
			set
			{
				_table.Values[Property_Name] = new SMLTableValue(new SMLToken(Property_Name), new SMLValue(value));
			}
		}

		/// <summary>
		/// Gets or sets the list of Source Directories
		/// </summary>
		public bool HasSourceDirectories()
		{
			return _table.Values.ContainsKey(Property_SourceDirectories);
		}

		public IList<Path> SourceDirectories
		{
			get
			{
				if (_table.Values.TryGetValue(Property_SourceDirectories, out var sourceDirectoriesValue))
				{
					var values = sourceDirectoriesValue.Value.AsArray();
					var result = new List<Path>();
					foreach (var value in values.Values)
					{
						result.Add(new Path(value.AsString()));
					}

					return result;
				}
				else
				{
					throw new InvalidOperationException("No SDKs.");
				}
			}
			set
			{
				SMLArray? values;
				if (_table.Values.TryGetValue(Property_SourceDirectories, out var sourceDirectoriesValue))
				{
					values = sourceDirectoriesValue.Value.AsArray();
				}
				else
				{
					values = new SMLArray();
					_table.Values[Property_SourceDirectories] = new SMLTableValue(new SMLToken(Property_SourceDirectories), new SMLValue(values));
				}

				// Add the new syntax to the parent table syntax
				values.Values.Clear();

				foreach (var item in value)
				{
					values.Values.Add(new SMLValue(item.ToString()));
				}
			}
		}

		/// <summary>
		/// Gets or sets the properties table
		/// </summary>
		public bool HasProperties()
		{
			return _table.Values.ContainsKey(Property_Properties);
		}

		public SMLTable Properties
		{
			get
			{
				if (_table.Values.TryGetValue(Property_Properties, out var propertiesValue))
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
			if (_table.Values.TryGetValue(Property_Properties, out var propertiesValues))
			{
				values = propertiesValues.Value.AsTable();
			}
			else
			{
				values = new SMLTable();
				_table.Values[Property_Properties] = new SMLTableValue(new SMLToken(Property_Properties), new SMLValue(values));
			}

			// Add the new syntax to the parent table syntax
			values.Values.Clear();
			foreach (var item in value)
			{
				values.Values.Add(item.Key, new SMLTableValue(new SMLToken(item.Key), new SMLValue(item.Value)));
			}
		}

		/// <summary>
		/// Raw access
		/// </summary>
		public SMLTable GetTable()
		{
			return _table;
		}
	}
}
