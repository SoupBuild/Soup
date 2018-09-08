
lexer grammar CppLexerFragments;

fragment Zero: '0';
fragment BinaryDigit: '0'..'1';
fragment NonZeroDecimalDigit: '1'..'9';
fragment DecimalDigit: '0'..'9';
fragment OctalDigit: '0'..'7';
fragment HexadecimalDigit: '0'..'9' | 'a'..'f' | 'A'..'F';
fragment LowerCaseLetter: 'a'..'z';
fragment UpperCaseLetter: 'A'..'Z';
fragment Letter: LowerCaseLetter | UpperCaseLetter;
fragment NonDigit: Letter | '_';

fragment Sign: '+' | '-';
fragment HexadecimalPrefix: '0x' | '0X';
fragment BinaryPrefix: '0b' | '0B';
fragment UnsignedSuffix: 'u' | 'U';
fragment LongSuffix: 'l' | 'L';
fragment LongLongSuffix: 'll' | 'LL';

fragment SingleQuote: '\'';
fragment DoubleQuote: '"';

fragment EscapeSequence:
	SimpleEscapeSequence |
	OctalEscapeSequence |
	HexadecimalEscapeSequence;

fragment SimpleEscapeSequence:
	'\\\'' |
	'\\"' |
	'\\?' |
	'\\\\' |
	'\\a' |
	'\\b' |
	'\\f' |
	'\\n' |
	'\\r' |
	'\\t' |
	'\\v';

fragment OctalEscapeSequence:
	'\\' OctalDigit |
	'\\' OctalDigit OctalDigit |
	'\\' OctalDigit OctalDigit OctalDigit;

fragment HexadecimalEscapeSequence:
	'\\x' HexadecimalDigit+;

fragment HexadecimalQuad:
	HexadecimalDigit HexadecimalDigit HexadecimalDigit HexadecimalDigit;

fragment UniversalCharacterName:
	'\\u' HexadecimalQuad |
	'\\U' HexadecimalQuad HexadecimalQuad;
