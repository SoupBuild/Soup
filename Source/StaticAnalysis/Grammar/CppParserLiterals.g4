grammar CppParserLiterals;
options { tokenVocab = CppLexer; }

/****************************************/
/* Literals
/****************************************/
literal:
	IntegerLiteral |
	CharacterLiteral |
	FloatingPointLiteral |
	StringLiteral |
	booleanLiteral |
	pointerLiteral | // C++ 11
	userDefinedLiteral; // C++ 11

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
