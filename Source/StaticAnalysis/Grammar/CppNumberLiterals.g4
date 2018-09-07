
lexer grammar CppNumberLiterals;

import 
	CppFragments;

/****************************************/
/* Integer Literal
/****************************************/
fragment HexadecimalPrefix: '0x' | '0X';
fragment BinaryPrefix: '0b' | '0B';
fragment UnsignedSuffix: 'u' | 'U';
fragment LongSuffix: 'l' | 'L';
fragment LongLongSuffix: 'll' | 'LL';
fragment SingleQuote: '\'';

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
