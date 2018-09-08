
grammar CppParserDeclarators;
options { tokenVocab = CppLexer; }

/****************************************/
/* Declarators
/****************************************/
declarator:
	unqualifiedId attributeSpecifierSequence?
	ptr-declarator
	noptr-declarator parameters-and-qualifiers trailing-return-type
