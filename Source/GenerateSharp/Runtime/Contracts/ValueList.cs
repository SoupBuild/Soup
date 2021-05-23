// <copyright file="ValueList.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;

namespace Soup.Build.Runtime
{
	public class ValueList : List<Value>, IValueList
	{
		public ValueList() : base()
		{
		}

		public ValueList(IEnumerable<Value> collection) : base(collection)
        {
        }
	}
}
