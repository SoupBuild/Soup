
lexer grammar CppLexerStringLiterals;

import 
	CppLexerFragments;

/****************************************/
/* String Literals
/****************************************/
fragment StringCharacter:
	~['\\\r\n] |
	EscapeSequence |
	UniversalCharacterName;

fragment StringPrefix: ('L' | 'u8' | 'u' | 'U');
fragment RawString: DoubleQuote .*? '(' .*? ')' .*? DoubleQuote;

StringLiteral:
	'L'? DoubleQuote StringCharacter* DoubleQuote |
	('u8' | 'u' | 'U') DoubleQuote StringCharacter* DoubleQuote | // C++ 11
	StringPrefix? RawString; // C++ 11
