// <copyright file="LocalUserConfig.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The local user config container
	/// </summary>
	public class LocalUserConfig
	{
		public static string Property_SDKs => "SDKs";

		private SMLDocument _table;

		/// <summary>
		/// Initializes a new instance of the <see cref="LocalUserConfig"/> class.
		/// </summary>
		public LocalUserConfig()
		{
			_table = new SMLDocument();
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="LocalUserConfig"/> class.
		/// </summary>
		public LocalUserConfig(SMLDocument table)
		{
			_table = table;
		}

		/// <summary>
		/// Gets or sets the list of SDKs
		/// </summary>
		public bool HasSDKs()
		{
			return _table.Values.ContainsKey(Property_SDKs);
		}

		public IList<SDKConfig> GetSDKs()
		{
			if (_table.Values.TryGetValue(Property_SDKs, out var sdksValue))
			{
				var values = sdksValue.Value.AsArray();
				var result = new List<SDKConfig>();
				foreach (var value in values.Values)
				{
					result.Add(new SDKConfig(value.AsTable()));
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
			SMLArray? values;
			if (_table.Values.TryGetValue(Property_SDKs, out var sdksValue))
			{
				values = sdksValue.Value.AsArray();
				foreach (var value in values.Values)
				{
					var config = new SDKConfig(value.AsTable());
					if (config.HasName() && config.Name == name)
						return config;
				}
			}
			else
			{
				values = new SMLArray();
				_table.Values.Add(
					Property_SDKs,
					new SMLTableValue(new SMLToken(Property_SDKs), new SMLValue(values)));
			}

			// No matching SDK as a table array entry
			var sdkValueTable = new SMLTable();
			values.Values.Add(new SMLValue(sdkValueTable));

			return new SDKConfig(sdkValueTable)
			{
				Name = name,
			};
		}

		/// <summary>
		/// Raw access
		/// </summary>
		public SMLDocument GetDocument()
		{
			return _table;
		}
	}
}
