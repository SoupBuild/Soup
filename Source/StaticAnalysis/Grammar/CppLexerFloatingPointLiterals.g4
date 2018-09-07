
lexer grammar CppLexerFloatingPointLiterals;

import 
	CppLexerFragments;

/****************************************/
/* Floating Point Literal
/****************************************/
fragment Point: '.';
fragment ExponentSign: '+' | '-';
fragment FloatSuffix: 'f' | 'F' | 'l' | 'L';

/* C++ 14 Adds optional quote between digits
fragment DecimalDigitSequence: 
	DecimalDigit+;

fragment HexidecimalDigitSequence:
	HexadecimalDigit+;
*/

fragment DecimalDigitSequence: 
	DecimalDigit (SingleQuote? DecimalDigit)*; // C++ 14

fragment HexidecimalDigitSequence:
	HexadecimalDigit (SingleQuote? HexadecimalDigit)*; // C++ 14

fragment FloatExponent:
	('e' | 'E') ExponentSign? DecimalDigitSequence |
	('p' | 'P') ExponentSign? DecimalDigitSequence; // C++ 17

FloatingPointLiteral:
	DecimalDigitSequence FloatExponent FloatSuffix? |
	DecimalDigitSequence Point FloatExponent? FloatSuffix? |
	DecimalDigitSequence? Point DecimalDigitSequence FloatExponent? FloatSuffix? |
	HexadecimalPrefix HexidecimalDigitSequence FloatExponent FloatSuffix? | // C++ 17
	HexadecimalPrefix HexidecimalDigitSequence Point FloatExponent FloatSuffix? | // C++ 17
	HexadecimalPrefix HexidecimalDigitSequence? Point HexidecimalDigitSequence FloatExponent FloatSuffix?; // C++ 17
