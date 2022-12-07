// <copyright file="IValueTable.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build
{
	using System.Collections.Generic;

	public interface IValueTable : IReadOnlyDictionary<string, IValue>
	{
		new IValue this[string key] { get; set; }

		void Add(string key, IValue value);

		bool Remove(string key);
	}
}
