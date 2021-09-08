// <copyright file="ValueFactory.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build.Runtime
{
    public class ValueFactory : IValueFactory
    {
        public IValue Create(bool value)
        {
            return new Value(value);
        }

        public IValue Create(long value)
        {
            return new Value(value);
        }

        public IValue Create(double value)
        {
            return new Value(value);
        }

        public IValue Create(string value)
        {
            return new Value(value);
        }

        public IValue Create(IValueTable value)
        {
            return new Value(value);
        }

        public IValue Create(IValueList value)
        {
            return new Value(value);
        }

        public IValue Create(DateTime value)
        {
            return new Value(value);
        }

        public IValueList CreateList()
        {
            return new ValueList();
        }

        public IValueTable CreateTable()
        {
            return new ValueTable();
        }
    }
}
