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
			return _table.GetValue().ContainsKey(Property_Name);
		}

		public string Name
		{
			get
			{
				if (_table.GetValue().TryGetValue(Property_Name, out var nameValue))
				{
					return nameValue.AsString();
				}
				else
				{
					throw new InvalidOperationException("No Name.");
				}
			}
			set
			{
				_table.GetValue()[Property_Name] = new SMLValue(value);
			}
		}

		/// <summary>
		/// Gets or sets the list of Source Directories
		/// </summary>
		public bool HasSourceDirectories()
		{
			return _table.GetValue().ContainsKey(Property_SourceDirectories);
		}

		public IList<Path> SourceDirectories
		{
			get
			{
				if (_table.GetValue().TryGetValue(Property_SourceDirectories, out var sourceDirectoriesValue))
				{
					var values = sourceDirectoriesValue.AsArray();
					var result = new List<Path>();
					foreach (var value in values.GetValue())
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
				if (_table.GetValue().TryGetValue(Property_SourceDirectories, out var sourceDirectoriesValue))
				{
					values = sourceDirectoriesValue.AsArray();
				}
				else
				{
					values = new SMLArray();
					_table.GetValue()[Property_SourceDirectories] = new SMLValue(values);
				}

				// Add the new syntax to the parent table syntax
				values.GetValue().Clear();

				foreach (var item in value)
				{
					values.GetValue().Add(new SMLValue(item.ToString()));
				}
			}
		}

		/// <summary>
		/// Gets or sets the properties table
		/// </summary>
		public bool HasProperties()
		{
			return _table.GetValue().ContainsKey(Property_Properties);
		}

		public SMLTable Properties
		{
			get
			{
				if (_table.GetValue().TryGetValue(Property_Properties, out var propertiesValue))
				{
					return propertiesValue.AsTable();
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
			if (_table.GetValue().TryGetValue(Property_Properties, out var propertiesValues))
			{
				values = propertiesValues.AsTable();
			}
			else
			{
				values = new SMLTable();
				_table.GetValue()[Property_Properties] = new SMLValue(values);
			}

			// Add the new syntax to the parent table syntax
			values.GetValue().Clear();
			foreach (var item in value)
			{
				values.GetValue().Add(item.Key, new SMLValue(item.Value));
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
