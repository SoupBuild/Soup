parser grammar CppParserAttributes;
options { tokenVocab = CppLexer; }

/****************************************/
/* Attributes C++ 11
/****************************************/
attributeSpecifierSequence:
	LeftBracket LeftBracket attributeList RightBracket RightBracket | // C++ 11
	LeftBracket LeftBracket Using attributeNamespace Colon attributeList RightBracket RightBracket | // C++ 17
	LeftBracket LeftBracket contractAttributeToken contractLevel? Identifier? Colon expression RightBracket RightBracket; // C++ 20

// Zero or more comma seperated attributes
attributeList:
	attribute? |
	attribute (Comma attribute)*;

attributeNamespace:
	Identifier;

attribute:
	Identifier |
	attributeNamespace DoubleColon Identifier |
	Identifier LeftParenthesis argumentList RightParenthesis
	attributeNamespace DoubleColon Identifier LeftParenthesis argumentList RightParenthesis;

argumentList:;

contractLevel:;
contractAttributeToken:;