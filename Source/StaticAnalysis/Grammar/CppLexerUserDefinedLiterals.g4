
lexer grammar CppLexerIntegerLiterals;
import 
	CppLexerIntegerLiterals,
	CppLexerFloatingPointLiterals,
	CppLexerCharacterLiterals,
	CppLexerStringLiterals;

/****************************************/
/* User Defined Literal
/****************************************/
fragment UserDefinedSuffix: Identifier;

UserDefinedIntegerLiteral:
	DecimalLiteral UserDefinedSuffix |
	OctalLiteral UserDefinedSuffix |
	HexadecimalLiteral UserDefinedSuffix |
	BinaryLiteral UserDefinedSuffix;

UserDefinedFloatingPointLiteral:
	FractionalConstant ExponentPart? UserDefinedSuffix |
	DecimalDigitSequence ExponentPart UserDefinedSuffix;

UserDefinedCharacterLiteral:
	CharacterLiteral UserDefinedSuffix;

UserDefinedStringLiteral:
	StringLiteral UserDefinedSuffix;
