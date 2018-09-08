parser grammar CppParserDeclarations;
options { tokenVocab = CppLexer; }

import CppParserLiterals;

/****************************************/
/* Declarations
/****************************************/
declarationSequence:;