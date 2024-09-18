// <copyright file="SMLValueTableVisitor.cs" company="Soup">
// Copyright (c) Soup. All rights reserved.
// </copyright>

using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using Opal;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;

namespace Soup.Build.Utilities;

/// <summary>
/// This class provides an implementation of <see cref="ISMLVisitor{Result}"/>,
/// which converts the input SML into a <see cref="SMLValue"/>
/// </summary>
public class SMLValueTableVisitor : AbstractParseTreeVisitor<object>, ISMLVisitor<object>
{
	private readonly CommonTokenStream _tokens;

	private SMLToken? _lastToken;

	public SMLValueTableVisitor(CommonTokenStream tokens)
	{
		_tokens = tokens;
		_lastToken = null;
	}

	public virtual object VisitDocument(SMLParser.DocumentContext context)
	{
		var leadingNewlines = (List<SMLToken>)context.leadingNewlines().Accept(this);
		var tableContent = (Dictionary<string, SMLTableValue>)context.tableContent().Accept(this);
		var trailingNewlines = (List<SMLToken>)context.trailingNewlines().Accept(this);

		// Attack any trailing content ot the last token
		if (_lastToken != null)
		{
			var endTrailingContent = GetLeadingTrivia(context.Eof());
			_lastToken.TrailingTrivia.AddRange(endTrailingContent);
		}

		return new SMLDocument(
			leadingNewlines,
			tableContent,
			trailingNewlines);
	}

	public virtual object VisitTable(SMLParser.TableContext context)
	{
		var openBraceToken = BuildToken(context.OPEN_BRACE());
		var leadingNewlines = (List<SMLToken>)context.leadingNewlines().Accept(this);
		var tableContent = (Dictionary<string, SMLTableValue>)context.tableContent().Accept(this);
		var trailingNewlines = (List<SMLToken>)context.trailingNewlines().Accept(this);
		var closeBraceToken = BuildToken(context.CLOSE_BRACE());

		// Cache the last seen token
		_lastToken = closeBraceToken;

		return new SMLTable(
			openBraceToken,
			leadingNewlines,
			tableContent,
			trailingNewlines,
			closeBraceToken);
	}

	public virtual object VisitTableContent(SMLParser.TableContentContext context)
	{
		var tableContent = new Dictionary<string, SMLTableValue>();
		var tableValues = context.tableValue();
		var delimiters = context.delimiter();
		for (var i = 0; i < tableValues.Length; i++)
		{
			var value = tableValues[i];
			var tableValue = (SMLTableValue)value.Accept(this);

			// Check for optional delimiter
			if (delimiters.Length > i)
			{
				var delimiter = (List<SMLToken>)delimiters[i].Accept(this);
				tableValue.Delimiter.AddRange(delimiter);
			}

			tableContent.Add(tableValue.KeyContent, tableValue);
		}

		return tableContent;
	}

	public virtual object VisitTableValue(SMLParser.TableValueContext context)
	{
		var keyContext = context.key();
		var key = (SMLToken)keyContext.Accept(this);
		var keyContent = key.Text;
		if (keyContext is SMLParser.KeyStringContext)
		{
			keyContent = keyContent[1..^1];
		}

		var colon = BuildToken(context.COLON());
		var values = (SMLValue)context.value().Accept(this);

		return new SMLTableValue(key, keyContent, colon, values, []);
	}

	public virtual object VisitKeyLiteral(SMLParser.KeyLiteralContext context)
	{
		return BuildToken(context.ALPHA_LITERAL());
	}

	public virtual object VisitKeyString(SMLParser.KeyStringContext context)
	{
		return BuildToken(context.STRING_LITERAL());
	}

	public virtual object VisitArray(SMLParser.ArrayContext context)
	{
		var openBracketToken = BuildToken(context.OPEN_BRACKET());
		var leadingNewlines = (List<SMLToken>)context.leadingNewlines().Accept(this);
		var arrayContent = (List<SMLArrayValue>)context.arrayContent().Accept(this);
		var trailingNewlines = (List<SMLToken>)context.trailingNewlines().Accept(this);
		var closeBracketToken = BuildToken(context.CLOSE_BRACKET());

		// Cache the last seen token
		_lastToken = closeBracketToken;

		return new SMLArray(
			openBracketToken,
			leadingNewlines,
			arrayContent,
			trailingNewlines,
			closeBracketToken);
	}

	public virtual object VisitArrayContent(SMLParser.ArrayContentContext context)
	{
		List<SMLArrayValue> arrayContent = [];
		var arrayValues = context.value();
		var delimiters = context.delimiter();
		for (var i = 0; i < arrayValues.Length; i++)
		{
			var value = arrayValues[i];
			var arrayValue = new SMLArrayValue((SMLValue)value.Accept(this));

			// Check for optional delimiter
			if (delimiters.Length > i)
			{
				var delimiter = (List<SMLToken>)delimiters[i].Accept(this);
				arrayValue.Delimiter.AddRange(delimiter);
			}

			arrayContent.Add(arrayValue);
		}

		return arrayContent;
	}

	public virtual object VisitValueFloat(SMLParser.ValueFloatContext context)
	{
		var floatNode = context.FLOAT();
		var value = double.Parse(floatNode.Symbol.Text, CultureInfo.InvariantCulture);
		var floatToken = BuildToken(floatNode);

		// Cache the last seen token
		_lastToken = floatToken;

		return new SMLValue(
			new SMLFloatValue(
				value,
				floatToken));
	}

	public virtual object VisitValueInteger(SMLParser.ValueIntegerContext context)
	{
		var integerNode = context.INTEGER();
		var value = long.Parse(integerNode.Symbol.Text, CultureInfo.InvariantCulture);
		var integerToken = BuildToken(integerNode);

		// Cache the last seen token
		_lastToken = integerToken;

		return new SMLValue(
			new SMLIntegerValue(
				value,
				integerToken));
	}

	public virtual object VisitValueString(SMLParser.ValueStringContext context)
	{
		var literal = context.STRING_LITERAL().Symbol.Text;
		var content = literal[1..^1];

		var openQuoteToken = new SMLToken("'")
		{
			LeadingTrivia = GetLeadingTrivia(context.STRING_LITERAL()),
		};
		var contentToken = new SMLToken(content);
		var closeQuoteToken = new SMLToken("'");

		// Cache the last seen token
		_lastToken = closeQuoteToken;

		return new SMLValue(new SMLStringValue(
			content,
			openQuoteToken,
			contentToken,
			closeQuoteToken));
	}

	public virtual object VisitValueTrue(SMLParser.ValueTrueContext context)
	{
		var booleanToken = BuildToken(context.TRUE());

		// Cache the last seen token
		_lastToken = booleanToken;

		return new SMLValue(new SMLBooleanValue(
			true,
			booleanToken));
	}

	public virtual object VisitValueFalse(SMLParser.ValueFalseContext context)
	{
		var booleanToken = BuildToken(context.FALSE());

		// Cache the last seen token
		_lastToken = booleanToken;

		return new SMLValue(
			new SMLBooleanValue(false,
			booleanToken));
	}

	public virtual object VisitValueTable(SMLParser.ValueTableContext context)
	{
		var table = (SMLTable)context.table().Accept(this);
		return new SMLValue(table);
	}

	public virtual object VisitValueArray(SMLParser.ValueArrayContext context)
	{
		var array = (SMLArray)context.array().Accept(this);
		return new SMLValue(array);
	}

	public virtual object VisitValueVersion(SMLParser.ValueVersionContext context)
	{
		var versionToken = BuildToken(context.VERSION());

		var literal = context.VERSION().Symbol.Text;
		var value = SemanticVersion.Parse(literal);

		// Cache the last seen token
		_lastToken = versionToken;

		var version = new SMLVersionValue(
			value,
			versionToken);
		return new SMLValue(version);
	}

	public object VisitPackageReference(SMLParser.PackageReferenceContext context)
	{
		var lessThanToken = BuildToken(context.LESS_THAN());

		var languageName = (SMLLanguageName?)context.language()?.Accept(this);

		var userNameToken = (SMLToken)context.userName().Accept(this); ;
		var userPipeToken = BuildToken(context.PIPE());

		var packageNameToken = (SMLToken)context.packageName().Accept(this);
		var atSignToken = BuildToken(context.AT_SIGN());
		SMLToken versionReferenceToken;
		if (context.INTEGER() is not null)
			versionReferenceToken = BuildToken(context.INTEGER());
		else
			versionReferenceToken = BuildToken(context.FLOAT());

		var greaterThanToken = BuildToken(context.GREATER_THAN());

		// Cache the last seen token
		_lastToken = greaterThanToken;

		var value = new PackageReference(
			languageName?.Value,
			userNameToken.Text,
			packageNameToken.Text,
			SemanticVersion.Parse(versionReferenceToken.Text));

		return new SMLPackageReferenceValue(
			lessThanToken,
			languageName,
			userNameToken,
			userPipeToken,
			packageNameToken,
			atSignToken,
			versionReferenceToken,
			greaterThanToken,
			value);
	}

	public object VisitValuePackageReference(SMLParser.ValuePackageReferenceContext context)
	{
		var packageReference = (SMLPackageReferenceValue)context.packageReference().Accept(this);
		return new SMLValue(packageReference);
	}

	public object VisitLanguageReference(SMLParser.LanguageReferenceContext context)
	{
		var openParenthesisToken = BuildToken(context.OPEN_PARENTHESIS());
		var languageNameToken = (SMLToken)context.languageName().Accept(this);
		var atSignToken = BuildToken(context.AT_SIGN());
		SMLToken versionReferenceToken;
		if (context.INTEGER() is not null)
			versionReferenceToken = BuildToken(context.INTEGER());
		else
			versionReferenceToken = BuildToken(context.FLOAT());

		var closeParenthesisToken = BuildToken(context.CLOSE_PARENTHESIS());

		// Cache the last seen token
		_lastToken = closeParenthesisToken;

		var value = new LanguageReference(
			languageNameToken.Text,
			SemanticVersion.Parse(versionReferenceToken.Text));

		return new SMLLanguageReferenceValue(
			openParenthesisToken,
			languageNameToken,
			atSignToken,
			versionReferenceToken,
			closeParenthesisToken,
			value);
	}

	public object VisitValueLanguageReference(SMLParser.ValueLanguageReferenceContext context)
	{
		var languageReference = (SMLLanguageReferenceValue)context.languageReference().Accept(this);
		return new SMLValue(languageReference);
	}

	public virtual object VisitNewlineDelimiter(SMLParser.NewlineDelimiterContext context)
	{
		return context.NEWLINE().Select(BuildToken).ToList();
	}

	public virtual object VisitLeadingNewlines(SMLParser.LeadingNewlinesContext context)
	{
		return context.NEWLINE().Select(BuildToken).ToList();
	}

	public virtual object VisitTrailingNewlines(SMLParser.TrailingNewlinesContext context)
	{
		return context.NEWLINE().Select(BuildToken).ToList();
	}

	public virtual object VisitCommaDelimiter(SMLParser.CommaDelimiterContext context)
	{
		return new List<SMLToken>()
		{
			BuildToken(context.COMMA()),
		};
	}

	private SMLToken BuildToken(ITerminalNode node)
	{
		var leadingTrivia = GetLeadingTrivia(node);
		List<string> trailingTrivia = [];
		return new SMLToken(
			leadingTrivia,
			node.Symbol.Text,
			trailingTrivia);
	}

	private List<string> GetLeadingTrivia(ITerminalNode node)
	{
		var left = _tokens.GetHiddenTokensToLeft(node.Symbol.TokenIndex);
		var leadingTrivia = left != null ? left.Select(value => value.Text).ToList() : [];
		return leadingTrivia;
	}

	public object VisitLanguageName(SMLParser.LanguageNameContext context)
	{
		if (context.ALPHA_LITERAL() is not null)
			return BuildToken(context.ALPHA_LITERAL());
		else if (context.ALPHA_EXT3_LITERAL() is not null)
			return BuildToken(context.ALPHA_EXT3_LITERAL());
		else
			throw new InvalidOperationException("Invalid LanguageName");
	}

	public object VisitUserName(SMLParser.UserNameContext context)
	{
		if (context.ALPHA_LITERAL() is not null)
			return BuildToken(context.ALPHA_LITERAL());
		else if (context.ALPHA_EXT1_LITERAL() is not null)
			return BuildToken(context.ALPHA_EXT1_LITERAL());
		else
			throw new InvalidOperationException("Invalid UserName");
	}

	public object VisitPackageName(SMLParser.PackageNameContext context)
	{
		if (context.ALPHA_LITERAL() is not null)
			return BuildToken(context.ALPHA_LITERAL());
		else if (context.ALPHA_EXT2_LITERAL() is not null)
			return BuildToken(context.ALPHA_EXT2_LITERAL());
		else
			throw new InvalidOperationException("Invalid PackageName");
	}

	public object VisitLanguage(SMLParser.LanguageContext context)
	{
		var openParenthesisToken = BuildToken(context.OPEN_PARENTHESIS());
		var languageNameToken = (SMLToken)context.languageName().Accept(this);
		var closeParenthesisToken = BuildToken(context.CLOSE_PARENTHESIS());

		return new SMLLanguageName(
			openParenthesisToken,
			languageNameToken,
			closeParenthesisToken,
			languageNameToken.Text);
	}
}