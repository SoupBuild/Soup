parser grammar CppParserDeclarations;
options { tokenVocab = CppLexer; }

import CppParserExpressions;

/****************************************/
/* Declarations
/****************************************/
declarationSequence:
	Delete; // TODO
