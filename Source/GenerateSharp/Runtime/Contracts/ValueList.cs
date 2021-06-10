// <copyright file="ValueList.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Text;

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

		public override string ToString()
		{
			var builder = new StringBuilder();

			builder.Append("[ ");
			foreach (var value in this)
			{
				builder.Append(value);
				builder.Append(", ");
			}

			builder.Append("]");

			return builder.ToString();
		}
	}
}
