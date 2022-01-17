// <copyright file="ValueTableExtensionsUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Soup.Build.Runtime;
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

		[Fact]
		public void AddListAndTableWithSyntax()
		{
			var uut = new ValueTable()
			{
				MirrorSyntax = new DocumentSyntax(),
			};

			var newList = uut.AddListWithSyntax("NewList");
			newList.AddTableWithSyntax();

			var content = uut.MirrorSyntax.ToString();
			Assert.Equal("NewList = [\n\t{ },\n]\n", content);
		}

		[Fact]
		public void AddListAndTableAndItemWithSyntax()
		{
			var uut = new ValueTable()
			{
				MirrorSyntax = new DocumentSyntax(),
			};

			var newList = uut.AddListWithSyntax("NewList");
			var newTable = newList.AddTableWithSyntax();
			newTable.AddItemWithSyntax("NewItem", "NewValue");

			var content = uut.MirrorSyntax.ToString();
			Assert.Equal("NewList = [\n\t{ NewItem = \"NewValue\" },\n]\n", content);
		}

		[Fact]
		public void AddListAndTableAndTwoItemsWithSyntax()
		{
			var uut = new ValueTable()
			{
				MirrorSyntax = new DocumentSyntax(),
			};

			var newList = uut.AddListWithSyntax("NewList");
			var newTable = newList.AddTableWithSyntax();
			newTable.AddItemWithSyntax("NewItem1", "NewValue1");
			newTable.AddItemWithSyntax("NewItem2", "NewValue2");

			var content = uut.MirrorSyntax.ToString();
			Assert.Equal("NewList = [\n\t{ NewItem1 = \"NewValue1\", NewItem2 = \"NewValue2\" },\n]\n", content);
		}

		[Fact]
		public void AddTableWithSyntax()
		{
			var uut = new ValueTable()
			{
				MirrorSyntax = new DocumentSyntax(),
			};

			uut.AddTableWithSyntax("NewTable");

			var content = uut.MirrorSyntax.ToString();
			Assert.Equal("[NewTable]\n", content);
		}

		[Fact]
		public void AddTableAndListWithSyntax()
		{
			var uut = new ValueTable()
			{
				MirrorSyntax = new DocumentSyntax(),
			};

			var newTable = uut.AddTableWithSyntax("NewTable");
			newTable.AddListWithSyntax("NewList");

			var content = uut.MirrorSyntax.ToString();
			Assert.Equal("[NewTable]\nNewList = [\n]\n", content);
		}

		[Fact]
		public void AddTableAndListAndTableWithSyntax()
		{
			var uut = new ValueTable()
			{
				MirrorSyntax = new DocumentSyntax(),
			};

			var newTable = uut.AddTableWithSyntax("NewTable");
			var newList = newTable.AddListWithSyntax("NewList");
			var newTable2 = newList.AddTableWithSyntax();
			newTable2.AddItemWithSyntax("NewItem", "NewValue");

			var content = uut.MirrorSyntax.ToString();
			Assert.Equal("[NewTable]\nNewList = [\n\t{ NewItem = \"NewValue\" },\n]\n", content);
		}
	}
}
