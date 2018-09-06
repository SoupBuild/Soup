
lexer grammar CppStringLiterals;

fragment LowerCaseLetter: 'a'..'z';
fragment UpperCaseLetter: 'A'..'Z';
fragment Letter: LowerCaseLetter | UpperCaseLetter;
fragment NonZeroDigit: '1'..'9';
fragment Digit: '0'..'9';
fragment OctalDigit: [0-7];
fragment NonDigit: Letter | '_';

fragment IdentifierNonDigit: NonDigit;
fragment EncodingPrefix: 'u8'| 'u' | 'U' | 'L';

/****************************************/
/* String Literals
/****************************************/
fragment StringCharSequence:
	StringChar |
	StringChar StringCharSequence;

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

Identifier:
	IdentifierNonDigit (IdentifierNonDigit | Digit)*;

StringLiteral:
	EncodingPrefix? '"' StringCharSequence? '"';
	// EncodingPrefix? 'R' RawString;
