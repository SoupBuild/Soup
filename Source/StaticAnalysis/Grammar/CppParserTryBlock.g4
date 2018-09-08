parser grammar CppParserTryBlock;

options { tokenVocab = CppLexer; }
import
	CppParserStatements,
	CppParserDeclarators,
	CppParserDeclarations;

/****************************************/
/* Try Block
/****************************************/
tryBlock:
	Try compoundStatement handlerSequence;

handlerSequence:
	handler+;

handler:
	Catch LeftParenthesis attributeSpecifierSequence? typeSpecifierSequence declarator RightParenthesis compoundStatement |
	Catch LeftParenthesis attributeSpecifierSequence? typeSpecifierSequence abstractDeclarator? RightParenthesis compoundStatement |
	Catch LeftParenthesis Ellipsis RightParenthesis compoundStatement;
