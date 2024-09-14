// <copyright file="SMLManager.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Antlr4.Runtime;
using Antlr4.Runtime.Misc;

namespace Soup.Build.Utilities;

/// <summary>
/// The recipe SML
/// </summary>
public static class SMLManager
{
	/// <summary>
	/// Attempt to load from file
	/// </summary>
	public static SMLDocument Deserialize(string content)
	{
		var inputStream = new CodePointCharStream(content);
		var lexer = new SMLLexer(inputStream);
		var commonTokenStream = new CommonTokenStream(lexer);
		var parser = new SMLParser(commonTokenStream)
		{
			ErrorHandler = new BailErrorStrategy()
		};

		try
		{
			var document = parser.document();

			var visitor = new SMLValueTableVisitor(commonTokenStream);
			var result = (SMLDocument)visitor.Visit(document);
			return result;
		}
		catch (ParseCanceledException ex)
		{
			throw new InvalidOperationException($"Failed to parser SML file: {ex.Message}");
		}
	}

	/// <summary>
	/// Save the document syntax to the root file
	/// </summary>
	public static async Task SerializeAsync(SMLDocument document, Stream stream)
	{
		// Write out the entire root table
		using var writer = new StreamWriter(stream, null, -1, true);

		await SerializeAsync(document, writer);
	}

	private static async Task SerializeAsync(SMLDocument document, StreamWriter writer)
	{
		await SerializeAsync(document.LeadingNewlines, writer);

		foreach (var value in document.Values)
		{
			await SerializeAsync(value.Value.Key, writer);
			await SerializeAsync(value.Value.Colon, writer);
			await SerializeAsync(value.Value.Value, writer);
			if (value.Value.Delimiter != null)
				await SerializeAsync(value.Value.Delimiter, writer);
		}

		await SerializeAsync(document.TrailingNewlines, writer);
	}

	private static async Task SerializeAsync(SMLTable table, StreamWriter writer)
	{
		await SerializeAsync(table.OpenBrace, writer);
		await SerializeAsync(table.LeadingNewlines, writer);

		foreach (var value in table.Values)
		{
			await SerializeAsync(value.Value.Key, writer);
			await SerializeAsync(value.Value.Colon, writer);
			await SerializeAsync(value.Value.Value, writer);
			if (value.Value.Delimiter != null)
				await SerializeAsync(value.Value.Delimiter, writer);
		}

		await SerializeAsync(table.TrailingNewlines, writer);
		await SerializeAsync(table.CloseBrace, writer);
	}

	private static async Task SerializeAsync(SMLArray array, StreamWriter writer)
	{
		await SerializeAsync(array.OpenBracket, writer);
		await SerializeAsync(array.LeadingNewlines, writer);

		foreach (var value in array.Values)
		{
			await SerializeAsync(value.Value, writer);
			if (value.Delimiter != null)
				await SerializeAsync(value.Delimiter, writer);
		}

		await SerializeAsync(array.TrailingNewlines, writer);
		await SerializeAsync(array.CloseBracket, writer);
	}

	private static async Task SerializeAsync(SMLValue value, StreamWriter writer)
	{
		switch (value.Type)
		{
			case SMLValueType.Empty:
				break;
			case SMLValueType.Boolean:
				await SerializeAsync(value.AsBoolean(), writer);
				break;
			case SMLValueType.Integer:
				await SerializeAsync(value.AsInteger(), writer);
				break;
			case SMLValueType.Float:
				await SerializeAsync(value.AsFloat(), writer);
				break;
			case SMLValueType.String:
				await SerializeAsync(value.AsString(), writer);
				break;
			case SMLValueType.Table:
				await SerializeAsync(value.AsTable(), writer);
				break;
			case SMLValueType.Array:
				await SerializeAsync(value.AsArray(), writer);
				break;
			case SMLValueType.Version:
				await SerializeAsync(value.AsVersion(), writer);
				break;
			case SMLValueType.PackageReference:
				await SerializeAsync(value.AsPackageReference(), writer);
				break;
			case SMLValueType.LanguageReference:
				await SerializeAsync(value.AsLanguageReference(), writer);
				break;
			default:
				throw new InvalidOperationException("Unknown SMLValueType");
		}
	}

	private static async Task SerializeAsync(SMLStringValue value, StreamWriter writer)
	{
		await SerializeAsync(value.OpenQuote, writer);
		await SerializeAsync(value.Content, writer);
		await SerializeAsync(value.CloseQuote, writer);
	}

	private static async Task SerializeAsync(SMLIntegerValue value, StreamWriter writer)
	{
		await SerializeAsync(value.Content, writer);
	}

	private static async Task SerializeAsync(SMLFloatValue value, StreamWriter writer)
	{
		await SerializeAsync(value.Content, writer);
	}

	private static async Task SerializeAsync(SMLBooleanValue value, StreamWriter writer)
	{
		await SerializeAsync(value.Content, writer);
	}

	private static async Task SerializeAsync(IEnumerable<SMLToken> tokens, StreamWriter writer)
	{
		foreach (var token in tokens)
			await SerializeAsync(token, writer);
	}

	private static async Task SerializeAsync(SMLVersionValue value, StreamWriter writer)
	{
		await SerializeAsync(value.Content, writer);
	}

	private static async Task SerializeAsync(SMLPackageReferenceValue value, StreamWriter writer)
	{
		await SerializeAsync(value.LessThan, writer);
		if (value.LanguageName is not null)
		{
			await SerializeAsync(value.LanguageName, writer);
		}

		await SerializeAsync(value.UserName, writer);
		await SerializeAsync(value.UserPipe, writer);
		await SerializeAsync(value.PackageName, writer);
		await SerializeAsync(value.AtSign, writer);
		await SerializeAsync(value.VersionReference, writer);
		await SerializeAsync(value.GreaterThan, writer);
	}

	private static async Task SerializeAsync(SMLLanguageName value, StreamWriter writer)
	{
		await SerializeAsync(value.OpenParenthesis, writer);
		await SerializeAsync(value.LanguageName, writer);
		await SerializeAsync(value.CloseParenthesis, writer);
	}

	private static async Task SerializeAsync(SMLLanguageReferenceValue value, StreamWriter writer)
	{
		await SerializeAsync(value.OpenParenthesis, writer);
		await SerializeAsync(value.LanguageName, writer);
		await SerializeAsync(value.AtSign, writer);
		await SerializeAsync(value.VersionReference, writer);
		await SerializeAsync(value.CloseParenthesis, writer);
	}

	private static async Task SerializeAsync(SMLToken token, StreamWriter writer)
	{
		foreach (var value in token.LeadingTrivia)
		{
			await writer.WriteAsync(value);
		}

		await writer.WriteAsync(token.Text);

		foreach (var value in token.TrailingTrivia)
		{
			await writer.WriteAsync(value);
		}
	}
}