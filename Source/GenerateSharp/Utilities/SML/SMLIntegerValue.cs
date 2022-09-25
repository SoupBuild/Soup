// <copyright file="SMLIntegerValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities
{
	public class SMLIntegerValue : IEquatable<SMLIntegerValue>
	{
		public long Value { get; set; }

		public SMLToken Content { get; set; }

		public SMLIntegerValue()
		{
			Value = 0;
			Content = SMLToken.Empty;
		}

		public SMLIntegerValue(long value)
		{
			Value = value;
			Content = new SMLToken(value.ToString());
		}

		public SMLIntegerValue(
			long value,
			SMLToken content)
		{
			Value = value;
			Content = content;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLIntegerValue);

		public bool Equals(SMLIntegerValue? rhs)
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

		public static bool operator ==(SMLIntegerValue? lhs, SMLIntegerValue? rhs)
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

		public static bool operator !=(SMLIntegerValue? lhs, SMLIntegerValue? rhs) => !(lhs == rhs);
	}
}
