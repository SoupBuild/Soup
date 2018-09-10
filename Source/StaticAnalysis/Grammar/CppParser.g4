/*******************************************************************************
 * C++ 17 Parser Grammar for ANTLR v4
 *
 * Based on N4659 final working paper
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf
 ******************************************************************************/

parser grammar CppParser;
options { tokenVocab = CppLexer; }
import CppParserLiterals;

/****************************************/
/* Keywords
/****************************************/
typedefName:
	Identifier;

namespaceName:
	Identifier |
	namespaceAlias;

namespaceAlias:
	Identifier;

className:
	Identifier |
	simpleTemplateIdentifier;

enumName:
	Identifier;

templateName:
	identifier;
	
/****************************************/
/* Basic Concepts
/****************************************/
// A file consists of a single translation unit with a sequence of declarations
translationUnit:
	declarationSequence? EOF;

/****************************************/
/* Expressions
/* https://en.cppreference.com/w/cpp/language/expressions#Primary_expressions
/****************************************/
primaryExpression:
	literal |
	This |
	LeftParenthesis expression RightParenthesis |
	identifierExpression |
	lambdaExpression | // C++ 11
	foldExpression; // C++ 17

identifierExpression:
	unqualifiedIdentifier |
	qualifiedIdentifier;

unqualifiedIdentifier:
	Identifier |
	operatorFunctionIdentifier |
	conversionFunctionIdentifier |
	literalOperatorIdentifier |
	Tilde className |
	Tilde declarationTypeSpecifier |
	templateIdentifier;

qualifiedIdentifier:
	nestedNameSpecifier Template? unqualifiedIdentifier;

nestedNameSpecifier:
	DoubleColon |
	typeName DoubleColon |
	namespaceName DoubleColon |
	declarationTypeSpecifier DoubleColon |
	nestedNameSpecifier Identifier DoubleColon |
	nestedNameSpecifier Template? simpleTemplateIdentifier DoubleColon;

// Lambda Expression
// https://en.cppreference.com/w/cpp/language/lambda
lambdaExpression:
	lambdaIntroducer lambdaDeclarator? compoundStatement;

lambdaIntroducer:
	LeftBracket lambdaCapture? RightBracket;

lambdaDeclarator:
	LeftParenthesis parameterDeclarationClause RightParenthesis declartionSecifierSequence? noExceptionSpecifier? attributeSpecifierSequence? trailingReturnType?;

lambdaCapture:
	captureDefault |
	captureList |
	captureDefault Comma captureList;

captureDefault:
	Ampersand |
	Equal;

capturelist:
	capture Ellipsis? |
	captureList Comma capture Ellipsis?;

capture:
	Ampersand? Identifier Ellipsis? |
	Ampersand? Identifier initializer | // C++ 14
	This |
	Asterisk This; // C++ 17

capture:
	simpleCapture
	initializerCapture; // C++ 14

simpleCapture:
	Ampersand? Identifier |
	This |
	Asterisk This; // C++ 17

initializerCapture:
	Ampersand? Identifier initializer;

// Fold Expression
foldExpression:
	LeftParenthesis castExpression foldOperator Ellipsis RightParenthesis |
	LeftParenthesis Ellipsis foldOperator castExpression RightParenthesis |
	LeftParenthesis castExpression foldOperator Ellipsis foldOperator castExpression RightParenthesis;

fold-operator:
	Plus |
	Minus |
	Asterisk |
	ForwardSlash |
	Percent |
	Caret |
	Ampersand |
	VerticalBar |
	DoubleLessThan |
	DoubleGreaterThan |
	PlusEqual |
	MinusEqual |
	AsteriskEqual |
	ForwardSlashEqual |
	PercentEqual |
	CaretEqual |
	AmpersandEqual |
	VerticalBarEqual |
	DoubleLessThanEqual |
	DoubleGreaterThanEqual |
	Equal |
	DoubleEqual |
	ExclamationMarkEqual |
	LessThan |
	GreaterThan |
	LessThanEqual |
	GreaterThanEqual |
	DoubleAmpersand |
	DoubleVerticalBar |
	Comma |
	PeriodAsterisk |
	ArrowAsterisk;

postfixExpression:
	primaryExpression |
	postfixExpression LeftBracket expressionOrBracedInitializerList RightBracket |
	postfixExpression LeftParenthesis expressionList? RightParenthesis |
	simpleTypeSpecifier LeftParenthesis expressionList? RightParenthesis |
	typenameSpecifier LeftParenthesis expressionList? RightParenthesis |
	simpleTypeSpecifier bracedInitializerList |
	typenameSpecifier bracedInitializerList |
	postfixExpression Period Template? identifierExpression |
	postfixExpression Arrow Template?  identifierExpression |
	postfixExpression Period pseudoDestructorName |
	postfixExpression Arrow pseudoDestructorName |
	postfixExpression DoublePlus |
	postfixExpression DoubleMinus |
	DynamicCast LessThan typeIdentifier GreaterThan LeftParenthesis expression RightParenthesis |
	StaticCast LessThan typeIdentifier GreaterThan LeftParenthesis expression RightParenthesis |
	ReinterpretCast GreaterThan LeftParenthesis expression RightParenthesis |
	ConstCast GreaterThan LeftParenthesis expression RightParenthesis |
	typeid LeftParenthesis expression RightParenthesis |
	typeid LeftParenthesis typeIdentifier RightParenthesis;

expressionList:
	initializerList;

pseudoDestructorName:
	nestedNameSpecifier? typeName DoubleColon Tilde typeName |
	nestedNameSpecifier template simpleTemplateIdentifier DoubleColon Tilde typeName |
	Tilde typeName |
	Tilde decltypeSpecifier;

unaryExpression:
	postfixExpression |
	DoublePlus castExpression |
	DoubleMinus castExpression |
	unaryOperator castExpression |
	SizeOf unaryExpression |
	SizeOf LeftParenthesis typeIdentifier RightParenthesis |
	sizeof Ellipsis LeftParenthesis identifier RightParenthesis |
	alignof LeftParenthesis typeIdentifier RightParenthesis |
	noexceptExpression |
	newExpression |
	deleteExpression;

unaryOperator:
	Asterisk |
	Ampersand |
	Plus |
	Minus |
	ExclamationMark |
	Tilde;

newExpression:
	DoubleColon? New newPlacement? newTypeIdentifier newInitializer? |
	DoubleColon? New newPlacement? LeftParenthesis typeIdentifier RightParenthesis newInitializer?;

newPlacement:
	LeftParenthesis expressionList RightParenthesis;

newTypeIdentifier:
	typeSpecifierSequence newDeclarator?;

newDeclarator:
	pointerOperator newDeclarator?
	noPointerNewDeclarator;

noPointerNewDeclarator:
	LeftBracket expression RightBracket attributeSpecifierSequence? |
	noPointerNewDeclarator LeftBracket constantExpression RightBracket attributeSpecifierSequence?;

newInitializer:
	LeftParenthesis expressionList? RightParenthesis |
	bracedInitializerList;

deleteExpression:
	DoubleColon? Delete castExpression |
	DoubleColon? Delete LeftBracket RightBracket castExpression;

noExceptionExpression:
	NoExcept LeftParenthesis expression RightParenthesis;

castExpression:
unaryExpression |
LeftParenthesis typeIdentifier RightParenthesis castExpression;

pointerManipulationExpression:
	castExpression |
	pointerManipulationExpression PeriodAsterisk castExpression |
	pointerManipulationExpression ArrowAsterisk castExpression;

multiplicativeExpression:
	pointerManipulationExpression |
	multiplicativeExpression AsteriskpointerManipulationExpression |
	multiplicativeExpression ForwardSlash pointerManipulationExpression |
	multiplicativeExpression Percent pointerManipulationExpression;

additiveExpression:
	multiplicativeExpression |
	additiveExpression Plus multiplicativeExpression |
	additiveExpression Minus multiplicativeExpression;

shiftExpression:
	additiveExpression |
	shiftExpression DoubleLessThan additiveExpression |
	shiftExpression DoubleGreaterThan additiveExpression;

relationalExpression:
	shiftExpression |
	relationalExpression LessThan shiftExpression |
	relationalExpression GreaterThan shiftExpression |
	relationalExpression LessThanEqual shiftExpression |
	relationalExpression GreaterThanEqual shiftExpression;

equalityExpression:
	relationalExpression |
	equalityExpression DoubleEqual relationalExpression |
	equalityExpression ExclamationMarkEqual relationalExpression;

andExpression:
	equalityExpression |
	andExpression Ampersand equalityExpression;

exclusiveOrExpression:
	andExpression |
	exclusiveOrExpression Caret andExpression;

inclusiveOrExpression:
	exclusiveOrExpression |
	inclusiveOrExpression VerticalBar exclusiveOrExpression;

logicalAndExpression:
	inclusiveOrExpression |
	logicalAndExpression DoubleAmpersand inclusiveOrExpression;

logicalOrExpression:
	logicalAndExpression |
	logicalOrExpression DoubleVerticalBar logicalAndExpression;

conditionalExpression:
	logicalOrExpression |
	logicalOrExpression QuestionMark expression Colon assignmentExpression;

throwExpression:
	Throw assignmentExpression?;

assignmentExpression:
	conditionalExpression |
	logicalOrExpression assignmentOperator initializerClause |
	throwExpression;

assignmentOperator:
	Equal |
	AsteriskEqual |
	ForwardSlashEqual |
	PercentEqual |
	PlusEqual |
	MinusEqual |
	DoubleGreaterThanEqual |
	DoubleLessThanEqual |
	AmpersandEqual |
	CaretEqual |
	VerticalBarEqual;

expression:
	assignmentExpression |
	expression Comma assignmentExpression;

constantExpression:
	conditionalExpression;

/****************************************/
/* Statements
/****************************************/

/****************************************/
/* Operators
/* https://en.cppreference.com/w/cpp/language/identifiers#Unqualified_identifiers
/****************************************/
operatorFunctionIdentifier: 
	Operator anyOperator;

// Cannot be operator because it is a keyword in C#
anyOperator:
	New |
	Delete|
	New LeftBracket RightBracket |
	Delete LeftBracket RightBracket |
	Plus |
	Minus |
	Asterisk |
	ForwardSlash |
	Percent |
	Ampersand |
	Caret |
	Ampersand |
	VerticalBar |
	Tilde |
	ExclamationMark |
	LessThan |
	GreaterThan |
	Equal |
	AsteriskEqual |
	ForwardSlashEqual |
	PercentEqual |
	PlusEqual |
	MinusEqual |
	DoubleGreaterThanEqual |
	DoubleLessThanEqual |
	AmpersandEqual |
	CaretEqual |
	VerticalBarEqual |
	DoubleLessThan |
	DoubleGreaterThan|
	DoubleEqual |
	ExclamationMarkEqual |
	LessThanEqual |
	GreaterThanEqual |
	DoubleAmpersand |
	DoubleVerticalBar |
	DoublePlus |
	DoubleMinus |
	Comma |
	ArrowAsterisk |
	Arrow |
	LeftParenthesis RightParenthesis |
	LeftBracket RightBracket;

conversionFunctionIdentifier: 
	Operator conversionTypeIdentifier;

conversionTypeIdentifier:
	typeSpecifierSequence conversionDeclarator?;

conversionDeclarator:
	pointerOperator conversionDeclarator?;

pointerOperator:
	Asterisk attributeSpecifierSequence? constVirtualQualifierSequence? |
	Ampersand attributeSpecifierSequence? |
	DoubleAmpersand attributeSpecifierSequence? |
	nestedNameSpecifier Asterisk attributeSpecifierSequence? constVirtualQualifierSequence?;

constVirtualQualifierSequence:Delete;
nestedNameSpecifier:Delete;

literalOperatorIdentifier: Delete;
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

/****************************************/
/* Attributes C++ 11
/****************************************/
attributeSpecifierSequence:
	attributeSpecifier |
	attributeSpecifierSequence attributeSpecifier;

attributeSpecifier:
	alignmentSpecifier |
	LeftBracket LeftBracket attributeList RightBracket RightBracket | // C++ 11
	LeftBracket LeftBracket Using attributeNamespace Colon attributeList RightBracket RightBracket; // C++ 17

alignmentSpecifier:
	AlignAs LeftParenthesis typeId Ellipsis? RightParenthesis |
	AlignAs LeftParenthesis constantExpression Ellipsis? RightParenthesis;

typeId: Delete;
constantExpression: Delete;

// Zero or more comma seperated attributes possibly ending in an ellipsis
attributeList:
	attribute? |
	attributeList Comma attribute? |
	attribute Ellipsis |
	attributeList Comma attribute Ellipsis;

attributeNamespace:
	Identifier;

attributeArgumentClause:
	LeftParenthesis argumentList RightParenthesis;

attribute:
	Identifier attributeArgumentClause? |
	attributeNamespace DoubleColon Identifier attributeArgumentClause?;

argumentList: Delete; // TODO

trailingReturnType:
	Arrow trailingTypeSpecifierSequence abstractDeclarator?;

trailingTypeSpecifierSequence:
	trailingTypeSpecifier attributeSpecifierSequence? |
	trailingTypeSpecifier trailingTypeSpecifierSequence;

simpleTypeSpecifier:
	nestedNameSpecifier? typeName |
	nestedNameSpecifier Template simpleTemplateIdentifier |
	Char |
	Char16 |
	Char32 |
	WChar |
	Bool |
	Short |
	Int |
	Long |
	Signed |
	Unsigned |
	Float |
	Double |
	Void |
	Auto |
	declarationTypeSpecifier;

typeName:
	className
	| enumName
	| typedefName
	| simpletemplateIdentifier;
