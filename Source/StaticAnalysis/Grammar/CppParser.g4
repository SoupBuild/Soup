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

grammar CppParserStatements;
options { tokenVocab = CppLexer; }
	
/****************************************/
/* Statements
/****************************************/
statement:
	labeledStatement |
	attributeSpecifierSequence? expressionStatement |
	attributeSpecifierSequence? compoundStatement |
	attributeSpecifierSequence? selectionStatement |
	attributeSpecifierSequence? iterationStatement |
	attributeSpecifierSequence? jumpStatement |
	declarationStatement |
	attributeSpecifierSequence? tryBlock;

initializerStatement:
	expressionStatement |
	simpleDeclaration;

condition:
	expression
	attributeSpecifierSequence?
	declarationSpecifierSequence declarator braceOrEqualInitializer;

labeledStatement:
	attributeSpecifierSequence? identifier Colon statement |
	attributeSpecifierSequence? Case constantExpression Colon statement |
	attributeSpecifierSequence? Default Colon statement;

expressionStatement:
	expression? SemiColon;

compoundStatement:
	LeftBrace statementSequence? RightBrace;

statementSequence:
	statement |
	statementSequence statement;

selectionStatement:
	If ConstExpr? LeftParenthesis initializerStatement? condition RightParenthesis statement |
	If ConstExpr? LeftParenthesis initializerStatement? condition RightParenthesis statement Else statement |
	Switch LeftParenthesis initializerStatement? condition RightParenthesis statement;

condition:
	expression |
	attributeSpecifierSequence? declSpecifierSequence declarator Equal initializerClause |
	attributeSpecifierSequence? declSpecifierSequence declarator bracedInitializerList;

iterationStatement:
	While LeftParenthesis condition RightParenthesis statement |
	Do statement While LeftParenthesis expression RightParenthesis SemiColon |
	For LeftParenthesis initializerStatement condition? SemiColon expression? RightParenthesis statement |
	For LeftParenthesis forRangeDeclaration Colon forRangeInitializer RightParenthesis statement;

forInitializerStatement:
	attributeSpecifierSequence? declarationSpecifierSequence declarator |
	attributeSpecifierSequence? declarationSpecifierSequence referenceQualifier? LeftBracket identifierList RightBracket;

forRangeDeclaration:
	attributeSpecifierSequence? declSpecifierSequence declarator;

forRangeInitializer:
	expressionOrBracedInitializerList;

jumpStatement:
	Break SemiColon |
	Continue SemiColon |
	Return expressionOrBracedInitializerList? SemiColon |
	GoTo identifier SemiColon;

declarationStatement:
	blockDeclaration;

/****************************************/
/* Declarations
/****************************************/
declarationSequence:
	declaration
	declarationSequence declaration;

declaration:
	blockDeclaration |
	nodeclspecFunctionDeclaration |
	functionDefinition |
	templateDeclaration |
	deductionGuide |
	explicitInstantiation |
	explicitSpecialization |
	linkageSpecification |
	namespaceDefinition |
	emptyDeclaration |
	attributeDeclaration;

blockDeclaration:
	simpleDeclaration |
	asmDefinition |
	namespaceAliasDefinition |
	usingDeclaration |
	usingDirective |
	staticAssertDeclaration |
	aliasDeclaration |
	opaqueEnumDeclaration;

nodeclspecFunctionDeclaration:
	attributeSpecifierSequence? declarator SemiColon;

aliasDeclaration:
	Using identifier attributeSpecifierSequence? Equal definingTypeIdentifier SemiColon;

simpleDeclaration:
	declarationSpecifierSequence initializerDeclaratorList? SemiColon |
	attributeSpecifierSequence declarationSpecifierSequence initializerDeclaratorList SemiColon |
	attributeSpecifierSequence? declarationSpecifierSequence referenceQualifier? LeftBracket identifierList RightBracket initializer SemiColon;

staticAssertDeclaration:
	StaticAssert LeftParenthesis constantExpression RightParenthesis SemiColon |
	StaticAssert LeftParenthesis constantExpression Comma StringLiteral RightParenthesis SemiColon;

emptyDeclaration:
	SemiColon;

attributeDeclaration:
	attributeSpecifierSequence SemiColon;

declarationSpecifier:
	storageClassSpecifier |
	definingTypeSpecifier |
	functionSpecifier |
	Friend |
	TypeDef |
	ConstExpr |
	Inline;

declarationSpecifierSequence:
	declarationSpecifier attributeSpecifierSequence? |
	declarationSpecifier declarationSpecifierSequence;

storageClassSpecifier:
	Static |
	ThreadLocal |
	Extern |
	Mutable;

functionSpecifier:
	Virtual |
	Explicit;

typeSpecifier:
	simpleTypeSpecifier |
	elaboratedTypeSpecifier |
	typenameSpecifier |
	constVirtualQualifier;

typeSpecifierSequence:
	typeSpecifier attributeSpecifierSequence? |
	typeSpecifier typeSpecifierSequence;

definingTypeSpecifier:
	typeSpecifier
	classSpecifier
	enumSpecifier;

definingTypeSpecifierSequence:
	definingTypeSpecifier attributeSpecifierSequence? |
	definingTypeSpecifier definingTypeSpecifierSequence;

simpleTypeSpecifier:
	nestedNameSpecifier? typeName |
	nestedNameSpecifier Template simpleTemplateIdentifier |
	nestedNameSpecifier? templateName |
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
	decltypeSpecifier;

typeName:
	className |
	enumName |
	typedefName |
	simpleTemplateIdentifier;

decltypeSpecifier:
	DeclType LeftParenthesis expression RightParenthesis |
	DeclType LeftParenthesis Auto RightParenthesis;

elaboratedTypeSpecifier:
	classKey attributeSpecifierSequence? nestedNameSpecifier? identifier |
	classKey simpleTemplateIdentifier |
	classKey nestedNameSpecifier Template? simpleTemplateIdentifier |
	Enum nestedNameSpecifier? identifier;

enumSpecifier:
	enumHead LeftBrace enumeratorList? RightBrace |
	enumHead LeftBrace enumeratorList Comma RightBrace;

enumHead:
	enumKey attributeSpecifierSequence? enumHeadName? enumBase?;

enumHeadName:
	nestedNameSpecifier? identifier;

opaqueEnumDeclaration:
	enumKey attributeSpecifierSequence? nestedNameSpecifier? identifier enumBase? SemiColon;

enumKey:
	Enum |
	Enum Class |
	Enum Struct;

enumBase:
	Colon typeSpecifierSequence;

enumeratorList:
	enumeratorDefinition |
	enumeratorList Comma enumeratorDefinition;

enumeratorDefinition:
	enumerator |
	enumerator Equal constantExpression;

enumerator:
	identifier attributeSpecifierSequence?;

namespaceName:
	identifier |
	namespaceAlias;

namespaceDefinition:
	namedNamespaceDefinition |
	unnamedNamespaceDefinition |
	nestedNamespaceDefinition;

namedNamespaceDefinition:
	Inline? Namespace attributeSpecifierSequence? identifier LeftBrace namespaceBody RightBrace;

unnamedNamespaceDefinition:
	Inline? Namespace attributeSpecifierSequence? LeftBrace namespaceBody RightBrace;

nestedNamespaceDefinition:
	Namespace enclosingNamespaceSpecifier DoubleColon identifier LeftBrace namespaceBody RightBrace;

enclosingNamespaceSpecifier:
	identifier enclosingNamespaceSpecifier DoubleColon identifier;

namespaceBody:
	declarationSequence?;

namespaceAliasDefinition:
	Namespace identifier Equal qualifiedNamespaceSpecifier SemiColon;

qualifiedNamespaceSpecifier:
	nestedNameSpecifier? namespaceName;

usingDeclaration:
	Using usingDeclaratorList SemiColon;

usingDeclaratorList:
	usingDeclarator Ellipsis? |
	usingDeclaratorList Comma usingDeclarator Ellipsis?;

usingDeclarator:
	typeName? nestedNameSpecifier unqualifiedIdentifier;

usingDirective:
	attributeSpecifierSequence? Using Namespace nestedNameSpecifier? namespaceName SemiColon;

asmDefinition:
	attributeSpecifierSequence? Asm LeftParenthesis stringLiteral RightParenthesis SemiColon;

linkageSpecification:
	Extern stringLiteral LeftBrace declarationSequence? RightBrace |
	Extern stringLiteral declaration;

attributeSpecifierSequence:
	attributeSpecifierSequence? attributeSpecifier;

attributeSpecifier:
	[ [ attribute-using-prefix? attribute-list ] ]
	alignmentSpecifier;

alignmentSpecifier:
	AlignAs LeftParenthesis typeIdentifier Ellipsis? RightParenthesis |
	AlignAs RightParenthesis constantExpression Ellipsis? RightParenthesis;

attributeUsingPrefix:
	Using attributeNamespace Colon;

attributeList:
	attribute? |
	attributeList Comma attribute? |
	attribute Ellipsis |
	attributeList Comma attribute Ellipsis;

attribute:
	attributeToken attributeArgumentClause?;

attributeToken:
	identifier |
	attributeScopedToken;

attributeScopedToken:
	attributeNamespace DoubleColon identifier;

attributeNamespace:
	identifier;

attributeArgumentClause:
	LeftParenthesis balancedTokenSequence? RightParenthesis;

balancedTokenSequence:
	balancedToken |
	balancedTokenSequence balancedToken;

balancedToken:
	LeftParenthesis balancedTokenSequence? RightParenthesis |
	LeftBracket balancedTokenSequence? RightBracket |
	LeftBrace balancedTokenSequence? RightBrace;
	/*any token other than a parenthesis, a bracket, or a brace*/

/****************************************/
/* Declarators
/****************************************/
initializerDeclaratorList:
	initializerDeclarator |
	initializerDeclaratorList Comma initializerDeclarator;

initializerDeclarator:
	declarator initializer?;

declarator:
	pointerDeclarator |
	noPointerDeclarator parametersAndQualifiers trailingReturnType;

pointerDeclarator:
	noPointerDeclarator |
	pointerOperator pointerDeclarator;

noPointerdeclarator:
	declaratorIdentifier attributeSpecifierSequence? |
	noPointerDeclarator parametersAndQualifiers |
	noPointerDeclarator LeftBracket constantExpression? RightBracket attributeSpecifierSequence? |
	LeftParenthesis pointerDeclarator RightParenthesis;

parametersAndQualifiers:
	LeftParenthesis parameterDeclarationClause RightParenthesis constVolitilEqualifierSequence? referenceQualifier? noexceptSpecifier? attributeSpecifierSequence?;

trailingReturnType:
	Arrow typeIdentifier;

pointerOperator:
	Asterisk attributeSpecifierSequence? constVirtualQualifierSequence? |
	Ampersand attributeSpecifierSequence? |
	DoubleAmpersand attributeSpecifierSequence? |
	nestedNameSpecifier Asterisk attributeSpecifierSequence? constVirtualQualifierSequence?;

constVolitilQualifierSequence:
	constVolitilQualifier constVolitilQualifierSequence?;

constVolitilQualifier:
	Const |
	Volatile;

referenceQualifier:
	Ampersand
	DoubleAmpersand;

declaratorIdentifier:
	Ellipsis? identifierExpression;

typeIdentifier:
	typeSpecifierSequence abstractDeclarator?;

definingTypeIdentifier:
	definingTypeSpecifierSequence abstractDeclarator?;

abstractDeclarator:
	pointerAbstractDeclarator |
	noPointerAbstractDeclarator? parametersAndQualifiers trailingReturnType |
	abstractPackDeclarator;

pointerAbstractDeclarator:
	noPointerAbstractDeclarator |
	pointerOperator pointerAbstractDeclarator?;

noPointerAbstractDeclarator:
	noptr-abstract-declarator? parametersAndQualifiers |
	noPointerAbstractDeclarator? LeftBracket constantExpression? RightBracket attributeSpecifierSequence? |
	LeftParenthesis pointerAbstractDeclarator RightParenthesis

abstractPackDeclarator:
	noPointerAbstractPackDeclarator |
	pointerOperator abstractPackDeclarator;

noPointerAbstractPackDeclarator:
	noPointerAbstractPackDeclarator parametersAndQualifiers |
	noPointerAbstractPackDeclarator LeftBracket constantExpression? RightBracket attributeSpecifierSequence? |
	Ellipsis;

parameterDeclarationClause:
	parameterDeclarationList? Ellipsis? |
	parameterDeclarationList Comma Ellipsis;

parameterDeclarationList:
	parameterDeclaration |
	parameterDeclarationList Comma parameterDeclaration;

parameterDeclaration:
	attributeSpecifierSequence? declarationSpecifierSequence declarator |
	attributeSpecifierSequence? declarationSpecifierSequence declarator Equal initializerClause |
	attributeSpecifierSequence? declarationSpecifierSequence abstractDeclarator? |
	attributeSpecifierSequence? declarationSpecifierSequence abstractDeclarator? Equal initializerClause;

functionDefinition:
	attributeSpecifierSequence? declarationSpecifierSequence? declarator virtualSpecifierSequence functionBody;

functionBody:
	constructorInitializer? compoundStatement |
	functionTryBlock |
	Equal Default SemiColon |
	Equal Delete SemiColon;

initializer:
	braceOrEqualInitializer |
	LeftParenthesis expressionList RightParenthesis;

braceOrEqualInitializer:
	Equal initializerClause |
	bracedInitializerList;

initializerClause:
	assignmentExpression
	bracedInitializerList;

initializerList:
	initializerClause Ellipsis? |
	initializerList Comma initializerClause Ellipsis?;

bracedInitializerList:
	LeftBrace initializerList Comma? RightBrace |
	LeftBrace RightBrace;

expressionOrBracedInitializerList:
	expression |
	bracedInitializerList;

/**************************/
/* Classes
/**************************/
className:
	Identifier |
	simpleTemplateIdentifier;

classSpecifier:
	classHead LeftBrace memberSpecification? RightBrace;

classHead:
	classKey attributeSpecifierSequence? classHeadName classVirtualSpecifier? baseClause? |
	classKey attributeSpecifier? baseClause?;

classHeadName:
	nestedNameSpecifier? className

classVirtualSpecifier:
	Final;

classKey:
	Class |
	Struct |
	Union;

memberSpecification:
	memberDeclaration memberSpecification? |
	accessSpecifier Colon memberSpecification?;

memberDeclaration:
	attributeSpecifierSequence? declarationSpecifierSpecifier? memberDeclaratorList? SemiColon |
	functionDefinition |
	usingDeclaration |
	staticAssertDeclaration |
	templateDeclaration |
	deductionGuide |
	aliasDeclaration |
	emptyDeclaration;

memberDeclaratorList:
	memberDeclarator |
	memberDeclarator-list Comma memberDeclarator;

memberDeclarator:
	declarator virtualSpecifierSequence? pureSpecifier? |
	declarator braceOrEqualInitializer? |
	identifier? attributeSpecifierSequence? Colon constantExpression;

virtualSpecifierSequence:
	virtualSpecifier |
	virtualSpecifierSequence virtualSpecifier;

virtualSpecifier:
	Override |
	Final;

pureSpecifier:
	Equal Zero;

/**************************/
/* Derived Classes
/**************************/
baseClause:
	Colon baseSpecifierList;

baseSpecifierList:
	baseSpecifier Ellipsis? |
	baseSpecifierList Comma baseSpecifier Ellipsis?;

baseSpecifier:
	attributeSpecifierSequence? classOrDecltype |
	attributeSpecifierSequence? Virtual accessSpecifier? classOrDecltype |
	attributeSpecifierSequence? accessSpecifier Virtual? classOrDecltype;

classOrDecltype:
	nestedNameSpecifier? className |
	nestedNameSpecifier Template simpleTemplateIdentifier |
	decltypeSpecifier;

accessSpecifier:
	Private |
	Protected |
	Public;

/**************************/
/* Special Member Functions
/**************************/
conversionFunctionIdentifier:
	Operator conversionTypeIdentifier;
	
conversionTypeIdentifier:
	typeSpecifierSequence conversionDeclarator?;

conversionDeclarator:
	pointerOperator conversionDeclarator?;

constructorInitializer:
	Colon memberInitializerList;

memberInitializerList:
	memberInitializer Ellipsis? |
	memberInitializerList Comma memberInitializer Ellipsis?;

memberInitializer:
	memberInitializerIdentifier LeftParenthesis expressionList? RightParenthesis
	memberInitializerIdentifier bracedInitializerList;

memberInitializerIdentifier:
	classOrDecltype |
	identifier;

/**************************/
/* Overloading
/**************************/
operatorFunctionIdentifier:
	Operator operator;

operator:
	New |
	Delete |
	New LeftBrace RightBracket |
	Delete LeftBrace RightBracket |
	Plus |
	Minus |
	Asterisk |
	ForwardSlash |
	Percent |
	Caret |
	Ampersand |
	VerticalBar |
	Tilde |
	ExclamationMark |
	Equal |
	LessThan |
	GreaterThan |
	PlusEqual |
	MinusEqual |
	AsteriskEqual |
	ForwardSlashEqual |
	PercentEqual |
	CaretEqual |
	AmpersandEqual |
	VerticalBarEqual |
	DoubleLessThan |
	DoubleGreaterThan |
	DoubleGreaterThanEqual |
	DoubleLessThanEqual |
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

literalOperatorIdentifier:
	operator stringLiteral identifier |
	operator userDefinedStringLiteral;

/**************************/
/* Templates
/**************************/
templateDeclaration:
	Template LessThan templateParameterList GreaterThan declaration;

templateParameterList:
	templateParameter |
	templateParameterList Comma templateParameter;

templateParameter:
	typeParameter |
	parameterDeclaration;

typeParameter:
	typeParameterKey Ellipsis? identifier? |
	typeParameterKey identifier? Equal typeIdentifier |
	Template LessThan templateParameterList GreaterThan typeParameterKey Ellipsis? identifier? |
	Template LessThan templateParameterList GreaterThan typeParameterKey identifier? Equal identifierExpression;

typeParameterKey:
	Class |
	TypeName;

simpleTemplateIdentifier:
	templateName LessThan templateArgumentList? GreaterThan;

templateIdentifier:
	simpleTemplateIdentifier |
	operatorFunctionIdentifier LessThan templateArgumentList? GreaterThan |
	literalOperatorIdentifier LessThan templateArgumentList? GreaterThan;

templateArgumentList:
	templateArgument Ellipsis? |
	templateArgumentList Comma templateArgument Ellipsis?;

templateArgument:
	constantExpression |
	typeIdentifier |
	idetifierExpression;

typenameSpecifier:
	TypeName nestedNameSpecifier identifier |
	TypeName nestedNameSpecifier Template? simpleTemplateIdentifier;

explicitInstantiation:
	Extern? Template declaration;

explicitSpecialization:
	Template LessThan GreaterThan declaration;

deductionGuide:
	Explicit? templateName LeftParenthesis parameterDeclarationClause RightParenthesis Arrow simpleTemplateIdentifier SemiColon;

/**************************/
/* Exception Handling
/**************************/
tryBlock:
	Try compoundStatement handlerSequence;

functionTryBlock:
	Try constructorInitializer? compoundStatement handlerSequence;

handlerSequence:
	handler handlerSequence?;

handler:
	Catch LeftParenthesis exceptionDeclaration RightParenthesis compoundStatement;

exceptionDeclaration:
	attributeSpecifierSequence? typeSpecifierSequence declarator |
	attributeSpecifierSequence? typeSpecifierSequence abstractDeclarator? |
	Ellipsis;

noExceptionSpecifier:
	NoExcept LeftParenthesis constantExpression RightParenthesis |
	NoExcept |
	Throw LeftParenthesis RightParenthesis;
