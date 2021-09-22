// <copyright file="SDKConfig.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using Soup.Build.Runtime;
using System;
using System.Collections.Generic;
using Tomlyn.Syntax;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The SDK config container
	/// </summary>
	public class SDKConfig
	{
		private static string Property_Name => "Name";
		private static string Property_SourceDirectories => "SourceDirectories";
		private static string Property_Properties => "Properties";

		private ValueTable _table;

		/// <summary>
		/// Initializes a new instance of the <see cref="SDKConfig"/> class.
		/// </summary>
		public SDKConfig() :
			this(new ValueTable())
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="SDKConfig"/> class.
		/// </summary>
		public SDKConfig(ValueTable table)
		{
			_table = table;
		}

		/// <summary>
		/// Gets or sets the Name
		/// </summary>
		public bool HasName()
		{
			return _table.ContainsKey(Property_Name);
		}

		public string Name
		{
			get
			{
				if (_table.TryGetValue(Property_Name, out var nameValue))
				{
					return nameValue.AsString();
				}
				else
				{
					throw new InvalidOperationException("No Name.");
				}
			}
			set
			{
				var valueSyntax = new StringValueSyntax(value);
				_table[Property_Name] = new Value(value);

				// Add the new syntax to the parent table syntax
				switch (_table.MirrorSyntax)
				{
					case TableSyntaxBase tableSyntax:
						tableSyntax.Items.Add(new KeyValueSyntax(Property_Name, valueSyntax));
						break;
					default:
						throw new InvalidOperationException("Unknown Syntax on ValueTable");
				}
			}
		}

		/// <summary>
		/// Gets or sets the list of Source Directories
		/// </summary>
		public bool HasSourceDirectories()
		{
			return _table.ContainsKey(Property_SourceDirectories);
		}

		public IList<Path> SourceDirectories
		{
			get
			{
				if (_table.TryGetValue(Property_SourceDirectories, out var sourceDirectoriesValue))
				{
					var values = sourceDirectoriesValue.AsList();
					var result = new List<Path>();
					foreach (var value in values)
					{
						result.Add(new Path(value.AsString()));
					}

					return result;
				}
				else
				{
					throw new InvalidOperationException("No SDKs.");
				}
			}
			set
			{
				ValueList? values;
				if (_table.TryGetValue(Property_SourceDirectories, out var sourceDirectoriesValue))
				{
					values = (ValueList)sourceDirectoriesValue.AsList();
				}
				else
				{
					var arraySyntax = new ArraySyntax()
					{
						OpenBracket = SyntaxFactory.Token(TokenKind.OpenBracket),
						CloseBracket = SyntaxFactory.Token(TokenKind.CloseBracket),
					};
					arraySyntax.OpenBracket.AddTrailingWhitespace();
					values = new ValueList()
					{
						MirrorSyntax = arraySyntax,
					};
					_table[Property_SourceDirectories] = new Value(values);

					// Add the new syntax to the parent table syntax
					switch (_table.MirrorSyntax)
					{
						case TableSyntaxBase tableSyntax:
							tableSyntax.Items.Add(new KeyValueSyntax(Property_SourceDirectories, arraySyntax));
							break;
						default:
							throw new InvalidOperationException("Unknown Syntax on ValueTable");
					}
				}

				// Add the new syntax to the parent table syntax
				values.Clear();
				switch (values.MirrorSyntax)
				{
					case ArraySyntax arraySyntax:
						// Replace all items
						while (arraySyntax.Items.ChildrenCount > 0)
							arraySyntax.Items.RemoveChildrenAt(0);

						foreach (var item in value)
						{
							var arrayItemSyntax = new ArrayItemSyntax()
							{
								Value = new StringValueSyntax(item.ToString()),
								Comma = SyntaxFactory.Token(TokenKind.Comma),
							};
							arrayItemSyntax.Comma.AddTrailingWhitespace();
							arraySyntax.Items.Add(arrayItemSyntax);
						}
						break;
					default:
						throw new InvalidOperationException("Unknown Syntax on ValueList");
				}
			}
		}

		/// <summary>
		/// Gets or sets the properties table
		/// </summary>
		public bool HasProperties()
		{
			return _table.ContainsKey(Property_Properties);
		}

		public IValueTable Properties
		{
			get
			{
				if (_table.TryGetValue(Property_Properties, out var propertiesValue))
				{
					return propertiesValue.AsTable();
				}
				else
				{
					throw new InvalidOperationException("No Properties.");
				}
			}
		}

		public void SetProperties(IDictionary<string, string> value)
		{
			ValueTable? values;
			if (_table.TryGetValue(Property_Properties, out var propertiesValues))
			{
				values = (ValueTable)propertiesValues.AsTable();
			}
			else
			{
				var newTableSyntax = new InlineTableSyntax()
				{
					OpenBrace = SyntaxFactory.Token(TokenKind.OpenBrace),
					CloseBrace = SyntaxFactory.Token(TokenKind.CloseBrace),
				};
				values = new ValueTable()
				{
					MirrorSyntax = newTableSyntax,
				};
				_table[Property_Properties] = new Value(values);

				// Add the new syntax to the parent table syntax
				switch (_table.MirrorSyntax)
				{
					case TableSyntaxBase tableSyntax:
						tableSyntax.Items.Add(new KeyValueSyntax(Property_Properties, newTableSyntax));
						break;
					default:
						throw new InvalidOperationException("Unknown Syntax on ValueTable");
				}
			}

			// Add the new syntax to the parent table syntax
			// values.();
			switch (values.MirrorSyntax)
			{
				case InlineTableSyntax tableSyntax:
					// Replace all items
					while (tableSyntax.Items.ChildrenCount > 0)
						tableSyntax.Items.RemoveChildrenAt(0);

					foreach (var item in value)
					{
						var newKeyValue = new InlineTableItemSyntax(
							new KeyValueSyntax(item.Key, new StringValueSyntax(item.Value)))
						{
						};
						tableSyntax.Items.Add(newKeyValue);
					}
					break;
				default:
					throw new InvalidOperationException("Unknown Syntax on ValueList");
			}
		}

		/// <summary>
		/// Raw access
		/// </summary>
		public IValueTable GetTable()
		{
			return _table;
		}
	}
}
