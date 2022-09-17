// <copyright file="SMLTableValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities
{
	public class SMLTableValue : IEquatable<SMLTableValue>
	{
		public SMLToken Key { get; set; }
		public SMLToken Colon { get; set; }
		public SMLValue Value { get; set; }

		public SMLTableValue(SMLToken key, SMLValue value)
		{
			Key = key;
			Colon = SMLToken.Empty;
			Value = value;
		}

		public SMLTableValue(
			SMLToken key,
			SMLToken colon,
			SMLValue value)
		{
			Key = key;
			Colon = colon;
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
			return Key == rhs.Key &&
				Colon == rhs.Colon &&
				this.Value == rhs.Value;
		}

		public override int GetHashCode() => (Key, Value).GetHashCode();

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