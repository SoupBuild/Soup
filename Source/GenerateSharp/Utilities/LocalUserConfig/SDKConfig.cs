// <copyright file="SDKConfig.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Opal;
using System;
using System.Collections.Generic;

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

		private SMLTable _table;

		/// <summary>
		/// Initializes a new instance of the <see cref="SDKConfig"/> class.
		/// </summary>
		public SDKConfig() :
			this(new SMLTable())
		{
		}

		/// <summary>
		/// Initializes a new instance of the <see cref="SDKConfig"/> class.
		/// </summary>
		public SDKConfig(SMLTable table)
		{
			_table = table;
		}

		/// <summary>
		/// Gets or sets the Name
		/// </summary>
		public bool HasName()
		{
			return _table.GetValue().ContainsKey(Property_Name);
		}

		public string Name
		{
			get
			{
				if (_table.GetValue().TryGetValue(Property_Name, out var nameValue))
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
				////var valueSyntax = new StringValueSyntax(value);
				_table.GetValue()[Property_Name] = new SMLValue(value);

				// Add the new syntax to the parent table syntax
				////switch (_table.MirrorSyntax)
				////{
				////	case TableSyntaxBase tableSyntax:
				////		tableSyntax.Items.Add(new KeyValueSyntax(Property_Name, valueSyntax));
				////		break;
				////	default:
				////		throw new InvalidOperationException("Unknown Syntax on ValueTable");
				////}
			}
		}

		/// <summary>
		/// Gets or sets the list of Source Directories
		/// </summary>
		public bool HasSourceDirectories()
		{
			return _table.GetValue().ContainsKey(Property_SourceDirectories);
		}

		public IList<Path> SourceDirectories
		{
			get
			{
				if (_table.GetValue().TryGetValue(Property_SourceDirectories, out var sourceDirectoriesValue))
				{
					var values = sourceDirectoriesValue.AsArray();
					var result = new List<Path>();
					foreach (var value in values.GetValue())
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
				SMLArray? values;
				if (_table.GetValue().TryGetValue(Property_SourceDirectories, out var sourceDirectoriesValue))
				{
					values = sourceDirectoriesValue.AsArray();
				}
				else
				{
					////var arraySyntax = new ArraySyntax()
					////{
					////	OpenBracket = SyntaxFactory.Token(TokenKind.OpenBracket),
					////	CloseBracket = SyntaxFactory.Token(TokenKind.CloseBracket),
					////};
					////arraySyntax.OpenBracket.TrailingTrivia = new List<SyntaxTrivia>() { SyntaxFactory.NewLineTrivia() };
					values = new SMLArray()
					{
						// TODO: MirrorSyntax = arraySyntax,
					};
					_table.GetValue()[Property_SourceDirectories] = new SMLValue(values);

					// Add the new syntax to the parent table syntax
					////switch (_table.MirrorSyntax)
					////{
					////	case TableSyntaxBase tableSyntax:
					////		tableSyntax.Items.Add(new KeyValueSyntax(Property_SourceDirectories, arraySyntax));
					////		break;
					////	default:
					////		throw new InvalidOperationException("Unknown Syntax on ValueTable");
					////}
				}

				// Add the new syntax to the parent table syntax
				values.GetValue().Clear();

				foreach (var item in value)
				{
					values.GetValue().Add(new SMLValue(item.ToString()));
				}

				////switch (values.MirrorSyntax)
				////{
				////	case ArraySyntax arraySyntax:
				////		// Replace all items
				////		while (arraySyntax.Items.ChildrenCount > 0)
				////			arraySyntax.Items.RemoveChildAt(0);

				////		foreach (var item in value)
				////		{
				////			var arrayItemSyntax = new ArrayItemSyntax()
				////			{
				////				Value = new StringValueSyntax(item.ToString()),
				////				Comma = SyntaxFactory.Token(TokenKind.Comma),
				////			};
				////			// arrayItemSyntax.LeadingTrivia = new List<SyntaxTrivia>() { SyntaxFactory.Whitespace() };
				////			arrayItemSyntax.Comma.TrailingTrivia = new List<SyntaxTrivia>() { SyntaxFactory.NewLineTrivia() };
				////			arraySyntax.Items.Add(arrayItemSyntax);
				////		}
				////		break;
				////	default:
				////		throw new InvalidOperationException("Unknown Syntax on ValueList");
				////}
			}
		}

		/// <summary>
		/// Gets or sets the properties table
		/// </summary>
		public bool HasProperties()
		{
			return _table.GetValue().ContainsKey(Property_Properties);
		}

		public SMLTable Properties
		{
			get
			{
				if (_table.GetValue().TryGetValue(Property_Properties, out var propertiesValue))
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
			SMLTable? values;
			if (_table.GetValue().TryGetValue(Property_Properties, out var propertiesValues))
			{
				values = propertiesValues.AsTable();
			}
			else
			{
				////var newTableSyntax = new InlineTableSyntax()
				////{
				////	OpenBrace = SyntaxFactory.Token(TokenKind.OpenBrace),
				////	CloseBrace = SyntaxFactory.Token(TokenKind.CloseBrace),
				////};
				values = new SMLTable()
				{
					// TODO: MirrorSyntax = newTableSyntax,
				};
				_table.GetValue()[Property_Properties] = new SMLValue(values);

				// Add the new syntax to the parent table syntax
				////switch (_table.MirrorSyntax)
				////{
				////	case TableSyntaxBase tableSyntax:
				////		tableSyntax.Items.Add(new KeyValueSyntax(Property_Properties, newTableSyntax));
				////		break;
				////	default:
				////		throw new InvalidOperationException("Unknown Syntax on ValueTable");
				////}
			}

			// Add the new syntax to the parent table syntax
			// values.();
			////switch (values.MirrorSyntax)
			////{
			////	case InlineTableSyntax tableSyntax:
			////		// Replace all items
			////		while (tableSyntax.Items.ChildrenCount > 0)
			////			tableSyntax.Items.RemoveChildAt(0);

			////		var index = 0;
			////		foreach (var item in value)
			////		{
			////			bool isLastItem = index == value.Keys.Count - 1;
			////			var equalToken = SyntaxFactory.Token(TokenKind.Equal);
			////			equalToken.AddLeadingWhitespace();
			////			equalToken.AddTrailingWhitespace();
			////			var newKeyValue = new KeyValueSyntax()
			////			{
			////				Key = new KeySyntax(item.Key),
			////				EqualToken = equalToken,
			////				Value = new StringValueSyntax(item.Value),
			////			};
			////			var newInlineTableItem = new InlineTableItemSyntax(newKeyValue)
			////			{
			////			};
			////			if (!isLastItem)
			////			{
			////				newInlineTableItem.Comma = SyntaxFactory.Token(TokenKind.Comma);
			////			}

			////			// newInlineTableItem.LeadingTrivia = new List<SyntaxTrivia>() { SyntaxFactory.Whitespace() };
			////			tableSyntax.Items.Add(newInlineTableItem);
			////			index++;
			////		}
			////		break;
			////	default:
			////		throw new InvalidOperationException("Unknown Syntax on ValueList");
			////}
		}

		/// <summary>
		/// Raw access
		/// </summary>
		public SMLTable GetTable()
		{
			return _table;
		}
	}
}
