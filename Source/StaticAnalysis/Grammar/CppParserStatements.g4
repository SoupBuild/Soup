
grammar CppParserStatements;

statement:
	labeledStatement |
	attributeSpecifierSequence? expressionStatement |
	attributeSpecifierSequence? compoundStatement |
	attributeSpecifierSequence? selectionStatement |
	attributeSpecifierSequence? iterationStatement |
	attributeSpecifierSequence? jumpStatement |
	declarationStatement |
	attributeSpecifierSequence? tryBlock;

labeledStatement:
	attributeSpecifierSequence? identifier Colon statement |
	attributeSpecifierSequence? Case constantExpression Colon statement |
	attributeSpecifierSequence? Default Colon statement;

expressionStatement:
	expression?SemiColon;

compoundStatement:
	LeftBrace statementSequence? RightBrace;

statementSequence:
	statement |
	statementSequence statement;

selectionStatement:
	If LeftParathensis condition RightParathensis statement |
	If LeftParathensis condition RightParathensis statement Else statement |
	Switch LeftParathensis condition RightParathensis statement;

condition:
	expression |
	attributeSpecifierSequence? declSpecifierSequence declarator Equals initializerClause |
	attributeSpecifierSequence? declSpecifierSequence declarator bracedInitializerList;

iterationStatement:
	While LeftParathensis condition RightParathensis statement |
	Do statement While LeftParathensis expression RightParathensis SemiColon |
	For LeftParathensis forInitializerStatement condition? SemiColon expression? RightParathensis statement |
	For LeftParathensis forRangeDeclaration Colon forRangeInitializer RightParathensis statement;

forInitializerStatement:
	expressionStatement |
	simpleDeclaration;

forRangeDeclaration:
	attributeSpecifierSequence? declSpecifierSequence declarator;

forRangeInitializer:
	expression |
	bracedInitializerList;

jumpStatement:
	Break SemiColon |
	Continue SemiColon |
	Return expression? SemiColon |
	Return bracedInitializerList SemiColon |
	GoTo identifier SemiColon;

declarationStatement:
	blockDeclaration;
