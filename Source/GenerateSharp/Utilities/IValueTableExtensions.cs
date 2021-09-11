// <copyright file="IValueTableExtensions.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using System.Collections.Generic;
using System.Linq;

namespace Soup.Build.Utilities
{
    public static class IValueTableExtensions
    {
        public static void SetAll(this IValueList list, IEnumerable<Path> values)
        {
            list.Clear();
            foreach (var value in values)
                list.Add(new Value(value.ToString()));
        }

        public static void Append(this IValueList list, IEnumerable<Path> values)
        {
            foreach (var value in values)
                list.Add(new Value(value.ToString()));
        }
    }
}
