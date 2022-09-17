// <copyright file="SMLToken.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities
{
	public class SMLToken : IEquatable<SMLToken>
	{
		private string Text { get; set; }

		public SMLToken(string text)
		{
			Text = text;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLToken);

		public bool Equals(SMLToken? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return this.Text == rhs.Text;
		}

		public override int GetHashCode() => (Text).GetHashCode();

		public static bool operator ==(SMLToken? lhs, SMLToken? rhs)
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

		public static bool operator !=(SMLToken? lhs, SMLToken? rhs) => !(lhs == rhs);
	}
}
