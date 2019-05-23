
lexer grammar CppLexerFloatingPointLiterals;
import CppLexerFragments;

/****************************************/
/* Floating Point Literal
/****************************************/
fragment Point: '.';
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

fragment ExponentPart:
	('e' | 'E') Sign? DecimalDigitSequence |
	('p' | 'P') Sign? DecimalDigitSequence; // C++ 17

fragment FractionalConstant:
	DecimalDigitSequence? Point DecimalDigitSequence |
	DecimalDigitSequence Point;

FloatingPointLiteral:
	DecimalDigitSequence ExponentPart FloatSuffix? |
	FractionalConstant ExponentPart? FloatSuffix? |
	HexadecimalPrefix HexidecimalDigitSequence ExponentPart FloatSuffix? | // C++ 17
	HexadecimalPrefix HexidecimalDigitSequence Point ExponentPart FloatSuffix? | // C++ 17
	HexadecimalPrefix HexidecimalDigitSequence? Point HexidecimalDigitSequence ExponentPart FloatSuffix?; // C++ 17
