
lexer grammar CppLexerCharacterLiterals;

import 
	CppLexerFragments;

/****************************************/
/* Character Literals
/****************************************/
fragment CCharacter:
	~['\\\r\n] |
	EscapeSequence |
	UniversalCharacterName;

CharacterLiteral:
	SingleQuote CCharacter+ SingleQuote |
	'u8' SingleQuote CCharacter+ SingleQuote | // C++ 17
	('u' | 'U') SingleQuote CCharacter+ SingleQuote | // C++ 11
	'L' SingleQuote CCharacter+ SingleQuote;
