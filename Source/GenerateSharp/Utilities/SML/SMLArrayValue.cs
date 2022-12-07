// <copyright file="SMLArrayValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;

namespace Soup.Build.Utilities
{
	public class SMLArrayValue : IEquatable<SMLArrayValue>
	{
		public SMLValue Value { get; set; }
		public List<SMLToken> Delimiter { get; set; }

		public SMLArrayValue(SMLValue value)
		{
			Value = value;
			Delimiter = new List<SMLToken>();
		}

		public SMLArrayValue(
			SMLValue value,
			List<SMLToken> delimiter)
		{
			Value = value;
			Delimiter = delimiter;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLArrayValue);

		public bool Equals(SMLArrayValue? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return this.Value == rhs.Value;
		}

		public override int GetHashCode() => (Value).GetHashCode();

		public static bool operator ==(SMLArrayValue? lhs, SMLArrayValue? rhs)
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

		public static bool operator !=(SMLArrayValue? lhs, SMLArrayValue? rhs) => !(lhs == rhs);
	}
}