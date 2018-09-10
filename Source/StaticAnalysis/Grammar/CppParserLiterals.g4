parser grammar CppParserLiterals;
options { tokenVocab = CppLexer; }

/****************************************/
/* Literals
/****************************************/
literal:
	integerLiteral |
	FloatingPointLiteral |
	CharacterLiteral |
	StringLiteral |
	booleanLiteral |
	pointerLiteral | // C++ 11
	userDefinedLiteral; // C++ 11

integerLiteral:
	Zero | // Special case Octal Literal
	IntegerLiteral;

booleanLiteral:
	True |
	False;

// C++ 11
pointerLiteral:
	Nullptr;

// C++ 11
userDefinedLiteral:
	UserDefinedIntegerLiteral |
	UserDefinedFloatingPointLiteral |
	UserDefinedCharacterLiteral |
	UserDefinedStringLiteral;
