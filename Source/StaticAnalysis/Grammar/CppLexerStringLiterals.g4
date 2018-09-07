
lexer grammar CppLexerStringLiterals;

import 
	CppLexerFragments;

/****************************************/
/* String Literals
/****************************************/
fragment IdentifierNonDigit: NonDigit;
fragment EncodingPrefix: 'u8'| 'u' | 'U' | 'L';

fragment StringCharSequence:
	StringChar |
	StringChar StringCharSequence;

fragment StringChar: UnescapedCharacter;

// Any valid character except the double-quote ", backslash \, or new-line character
fragment UnescapedCharacter:
	Letter |
	DecimalDigit |
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

Identifier:
	IdentifierNonDigit (IdentifierNonDigit | DecimalDigit)*;

StringLiteral:
	EncodingPrefix? '"' StringCharSequence? '"';
	// EncodingPrefix? 'R' RawString;
