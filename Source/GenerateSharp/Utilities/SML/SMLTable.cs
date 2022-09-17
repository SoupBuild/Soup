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
		public SMLToken OpenBrace { get; set; }
		public Dictionary<string, SMLTableValue> Values { get; set; }
		public SMLToken CloseBrace { get; set; }

		public SMLTable()
		{
			OpenBrace = SMLToken.Empty;
			Values = new Dictionary<string, SMLTableValue>();
			CloseBrace = SMLToken.Empty;
		}

		public SMLTable(
			Dictionary<string, SMLTableValue> values)
		{
			OpenBrace = SMLToken.Empty;
			Values = values;
			CloseBrace = SMLToken.Empty;
		}

		public SMLTable(
			SMLToken openBrace,
			Dictionary<string, SMLTableValue> values,
			SMLToken closeBrace)
		{
			OpenBrace = openBrace;
			Values = values;
			CloseBrace = closeBrace;
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
			return OpenBrace == rhs.OpenBrace &&
				Enumerable.SequenceEqual(this.Values, rhs.Values) &&
				CloseBrace == rhs.OpenBrace;
		}

		public override int GetHashCode() => (Values).GetHashCode();

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
