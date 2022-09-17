// <copyright file="SMLTable.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
	public class SMLTable : IEquatable<SMLTable>
	{
		private Dictionary<string, SMLValue> _value;

		public SMLTable()
		{
			_value = new Dictionary<string, SMLValue>();
		}

		public SMLTable(Dictionary<string, SMLValue> value)
		{
			_value = value;
		}

		public SMLValue this[string key]
		{
			get
			{
				return _value[key];
			}
		}

		public Dictionary<string, SMLValue> GetValue()
		{
			return _value;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLTable);

		public bool Equals(SMLTable? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return Enumerable.SequenceEqual(this._value, rhs._value);
		}

		public override int GetHashCode() => (_value).GetHashCode();

		public static bool operator ==(SMLTable? lhs, SMLTable? rhs)
		{
			if (lhs is null)
			{
				if (rhs is null)
					return true;
				else
					return false;
			}

			return lhs.Equals(rhs);
		}

		public static bool operator !=(SMLTable? lhs, SMLTable? rhs) => !(lhs == rhs);
	}
}
