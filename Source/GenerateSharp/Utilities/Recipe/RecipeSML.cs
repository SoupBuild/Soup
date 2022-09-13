// <copyright file="RecipeSMLManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Antlr4.Runtime;
using Soup.Build.Runtime;

namespace Soup.Build.Utilities
{
	/// <summary>
	/// The recipe SML
	/// </summary>
	public static class RecipeSML
	{
		/// <summary>
		/// Attempt to load from file
		/// </summary>
		public static Recipe Deserialize(string content)
		{
			var inputStream = new CodePointCharStream(content);
			var lexer = new SMLLexer(inputStream);
			var commonTokenStream = new CommonTokenStream(lexer);
			var parser = new SMLParser(commonTokenStream);

			var document = parser.document();

			var visitor = new SMLValueTableVisitor();
			var valueTable = (ValueTable)visitor.Visit(document).RawValue;

			return new Recipe(valueTable);
		}
	}
}
