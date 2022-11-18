// <copyright file="IValueList.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

namespace Soup.Build
{
	using System.Collections.Generic;

	public interface IValueList : IReadOnlyList<IValue>
	{
		void Add(IValue item);

		void Clear();
	}
}
