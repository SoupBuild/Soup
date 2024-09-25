// <copyright file="SMLBooleanValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Utilities;

public class SMLBooleanValue : IEquatable<SMLBooleanValue>
{
	public bool Value { get; set; }

	public SMLToken Content { get; set; }

	public SMLBooleanValue()
	{
		Value = false;
		Content = SMLToken.Empty;
	}

	public SMLBooleanValue(bool value)
	{
		Value = value;
		Content = new SMLToken(value.ToString());
	}

	public SMLBooleanValue(
		bool value,
		SMLToken content)
	{
		Value = value;
		Content = content;
	}

	public override bool Equals(object? obj)
	{
		return Equals(obj as SMLBooleanValue);
	}

	public bool Equals(SMLBooleanValue? other)
	{
		if (other is null)
			return false;

		// Optimization for a common success case.
		if (ReferenceEquals(this, other))
			return true;

		// Return true if the fields match.
		return Value == other.Value;
	}

	public override int GetHashCode()
	{
		return Value.GetHashCode();
	}

	public static bool operator ==(SMLBooleanValue? lhs, SMLBooleanValue? rhs)
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

	public static bool operator !=(SMLBooleanValue? lhs, SMLBooleanValue? rhs)
	{
		return !(lhs == rhs);
	}
}