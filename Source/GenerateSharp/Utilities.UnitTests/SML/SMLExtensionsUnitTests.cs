// <copyright file="SMLExtensionsUnitTests.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System.IO;
using System.Threading.Tasks;
using Xunit;

namespace Soup.Build.Utilities.UnitTests
{
	public class SMLExtensionsUnitTests
	{
		[Fact]
		public async Task AddArrayWithSyntax()
		{
			var uut = new SMLDocument();

			uut.AddArrayWithSyntax("NewList");

			var content = await SerializeAsync(uut);
			var expected =
@"NewList: [
]
";
			Assert.Equal(expected, content);
		}

		[Fact]
		public async Task AddTwoArraysWithSyntax()
		{
			var uut = new SMLDocument();

			uut.AddArrayWithSyntax("NewList");
			uut.AddArrayWithSyntax("ANewList2");

			var content = await SerializeAsync(uut);
			var expected =
@"NewList: [
]
ANewList2: [
]
";
			Assert.Equal(expected, content);
		}

		[Fact]
		public async Task AddArrayAndItemWithSyntax()
		{
			var uut = new SMLDocument();

			var newList = uut.AddArrayWithSyntax("NewList");
			newList.AddItemWithSyntax("NewItem", 1);

			var content = await SerializeAsync(uut);
			var expected =
@"NewList: [
	""NewItem""
]
";
			Assert.Equal(expected, content);
		}

		[Fact]
		public async Task AddArrayAndTableWithSyntax()
		{
			var uut = new SMLDocument();

			var newList = uut.AddArrayWithSyntax("NewList");
			newList.AddTableWithSyntax(1);

			var content = await SerializeAsync(uut);
			var expected =
@"NewList: [
	{
	}
]
";
			Assert.Equal(expected, content);
		}

		[Fact]
		public async Task AddArrayAndTableAndItemWithSyntax()
		{
			var uut = new SMLDocument();

			var newList = uut.AddArrayWithSyntax("NewList");
			var newTable = newList.AddTableWithSyntax(1);
			newTable.AddItemWithSyntax("NewItem", "NewValue", 2);

			var content = await SerializeAsync(uut);
			var expected =
@"NewList: [
	{
		NewItem: ""NewValue""
	}
]
";
			Assert.Equal(expected, content);
		}

		[Fact]
		public async Task AddArrayAndTableAndTwoItemsWithSyntax()
		{
			var uut = new SMLDocument();

			var newList = uut.AddArrayWithSyntax("NewList");
			var newTable = newList.AddTableWithSyntax(1);
			newTable.AddItemWithSyntax("NewItem1", "NewValue1", 2);
			newTable.AddItemWithSyntax("NewItem2", "NewValue2", 2);

			var content = await SerializeAsync(uut);
			var expected =
@"NewList: [
	{
		NewItem1: ""NewValue1""
		NewItem2: ""NewValue2""
	}
]
";
			Assert.Equal(expected, content);
		}

		[Fact]
		public async Task AddTableWithSyntax()
		{
			var uut = new SMLDocument();

			uut.AddTableWithSyntax("NewTable");

			var content = await SerializeAsync(uut);
			var expected =
@"NewTable: {
}";
			Assert.Equal(expected, content);
		}

		[Fact]
		public async Task AddInlineTableWithSyntax()
		{
			var uut = new SMLDocument();

			uut.AddInlineTableWithSyntax("NewTable");

			var content = await SerializeAsync(uut);
			var expected =
@"NewTable: { }";
			Assert.Equal(expected, content);
		}
		[Fact]
		public async Task AddInlineTableWithItemsSyntax()
		{
			var uut = new SMLDocument();

			var table = uut.AddInlineTableWithSyntax("NewTable");
			table.AddInlineItemWithSyntax("Item1", "Value1");
			table.AddInlineItemWithSyntax("Item2", "Value2");

			var content = await SerializeAsync(uut);
			var expected =
@"NewTable: { Item1: ""Value1"", Item2: ""Value2"" }";
			Assert.Equal(expected, content);
		}

		[Fact]
		public async Task AddTwoTablesWithSyntax()
		{
			var uut = new SMLDocument();

			uut.AddTableWithSyntax("NewTable");
			uut.AddTableWithSyntax("ANewTable2");

			var content = await SerializeAsync(uut);
			var expected =
@"NewTable: {
}
ANewTable2: {
}";
			Assert.Equal(expected, content);
		}

		[Fact]
		public async Task AddTableAndArrayWithSyntax()
		{
			var uut = new SMLDocument();

			var newTable = uut.AddTableWithSyntax("NewTable");
			newTable.AddArrayWithSyntax("NewList", 1);

			var content = await SerializeAsync(uut);
			var expected =
@"NewTable: {
	NewList: [
	]
}";
			Assert.Equal(expected, content);
		}

		[Fact]
		public async Task AddTableAndArrayAndTableWithSyntax()
		{
			var uut = new SMLDocument();

			var newTable = uut.AddTableWithSyntax("NewTable");
			var newList = newTable.AddArrayWithSyntax("NewList", 1);
			var newTable2 = newList.AddTableWithSyntax(2);
			newTable2.AddItemWithSyntax("NewItem", "NewValue", 3);

			var content = await SerializeAsync(uut);
			var expected =
@"NewTable: {
	NewList: [
		{
			NewItem: ""NewValue""
		}
	]
}";
			Assert.Equal(expected, content);
		}

		private async Task<string> SerializeAsync(SMLDocument document)
		{
			using var stream = new MemoryStream();
			await SMLManager.SerializeAsync(
				document,
				stream);

			stream.Seek(0, SeekOrigin.Begin);
			using var reader = new StreamReader(stream);

			return await reader.ReadToEndAsync();
		}
	}
}
