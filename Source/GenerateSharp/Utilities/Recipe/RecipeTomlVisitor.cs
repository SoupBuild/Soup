// <copyright file="RecipeTomlVisitor.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using System;
using System.Collections.Generic;
using Soup.Build.Runtime;
using Tomlyn.Syntax;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// Visitor used to transform a <see cref="DocumentSyntax"/> into a <see cref="ValueTable"/>
	/// </summary>
	internal class RecipeTomlVisitor : SyntaxVisitor
	{
		private readonly ValueTable _rootTable;
		private ValueTable _currentTable;
		private Value? _currentValue;

		public RecipeTomlVisitor(ValueTable rootTable)
		{
			_rootTable = rootTable;
			_currentTable = _rootTable;
		}

		public override void Visit(KeyValueSyntax keyValue)
		{
			if (_currentValue != null)
				throw new InvalidOperationException("An KeyValue value must not have an active value.");

			// Visit the key value, which will place the converted value into the current value
			keyValue.Value.Accept(this);
			if (_currentValue == null)
				throw new InvalidOperationException("The current value must be set for KeyValue");

			// Resolve the key
			var (currentTable, valueName) = ResolveKey(keyValue.Key);

			// Move the value into the reference
			currentTable[valueName] = _currentValue;
			_currentValue = null;
		}

		public override void Visit(TableSyntax table)
		{
			if (_currentValue != null)
				throw new InvalidOperationException("An Table value must not have an active value.");

			// A direct table reference is always from the root
			_currentTable = _rootTable;

			// Resolve the key
			var (currentTable, valueName) = ResolveKey(table.Name);
			_currentTable = GetTable(currentTable, valueName, false);

			base.Visit(table);
		}

		public override void Visit(TableArraySyntax table)
		{
			if (_currentValue != null)
				throw new InvalidOperationException("An TableArray value must not have an active value.");

			// A direct table reference is always from the root
			_currentTable = _rootTable;

			// Resolve the key
			var (currentTable, valueName) = ResolveKey(table.Name);
			_currentTable = GetTable(currentTable, valueName, true);

			base.Visit(table);
		}

		public override void Visit(BooleanValueSyntax boolValue)
		{
			if (_currentValue != null)
				throw new InvalidOperationException("A Boolean value must be at a leaf.");
			_currentValue = new Value(boolValue.Value);
		}

		public override void Visit(StringValueSyntax stringValue)
		{
			if (_currentValue != null)
				throw new InvalidOperationException("A String value must be at a leaf.");
			_currentValue = new Value(stringValue.Value);
		}

		public override void Visit(DateTimeValueSyntax dateTimeValueSyntax)
		{
			throw new NotImplementedException("Recipe TOML does not allow DateTimes.");
		}

		public override void Visit(FloatValueSyntax floatValueSyntax)
		{
			if (_currentValue != null)
				throw new InvalidOperationException("A Float value must be at a leaf.");
			_currentValue = new Value(floatValueSyntax.Value);
		}

		public override void Visit(IntegerValueSyntax integerValueSyntax)
		{
			if (_currentValue != null)
				throw new InvalidOperationException("An Integer value must be at a leaf.");
			_currentValue = new Value(integerValueSyntax.Value);
		}

		public override void Visit(ArraySyntax array)
		{
			if (_currentValue != null)
				throw new InvalidOperationException("An Array value must not have an active value.");

			var tomlArray = new ValueList();
			var items = array.Items;
			for (int i = 0; i < items.ChildrenCount; i++)
			{
				var item = items.GetChildren(i);

				// Visit the child which will set the current value
				item.Accept(this);
				if (_currentValue == null)
					throw new InvalidOperationException("The current value must be set for Array item");

				// Move the value into the array
				tomlArray.Add(_currentValue);
				_currentValue = null;
			}

			// Set the array as the current value
			_currentValue = new Value(tomlArray);
		}

		public override void Visit(InlineTableSyntax inlineTable)
		{
			var parentTable = _currentTable;
			_currentTable = new ValueTable();
			base.Visit(inlineTable);
			_currentValue = new Value(_currentTable);
			_currentTable = parentTable;
		}

		private (ValueTable Table, string Name) ResolveKey(KeySyntax key)
		{
			var currentTable = _currentTable;
			var name = GetStringFromBasic(key.Key);

			// Resolve dot references from the current table
			var items = key.DotKeys;
			for (int i = 0; i < items.ChildrenCount; i++)
			{
				currentTable = GetTable(currentTable, name, false);
				name = GetStringFromBasic(items.GetChildren(i).Key);
			}

			return (currentTable, name);
		}

		private ValueTable GetTable(ValueTable table, string key, bool createTableArrayItem)
		{
			if (table.TryGetValue(key, out var subTableObject))
			{
				if (subTableObject.Type == ValueType.List)
				{
					var tomlArray = subTableObject.AsList();
					if (createTableArrayItem)
					{
						var newTableForArray = new Value(new ValueTable());
						tomlArray.Add(newTableForArray);
						subTableObject = newTableForArray;
					}
					else
					{
						subTableObject = tomlArray[tomlArray.Count - 1];
					}
				}

				if (!(subTableObject.Type == ValueType.Table))
				{
					throw new InvalidOperationException($"Cannot transform the key `{key}` to a table while the existing underlying object is a `{subTableObject.GetType()}");
				}

				return (ValueTable)subTableObject.AsTable();
			}

			var newTable = new ValueTable();
			if (createTableArrayItem)
			{
				table[key] = new Value(new ValueList(new List<Value>() { new Value(newTable), }));
			}
			else
			{
				table[key] = new Value(newTable);
			}

			return newTable;
		}

		private string GetStringFromBasic(BareKeyOrStringValueSyntax value)
		{
			switch (value)
			{
				case BareKeySyntax basicKey:
					return basicKey.Key.Text;
				case StringValueSyntax stringValue:
					return stringValue.Value;
				default:
					throw new InvalidOperationException("Unknown type of BareKeyOrStringValueSyntax");
			}
		}
	}
}
