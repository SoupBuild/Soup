
lexer grammar CppLexerIdentifiers;

import 
	CppLexerFragments;

/****************************************/
/* Identifiers
/****************************************/
fragment IdentifierNonDigit:
	NonDigit | UniversalCharacterName;

Identifier:
	IdentifierNonDigit (IdentifierNonDigit | DecimalDigit)*;
