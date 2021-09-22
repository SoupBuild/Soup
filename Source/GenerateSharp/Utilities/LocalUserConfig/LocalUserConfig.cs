// <copyright file="LocalUserConfig.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Runtime;
using System;
using System.Collections.Generic;
using Tomlyn.Syntax;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The local user config container
	/// </summary>
	public class LocalUserConfig
	{
		public static string Property_SDKs => "SDKs";

		private ValueTable _table;

		private DocumentSyntax _mirrorSyntax;

		public DocumentSyntax MirrorSyntax => _mirrorSyntax;

		/// <summary>
		/// Initializes a new instance of the <see cref="LocalUserConfig"/> class.
		/// </summary>
		public LocalUserConfig()
		{
			_table = new ValueTable();
			_mirrorSyntax = new DocumentSyntax();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="LocalUserConfig"/> class.
		/// </summary>
		public LocalUserConfig(ValueTable table, DocumentSyntax mirrorSystax)
		{
			_table = table;
			_mirrorSyntax = mirrorSystax;
		}

		/// <summary>
		/// Gets or sets the list of SDKs
		/// </summary>
		public bool HasSDKs()
		{
			return _table.ContainsKey(Property_SDKs);
		}

		public IList<SDKConfig> GetSDKs()
		{
			if (_table.TryGetValue(Property_SDKs, out var sdksValue))
			{
				var values = sdksValue.AsList();
				var result = new List<SDKConfig>();
				foreach (var value in values)
				{
					result.Add(new SDKConfig((ValueTable)value.AsTable()));
				}

				return result;
			}
			else
			{
				throw new InvalidOperationException("No SDKs.");
			}
		}

		public SDKConfig EnsureSDK(string name)
		{
			// Check the existing entries
			IValueList? values;
			if (_table.TryGetValue(Property_SDKs, out var sdksValue))
			{
				values = sdksValue.AsList();
				foreach (var value in values)
				{
					var config = new SDKConfig((ValueTable)value.AsTable());
					if (config.HasName() && config.Name == name)
						return config;
				}
			}
			else
			{
				values = new ValueList();
				_table.Add(Property_SDKs, new Value(values));
			}

			// No matching SDK as a table array entry
			var sdkSyntax = new TableArraySyntax(Property_SDKs);
			_mirrorSyntax.Tables.Add(sdkSyntax);

			var sdkValueTable = new ValueTable()
			{
				MirrorSyntax = sdkSyntax,
			};
			values.Add(new Value(sdkValueTable));

			return new SDKConfig(sdkValueTable)
			{
				Name = name,
			};
		}

		/// <summary>
		/// Raw access
		/// </summary>
		public ValueTable GetTable()
		{
			return _table;
		}
	}
}
