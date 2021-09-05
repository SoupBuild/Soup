// <copyright file="ValueTable.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.Collections.Generic;
using System.Text;

namespace Soup.Build.Runtime
{
    public class ValueTable : Dictionary<string, IValue>, IValueTable
    {
        public ValueTable()
        {
        }

        public override string ToString()
        {
            var builder = new StringBuilder();

            builder.Append("{ ");
            foreach (var value in this)
            {
                builder.Append("\"");
                builder.Append(value.Key);
                builder.Append("\": ");
                builder.Append(value.Value);
                builder.Append(", ");
            }

            builder.Append("}");

            return builder.ToString();
        }
    }
}
