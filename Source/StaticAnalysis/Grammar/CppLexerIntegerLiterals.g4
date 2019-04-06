
lexer grammar CppLexerIntegerLiterals;
import CppLexerFragments;

/****************************************/
/* Integer Literal
/****************************************/
fragment IntegerSuffix:
	UnsignedSuffix LongSuffix? |
	UnsignedSuffix LongLongSuffix? | // C++ 11
	LongSuffix UnsignedSuffix? |
	LongLongSuffix UnsignedSuffix?; // C++ 11

/* C++ 14 Adds optional quote between digits
fragment DecimalLiteral: 
	NonZeroDecimalDigit DecimalDigit*;

fragment OctalLiteral:
	Zero OctalDigit*;

fragment HexadecimalLiteral:
	HexadecimalPrefix HexadecimalDigit+;

fragment BinaryLiteral:
	BinaryPrefix BinaryDigit+;
*/

// Zero is a special case OctalLiteral that has special meaning
Zero: '0';

fragment DecimalLiteral: 
	NonZeroDecimalDigit (SingleQuote? DecimalDigit)*; // C++ 14

fragment OctalLiteral:
	Zero (SingleQuote? OctalDigit)*; // C++ 14

fragment HexadecimalLiteral:
	HexadecimalPrefix HexadecimalDigit (SingleQuote? HexadecimalDigit)*; // C++ 14

fragment BinaryLiteral:
	BinaryPrefix BinaryDigit (SingleQuote? BinaryDigit)*; // C++ 14

IntegerLiteral:
	DecimalLiteral IntegerSuffix? |
	OctalLiteral IntegerSuffix? |
	HexadecimalLiteral IntegerSuffix? |
	BinaryLiteral IntegerSuffix?;
