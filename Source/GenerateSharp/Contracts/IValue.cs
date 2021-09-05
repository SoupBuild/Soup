// <copyright file="IValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build
{
	public interface IValue
	{
		ValueType Type { get; }
		object RawValue { get; }
	}
}
