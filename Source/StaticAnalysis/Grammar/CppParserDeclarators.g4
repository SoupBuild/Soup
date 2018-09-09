
grammar CppParserDeclarators;
options { tokenVocab = CppLexer; }

/****************************************/
/* Declarators
/****************************************/
initializerDeclaratorList:
	initializerDeclarator |
	initializerDeclaratorList Comma initializerDeclarator;

initializerDeclarator:
	declarator initializer?;

declarator
:
	ptrdeclarator
	| noptrdeclarator parametersandqualifiers trailingreturntype
;

declarator:
	unqualifiedId attributeSpecifierSequence?
	pointerDeclarator
	noPointerDeclarator parametersAndQualifiers trailingReturnType;

unqualifiedId:;

/****************************************/
/* Initialization
/****************************************/
initializer:
	LeftParenthesis expressionList RightParenthesis |
	Equal expression |
	LeftBrace initializerList RightBrace;
