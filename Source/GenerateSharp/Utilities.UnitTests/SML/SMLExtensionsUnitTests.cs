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
		public async Task AddListWithSyntax()
		{
			var uut = new SMLTable();

			uut.AddListWithSyntax("NewList");

			var content = await SerializeAsync(uut);
			Assert.Equal("{NewList = []}", content);
		}

		[Fact]
		public async Task AddListAndItemWithSyntax()
		{
			var uut = new SMLTable();

			var newList = uut.AddListWithSyntax("NewList");
			newList.AddItemWithSyntax("NewItem");

			var content = await SerializeAsync(uut);
			Assert.Equal("{NewList = [\"NewItem\", ]}", content);
		}

		[Fact]
		public async Task AddListAndTableWithSyntax()
		{
			var uut = new SMLTable();

			var newList = uut.AddListWithSyntax("NewList");
			newList.AddTableWithSyntax();

			var content = await SerializeAsync(uut);
			Assert.Equal("{NewList = [{}, ]}", content);
		}

		[Fact]
		public async Task AddListAndTableAndItemWithSyntax()
		{
			var uut = new SMLTable();

			var newList = uut.AddListWithSyntax("NewList");
			var newTable = newList.AddTableWithSyntax();
			newTable.AddItemWithSyntax("NewItem", "NewValue");

			var content = await SerializeAsync(uut);
			Assert.Equal("{NewList = [{NewItem = \"NewValue\"}, ]}", content);
		}

		[Fact]
		public async Task AddListAndTableAndTwoItemsWithSyntax()
		{
			var uut = new SMLTable();

			var newList = uut.AddListWithSyntax("NewList");
			var newTable = newList.AddTableWithSyntax();
			newTable.AddItemWithSyntax("NewItem1", "NewValue1");
			newTable.AddItemWithSyntax("NewItem2", "NewValue2");

			var content = await SerializeAsync(uut);
			Assert.Equal("{NewList = [{NewItem1 = \"NewValue1\"NewItem2 = \"NewValue2\"}, ]}", content);
		}

		[Fact]
		public async Task AddTableWithSyntax()
		{
			var uut = new SMLTable();

			uut.AddTableWithSyntax("NewTable");

			var content = await SerializeAsync(uut);
			Assert.Equal("{NewTable = {}}", content);
		}

		[Fact]
		public async Task AddTableAndListWithSyntax()
		{
			var uut = new SMLTable();

			var newTable = uut.AddTableWithSyntax("NewTable");
			newTable.AddListWithSyntax("NewList");

			var content = await SerializeAsync(uut);
			Assert.Equal("{NewTable = {NewList = []}}", content);
		}

		[Fact]
		public async Task AddTableAndListAndTableWithSyntax()
		{
			var uut = new SMLTable();

			var newTable = uut.AddTableWithSyntax("NewTable");
			var newList = newTable.AddListWithSyntax("NewList");
			var newTable2 = newList.AddTableWithSyntax();
			newTable2.AddItemWithSyntax("NewItem", "NewValue");

			var content = await SerializeAsync(uut);
			Assert.Equal("{NewTable = {NewList = [{NewItem = \"NewValue\"}, ]}}", content);
		}

		private async Task<string> SerializeAsync(SMLTable table)
		{
			using var stream = new MemoryStream();
			await SMLManager.SerializeAsync(table, stream);

			stream.Seek(0, SeekOrigin.Begin);
			using var reader = new StreamReader(stream);

			return await reader.ReadToEndAsync();
		}
	}
}
