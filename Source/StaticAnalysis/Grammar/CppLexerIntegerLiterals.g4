
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
DecimalLiteral: 
	NonZeroDecimalDigit DecimalDigit*;

OctalLiteral:
	Zero OctalDigit*;

HexadecimalLiteral:
	HexadecimalPrefix HexadecimalDigit+;

BinaryLiteral:
	BinaryPrefix BinaryDigit+;
*/

// Zero is a special case OctalLiteral that has special meaning
Zero: '0';

DecimalLiteral: 
	NonZeroDecimalDigit (SingleQuote? DecimalDigit)*; // C++ 14

OctalLiteral:
	Zero (SingleQuote? OctalDigit)*; // C++ 14

HexadecimalLiteral:
	HexadecimalPrefix HexadecimalDigit (SingleQuote? HexadecimalDigit)*; // C++ 14

BinaryLiteral:
	BinaryPrefix BinaryDigit (SingleQuote? BinaryDigit)*; // C++ 14

IntegerLiteral:
	DecimalLiteral IntegerSuffix? |
	OctalLiteral IntegerSuffix? |
	HexadecimalLiteral IntegerSuffix? |
	BinaryLiteral IntegerSuffix?;
