// <copyright file="SMLFloatValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Globalization;

namespace Soup.Build.Utilities;

public class SMLFloatValue : IEquatable<SMLFloatValue>
{
	public double Value { get; set; }

	public SMLToken Content { get; set; }

	public SMLFloatValue()
	{
		this.Value = 0;
		this.Content = SMLToken.Empty;
	}

	public SMLFloatValue(double value)
	{
		this.Value = value;
		this.Content = new SMLToken(value.ToString(CultureInfo.InvariantCulture));
	}

	public SMLFloatValue(
		double value,
		SMLToken content)
	{
		this.Value = value;
		this.Content = content;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLFloatValue);
	}

	public bool Equals(SMLFloatValue? other)
	{
		if (other is null)
			return false;

		// Optimization for a common success case.
		if (ReferenceEquals(this, other))
			return true;

		// Return true if the fields match.
		return this.Value == other.Value;
	}

	public override int GetHashCode()
	{
		return this.Value.GetHashCode();
	}

	public static bool operator ==(SMLFloatValue? lhs, SMLFloatValue? rhs)
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

	public static bool operator !=(SMLFloatValue? lhs, SMLFloatValue? rhs)
	{
		return !(lhs == rhs);
	}
}