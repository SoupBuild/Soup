
parser grammar CppParserFunctionDefinition;
options { tokenVocab = CppLexer; }
import
	CppParserAttributes,
	CppParserDeclarations,
	CppParserDeclarators;

/****************************************/
/* Function Definition
/****************************************/
functionDefinition:
	attributeSpecifierSequence?
	declarationSpecifierSequence?
	declarator
	virtualSpecifierSequence?
	functionBody;

virtualSpecifierSequence:;

functionBody:
	constructorInitializer? compoundStatement |
	functionTryBlock |
	Equal Delete | // C++ 11
	Equal Default; // C++ 11

constructorInitializer:;
compoundStatement:;
functionTryBlock:
	constructorInitializer? compoundStatement handlerSequence;

handlerSequence:
	handler+;

handler:
	Catch LeftParenthesis attributeSpecifierSequence? typeSpecifierSequence declarator RightParenthesis compoundStatement |
	Catch LeftParenthesis attributeSpecifierSequence? typeSpecifierSequence abstractDeclarator? RightParenthesis compoundStatement |
	Catch LeftParenthesis Ellipsis RightParenthesis compoundStatement;

typeSpecifierSequence:;
abstractDeclarator:;