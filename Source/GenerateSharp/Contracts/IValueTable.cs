// <copyright file="IValue.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.Build
{
	public interface IValueTable : IReadOnlyDictionary<string, IValue>
	{
		new IValue this[string key] { get; set; }

		void Add(string key, IValue value);

		bool Remove(string key);
	}
}
