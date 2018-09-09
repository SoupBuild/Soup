
grammar CppParserDeclarators;
options { tokenVocab = CppLexer; }

/****************************************/
/* Declarators
/****************************************/
declarator:
	unqualifiedId attributeSpecifierSequence?
	pointerDeclarator
	noPointerDeclarator parametersAndQualifiers trailingReturnType;

unqualifiedId:;
