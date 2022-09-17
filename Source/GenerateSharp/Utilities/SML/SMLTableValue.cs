// <copyright file="SMLTableValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities
{
	public class SMLTableValue : IEquatable<SMLTableValue>
	{
		public SMLValue Value { get; set; }

		public SMLTableValue(SMLValue value)
		{
			Value = value;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLTableValue);

		public bool Equals(SMLTableValue? rhs)
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

		public static bool operator ==(SMLTableValue? lhs, SMLTableValue? rhs)
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

		public static bool operator !=(SMLTableValue? lhs, SMLTableValue? rhs) => !(lhs == rhs);
	}
}