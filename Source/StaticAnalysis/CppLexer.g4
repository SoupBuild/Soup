/*******************************************************************************
 * C++ 17 Lexer Grammar for ANTLR v4
 *
 * Based on N4659 final working paper
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf
 ******************************************************************************/

lexer grammar CppLexer;

// Source Character Set
// a b c d e f g h i j k l m n o p q r s t u v w x y z
// A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
// 0 1 2 3 4 5 6 7 8 9
// _ { } [ ] # ( ) < > % : ; . ? * + - / ^ & | ~ ! = , \ " ’
// space horizontal-tab vertical-tab form-feed new-line

import 
	CppKeywords,
	CppOperatorsAndPunctuators;

fragment LowerCaseLetter: [a-z];
fragment UpperCaseLetter: [A-Z];
fragment Letter: LowerCaseLetter | UpperCaseLetter;
fragment Digit: [0-9];
fragment NonDigit: Letter | '_';

fragment IdentifierNonDigit: NonDigit;
fragment EncodingPrefix: 'u8'| 'u' | 'U' | 'L';

fragment StringCharSequence:
	StringChar |
	StringCharSequence StringChar;

fragment StringChar: UnescapedCharacter;

// Any valid character except the double-quote ", backslash \, or new-line character
fragment UnescapedCharacter:
	Letter |
	Digit |
	'_' |
	'{' |
	'}' |
	'[' |
	']' |
	'#' |
	'(' |
	')' |
	'<' |
	'>' |
	'%' |
	':' |
	';' |
	'.' |
	'?' |
	'*' |
	'+' |
	'-' |
	'/' |
	'^' |
	'&' |
	'|' |
	'~' |
	'!' |
	'=' |
	',' |
	'’' |
	' ' |
	'\t' |
	'\r' |
	'\n';

fragment RawString:
	'"' DelimiterCharSequence? LeftParenthesis RawCharSequence? RightParenthesis DelimiterCharSequence? '"';
fragment RawCharSequence:
	RawChar |
	RawCharSequence RawChar;

fragment RawChar: .*?;

// A character sequence made of any source character but parentheses,
// backslash and spaces (can be empty, and at most 16 characters long)
fragment DelimiterCharSequence:
	DelimiterChar |
	DelimiterCharSequence DelimiterChar;

fragment DelimiterChar:
	Letter |
	Digit |
	'_' |
	'{' |
	'}' |
	'[' |
	']' |
	'#' |
	'<' |
	'>' |
	'%' |
	':' |
	';' |
	'.' |
	'?' |
	'*' |
	'+' |
	'-' |
	'/' |
	'^' |
	'&' |
	'|' |
	'~' |
	'!' |
	'=' |
	'"' |
	',' |
	'’' |
	'\t' |
	'\r' |
	'\n';

Identifier:
	IdentifierNonDigit (IdentifierNonDigit | Digit)*;

StringLiteral:
	EncodingPrefix? '"' StringCharSequence? '"'
	EncodingPrefix? 'R' RawString;

Whitespace: (' ' | '\t')+;
Newline: ('\r' '\n'? | '\n');
