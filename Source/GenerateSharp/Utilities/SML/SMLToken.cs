// <copyright file="SMLToken.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
	public class SMLToken : IEquatable<SMLToken>
	{
		public List<string> LeadingTrivia { get; set; }
		public string Text { get; set; }
		public List<string> TrailingTrivia { get; set; }

		public static SMLToken Empty => new SMLToken(string.Empty);

		public SMLToken(string text)
		{
			LeadingTrivia = new List<string>();
			Text = text;
			TrailingTrivia = new List<string>();
		}

		public SMLToken(List<string> leadingTrivia, string text, List<string> trailingTrivia)
		{
			LeadingTrivia = new List<string>();
			Text = text;
			TrailingTrivia = new List<string>();
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
			return this.Text == rhs.Text &&
				Enumerable.SequenceEqual(LeadingTrivia, rhs.LeadingTrivia) &&
				Enumerable.SequenceEqual(TrailingTrivia, rhs.TrailingTrivia);
		}

		// Only use the text to allow for mapping to same bins
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
