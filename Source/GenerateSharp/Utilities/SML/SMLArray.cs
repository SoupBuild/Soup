// <copyright file="SMLArray.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
	public class SMLArray : IEquatable<SMLArray>
	{
		public SMLToken OpenBracket { get; set; }
		public List<SMLToken> LeadingNewlines { get; set; }
		public List<SMLArrayValue> Values { get; set; }
		public List<SMLToken> TrailingNewlines { get; set; }
		public SMLToken CloseBracket { get; set; }

		public SMLArray()
		{
			OpenBracket = SMLToken.Empty;
			LeadingNewlines = new List<SMLToken>();
			Values = new List<SMLArrayValue>();
			TrailingNewlines = new List<SMLToken>();
			CloseBracket = SMLToken.Empty;
		}

		public SMLArray(List<SMLArrayValue> values)
		{
			OpenBracket = SMLToken.Empty;
			LeadingNewlines = new List<SMLToken>();
			Values = values;
			TrailingNewlines = new List<SMLToken>();
			CloseBracket = SMLToken.Empty;
		}

		public SMLArray(
			SMLToken openBracket,
			List<SMLArrayValue> values,
			SMLToken closeBracket)
		{
			OpenBracket = openBracket;
			LeadingNewlines = new List<SMLToken>();
			Values = values;
			TrailingNewlines = new List<SMLToken>();
			CloseBracket = closeBracket;
		}

		public SMLArray(
			SMLToken openBracket,
			List<SMLToken> leadingNewlines,
			List<SMLArrayValue> values,
			List<SMLToken> trailingNewlines,
			SMLToken closeBracket)
		{
			OpenBracket = openBracket;
			LeadingNewlines = leadingNewlines;
			Values = values;
			TrailingNewlines = trailingNewlines;
			CloseBracket = closeBracket;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLArray);

		public bool Equals(SMLArray? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return Enumerable.SequenceEqual(this.Values, rhs.Values);
		}

		public override int GetHashCode() => (Values).GetHashCode();

		public static bool operator ==(SMLArray? lhs, SMLArray? rhs)
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

		public static bool operator !=(SMLArray? lhs, SMLArray? rhs) => !(lhs == rhs);
	}
}
