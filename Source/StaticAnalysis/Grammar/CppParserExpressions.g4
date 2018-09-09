grammar CppParserExpressions;
options { tokenVocab = CppLexer; }

literal:
	IntegerLiteral
	| CharacterLiteral
	| FloatingPointLiteral
	| StringLiteral
	| booleanLiteral
	| pointerliteral
	| userdefinedliteral
;


primaryExpression:
	Literal |
	This |
	LeftBrace expression RightBrace |
	idExpression |
	lambdaExpression;

idExpression:
	unqualifiedId |
	qualifiedId;

unqualifiedId:
	identifier |
	operatorFunctionId |
	conversionFunctionId |
	literalOperatorId |
	Tilde className |
	Tilde decltypeSpecifier |
	templateId;

qualifiedId:
	nestedNameSpecifier Template? unqualifiedId;

nestedNameSpecifier:
	DoubleColon |
	typeName DoubleColon |
	namespaceName DoubleColon |
	decltypeSpecifier DoubleColon |
	nestedNameSpecifier identifier DoubleColon |
	nestedNameSpecifier Template? simpleTemplateId DoubleColon;

lambdaExpression:
	lambdaIntroducer lambdaDeclarator? compoundStatement;

lambdaIntroducer:
	LeftBracket lambdaCapture? RightBracket;

lambdaCapture:
	captureDefault |
	captureList |
	captureDefault Comma captureList;

captureDefault:
	Ampersand |
	Equals;

captureList:
	capture Ellipses? |
	captureList Comma capture Ellipses?;

capture:
	simpleCapture |
	initializerCapture;

simpleCapture:
	identifier |
	Ampersand identifier |
	This;

initializerCapture:
	identifier initializer |
	Ampersand identifier initializer;

lambdaDeclarator:
	LeftParenthesis parameterDeclarationClause RightParenthesis mutable? |
	exceptionSpecification? attributeSpecifierSeq? trailingReturnType?;

postfixExpression:
	primaryExpression |
	postfixExpression LeftBracket expression RightBracket |
	postfixExpression LeftBracket bracedInitializerList RightBracket |
	postfixExpression LeftParenthesis expressionList? RightParenthesis |
	simpleTypeSpecifier LeftParenthesis expressionList? RightParenthesis |
	typenameSpecifier LeftParenthesis expressionList? RightParenthesis |
	simpleTypeSpecifier bracedInitializerList |
	typenameSpecifier bracedInitializerList |
	postfixExpression Period Template? idExpression |
	postfixExpression Arrow Template? idExpression |
	postfixExpression Period pseudoDestructorName |
	postfixExpression Arrow pseudoDestructorName |
	postfixExpression DoublePlus |
	postfixExpression DoubleMinus |
	DynamicCast LessThan typeId GreaterThan LeftParenthesis expression RightParenthesis |
	StaticCast LessThan typeId GreaterThan LeftParenthesis expression RightParenthesis |
	ReinterpretCast LessThan typeId GreaterThan LeftParenthesis expression RightParenthesis |
	ConstCast LessThan typeId GreaterThan LeftParenthesis expression RightParenthesis |
	typeid LeftParenthesis expression RightParenthesis |
	typeid LeftParenthesis typeId RightParenthesis;

expressionList:
	initializerList;

pseudoDestructorName:
	nestedNameSpecifier? TypeName DoubleColon Tilde TypeName |
	nestedNameSpecifier Template simpleTemplateId DoubleColon Tilde TypeName |
	nestedNameSpecifier? Tilde TypeName |
	Tilde decltypeSpecifier;

unaryExpression:
	postfixExpression |
	DoublePlus castExpression |
	DoubleMinus castExpression |
	unaryOperator castExpression |
	SizeOf unaryExpression |
	SizeOf LeftParenthesis typeId RightParenthesis |
	SizeOf Ellipses LeftParenthesis identifier RightParenthesis |
	AlignOf LeftParenthesis typeId RightParenthesis |
	noexceptExpression |
	newExpression |
	deleteExpression;

unaryOperator:
	Asteriks |
	Ampersand |
	Plus |
	Minus |
	ExclamationMark |
	VerticalBar |
	Tilde;

newExpression:
	DoubleColon? New newPlacement? newTypeId newInitializer? |
	DoubleColon? New newPlacement? LeftParenthesis typeId RightParenthesis newInitializer?

newPlacement:
	LeftParenthesis expressionList RightParenthesis;

newTypeId:
	typeSpecifierSequence newDeclarator?;

newDeclarator:
	ptrOperator newDeclarator? |
	noptrNewDeclarator;

noptrNewDeclarator:
	LeftBracket expression RightBracket attributeSpecifierSeq? |
	noptrNewDeclarator LeftBracket constantExpression RightBracket attributeSpecifierSeq?;

newInitializer:
	LeftParenthesis expressionList? RightParenthesis |
	bracedInitializerList;

deleteExpression:
	DoubleColon?delete castExpression |
	DoubleColon?delete LeftBracket RightBracket castExpression;

noexceptExpression:
	noexcept LeftParenthesis expression RightParenthesis;

castExpression:
	unaryExpression |
	LeftParenthesis typeId RightParenthesis castExpression;

pmExpression:
	castExpression |
	pmExpression PeriodAsteriks castExpression |
	pmExpression ArrowAsteriks castExpression;

multiplicativeExpression:
	pmExpression |
	multiplicativeExpression Asteriks pmExpression |
	multiplicativeExpression ForwardSlash pmExpression |
	multiplicativeExpression Percent pmExpression;

additiveExpression:
	multiplicativeExpression |
	additiveExpression Plus multiplicativeExpression |
	additiveExpression Minus multiplicativeExpression;

shiftExpression:
	additiveExpression |
	shiftExpression LessThanLessThan additiveExpression |
	shiftExpression GreaterThanGreaterThan additiveExpression;

relationalExpression:
	shiftExpression |
	relationalExpression LessThan shiftExpression |
	relationalExpression GreaterThan shiftExpression |
	relationalExpression LessThanEquals shiftExpression |
	relationalExpression GreaterThanEquals shiftExpression;

equalityExpression:
	relationalExpression |
	equalityExpression DoubleEquals relationalExpression |
	equalityExpression ExclamationMarkEquals relationalExpression;

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

assignmentExpression:
	conditionalExpression |
	logicalOrExpression assignmentOperator initializerClause |
	throwExpression;

assignmentOperator:
	Equals |
	AsteriksEquals |
	ForwardSlashEquals |
	PercentEquals |
	PlusEquals |
	MinusEquals |
	GreaterThanGreaterThanEquals |
	LessThanLessThanEquals |
	AmpersandEquals |
	CaretEquals |
	VerticalBarEquals;

expression:
	assignmentExpression |
	expression Comma assignmentExpression;

constantExpression:
	conditionalExpression;
