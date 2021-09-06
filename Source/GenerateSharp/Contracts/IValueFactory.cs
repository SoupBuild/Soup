// <copyright file="IValueFactory.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;

namespace Soup.Build
{
	public interface IValueFactory
	{
		IValue Create(bool value);
		IValue Create(long value);
		IValue Create(double value);
		IValue Create(string value);
		IValue Create(IValueTable value);
		IValue Create(IValueList value);
		IValue Create(DateTime value);
	}
}
