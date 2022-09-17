// <copyright file="SMLStringValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Reflection.Metadata;

namespace Soup.Build.Utilities
{
	public class SMLStringValue : IEquatable<SMLStringValue>
	{
		public string Content { get; set; }

		public SMLToken OpenQuote { get; set; }

		public SMLToken Value { get; set; }

		public SMLToken CloseQuote { get; set; }

		public SMLStringValue()
		{
			Content = string.Empty;
			OpenQuote = SMLToken.Empty;
			Value = SMLToken.Empty;
			CloseQuote = SMLToken.Empty;
		}

		public SMLStringValue(string content)
		{
			Content = content;
			OpenQuote = SMLToken.Empty;
			Value = new SMLToken(content);
			CloseQuote = SMLToken.Empty;
		}

		public SMLStringValue(
			string content,
			SMLToken openQuote,
			SMLToken value,
			SMLToken closeQuote)
		{
			Content = content;
			OpenQuote = openQuote;
			Value = value;
			CloseQuote = closeQuote;
		}

		public override bool Equals(object? obj) => this.Equals(obj as SMLStringValue);

		public bool Equals(SMLStringValue? rhs)
		{
			if (rhs is null)
				return false;

			// Optimization for a common success case.
			if (object.ReferenceEquals(this, rhs))
				return true;

			// Return true if the fields match.
			return this.Content == rhs.Content;
		}

		public override int GetHashCode() => (Content).GetHashCode();

		public static bool operator ==(SMLStringValue? lhs, SMLStringValue? rhs)
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

		public static bool operator !=(SMLStringValue? lhs, SMLStringValue? rhs) => !(lhs == rhs);
	}
}
