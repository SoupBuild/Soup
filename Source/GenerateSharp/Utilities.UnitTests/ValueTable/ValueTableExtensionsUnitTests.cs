// <copyright file="ValueTableExtensionsUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Runtime;
using System.IO;
using System.Threading.Tasks;
using Tomlyn.Syntax;
using Xunit;

namespace Soup.Build.Utilities.UnitTests
{
	public class ValueTableExtensionsUnitTests
	{
		[Fact]
		public void AddListWithSyntax()
		{
			var uut = new ValueTable()
			{
				MirrorSyntax = new DocumentSyntax(),
			};

			uut.AddListWithSyntax("NewList");

			var content = uut.MirrorSyntax.ToString();
			Assert.Equal("NewList = [\n]\n", content);
		}

		[Fact]
		public void AddListAndItemWithSyntax()
		{
			var uut = new ValueTable()
			{
				MirrorSyntax = new DocumentSyntax(),
			};

			var newList = uut.AddListWithSyntax("NewList");
			newList.AddItemWithSyntax("NewItem");

			var content = uut.MirrorSyntax.ToString();
			Assert.Equal("NewList = [\n\t\"NewItem\",\n]\n", content);
		}
	}
}
