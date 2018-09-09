grammar CppParserExpressions;
options { tokenVocab = CppLexer; }
import CppParserLiterals;

/****************************************/
/* Primary Expressions
/* https://en.cppreference.com/w/cpp/language/expressions#Primary_expressions
/****************************************/
primaryExpression:
	literal |
	identifierExpression; // |
	//lambdaExpression | // C++ 11
	//foldExpression | // C++ 17
	//requiredsExpression; // C++ 20

identifierExpression:
	unqualifiedIdentifier;// |
	//qualifiedIdentifier;

/****************************************/
/* Unqualified Identifier
/* https://en.cppreference.com/w/cpp/language/identifiers#Unqualified_identifiers
/****************************************/
unqualifiedIdentifier:
	Identifier |
	operatorFunctionIdentifier |
	conversionFunctionIdentifier |
	literalOperatorIdentifier |
	Tilde className |
	Tilde decltypeSpecifier |
	templateIdentifier;


/****************************************/
/* Operators
/* https://en.cppreference.com/w/cpp/language/identifiers#Unqualified_identifiers
/****************************************/
operatorFunctionIdentifier: 
	Operator operator;

assignmentOperator:
	Equal |
	AsteriksEqual |
	ForwardSlashEqual |
	PercentEqual |
	PlusEqual |
	MinusEqual |
	DoubleGreaterThanEqual |
	DoubleLessThanEqual |
	AmpersandEqual |
	CaretEqual |
	VerticalBarEqual;

unaryOperator:
	VerticalBar |
	Asteriks |
	Ampersand |
	Plus |
	ExclamationMark |
	Tilde |
	Minus;

anyOperator:
	New |
	Delete|
	New '[' ']'|
	Delete '[' ']'|
	'+'|
	'-'
	|
	'*'|
	'/'|
	'%'|
	'^'|
	'&'|
	'|'|
	'~'|
	'!'|
	'<'|
	'>'
	Equal |
	AsteriksEqual |
	ForwardSlashEqual |
	PercentEqual |
	PlusEqual |
	MinusEqual |
	DoubleGreaterThanEqual |
	DoubleLessThanEqual |
	AmpersandEqual |
	CaretEqual |
	VerticalBarEqual |
	| '<<'
	| rightShift
	| '=='
	| '!='
	| '<='
	| '>='
	| '&&'
	| '||'
	| '++'
	| '--'
	| ','
	| '->*'
	| '->'
	| '(' ')'
	| '[' ']'
;
conversionFunctionIdentifier: Delete;
literalOperatorIdentifier: Delete;
className: Delete;
decltypeSpecifier: Delete;
templateIdentifier: Delete;
constantExpression: Delete;

/****************************************/
/* Qualified Identifier
/* https://en.cppreference.com/w/cpp/language/identifiers#Qualified_identifiers
/****************************************/
qualifiedIdentifier:;
//	nestedNameSpecifier Template? unqualifiedIdentifier;

//nestedNameSpecifier:
//	DoubleColon |
//	typeName DoubleColon |
//	namespaceName DoubleColon |
//	decltypeSpecifier DoubleColon |
//	nestedNameSpecifier identifier DoubleColon |
//	nestedNameSpecifier Template? simpleTemplateId DoubleColon;

/**************************/
/* Classes
/**************************/
className:
	Identifier |
	simpleTemplateIdentifier;

/**************************/
/* Templates
/**************************/
simpleTemplateIdentifier:
	templateName LessThan templateArgumentList? GreaterThan;

templateName:
	Identifier;

templateArgumentList:
	templateArgument Ellipsis? |
	templateArgumentList Comma templateArgument Ellipsis?;

templateArgument:
	typeIdentifier |
	constantExpression |
	identifierExpression;

/**************************/
/* Types
/**************************/
typeIdentifier:
	typeSpecifierSequence abstractDeclarator?;

typeSpecifierSequence:Delete;
abstractDeclarator:Delete;
