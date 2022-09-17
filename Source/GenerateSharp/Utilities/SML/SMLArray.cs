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
		public List<SMLValue> Values { get; set; }
		public SMLToken CloseBracket { get; set; }

		public SMLArray()
		{
			OpenBracket = SMLToken.Empty;
			Values = new List<SMLValue>();
			CloseBracket = SMLToken.Empty;
		}

		public SMLArray(List<SMLValue> values)
		{
			OpenBracket = SMLToken.Empty;
			Values = values;
			CloseBracket = SMLToken.Empty;
		}

		public SMLArray(
			SMLToken openBracket,
			List<SMLValue> values,
			SMLToken closeBracket)
		{
			OpenBracket = openBracket;
			Values = values;
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
			return OpenBracket == rhs.OpenBracket &&
				Enumerable.SequenceEqual(this.Values, rhs.Values) &&
				CloseBracket == rhs.CloseBracket;
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
