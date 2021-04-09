// <copyright file="ValueTable.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.Build.Runtime
{
	public class ValueTable : Dictionary<string, Value>, IValueTable
	{
	}
}
