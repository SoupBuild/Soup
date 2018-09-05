
grammar CppDeclarations;

declarationSequence:
	declaration |
	declarationSequence declaration;

declaration:
	blockDeclaration |
	functionDefinition |
	templateDeclaration |
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
	static_assertDeclaration |
	aliasDeclaration |
	opaqueEnumDeclaration;

aliasDeclaration:
	using identifier attributeSpecifierSequence? Equals typeId SemiColon;

simpleDeclaration:
	declSpecifierSequence? initDeclaratorList? SemiColon
	attributeSpecifierSequence declSpecifierSequence? initDeclaratorList SemiColon;

static_assertDeclaration:
	StaticAssert LeftParenthasis constantExpression Comma stringLiteral RightParenthasis SemiColon;

emptyDeclaration:
	SemiColon;

attributeDeclaration:
	attributeSpecifierSequence SemiColon;

declarationSpecifier:
	storageClassSpecifier |
	typeSpecifier |
	functionSpecifier |
	Friend |
	TypeDef |
	ConstExpr;

declSpecifierSequence:
	declSpecifier attributeSpecifierSequence? |
	declSpecifier declSpecifierSequence;

storageClassSpecifier:
	Register |
	Static |
	ThreadLocal |
	Extern |
	Mutable;

functionSpecifier:
	Inline |
	Virtual |
	Explicit;

typedefName:
	identifier;

typeSpecifier:
	trailingTypeSpecifier |
	classSpecifier |
	enumSpecifier;

trailingTypeSpecifier:
	simpleTypeSpecifier |
	elaboratedTypeSpecifier |
	typenameSpecifier |
	cvQualifier;

typeSpecifierSequence:
	typeSpecifier attributeSpecifierSequence? |
	typeSpecifier typeSpecifierSequence;

trailingTypeSpecifierSequence:
	trailingTypeSpecifier attributeSpecifierSequence? |
	trailingTypeSpecifier trailingTypeSpecifierSequence;

simpleTypeSpecifier:
	nestedNameSpecifier? typeName |
	nestedNameSpecifier Template simpleTemplateId |
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
	simpleTemplateId;

decltypeSpecifier:
	decltype LeftParenthasis expression RightParenthasis |
	decltype LeftParenthasis auto RightParenthasis;

elaboratedTypeSpecifier:
	classKey attributeSpecifierSequence? nestedNameSpecifier? identifier |
	classKey simpleTemplateId |
	classKey nestedNameSpecifier Template? simpleTemplateId |
	Enum nestedNameSpecifier? identifier;

enumName:
	identifier;

enumSpecifier:
	enumHead LeftBrace enumeratorList? RightBrace |
	enumHead LeftBrace enumeratorList Comma RightBrace;

enumHead:
	enumKey attributeSpecifierSequence? identifier? enumBase? |
	enumKey attributeSpecifierSequence? nestedNameSpecifier identifier |
	enumBase?;

opaqueEnumDeclaration:
	enumKey attributeSpecifierSequence? identifier enumBase?SemiColon;

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
	enumerator Equals constantExpression;

enumerator:
	identifier;

namespaceName:
	originalNamespaceName |
	namespaceAlias;

originalNamespaceName:
	identifier;

namespaceDefinition:
	namedNamespaceDefinition |
	unnamedNamespaceDefinition;

namedNamespaceDefinition:
	originalNamespaceDefinition |
	extensionNamespaceDefinition;

originalNamespaceDefinition:
	inline? namespace identifier LeftBrace namespaceBody RightBrace;

extensionNamespaceDefinition:
	inline? namespace originalNamespaceName LeftBrace namespaceBody RightBrace;

unnamedNamespaceDefinition:
	inline? namespace LeftBrace namespaceBody RightBrace;

namespaceBody:
	declarationSequence?;

namespaceAlias:
	identifier;

namespaceAliasDefinition:
	namespace identifier Equals qualifiedNamespaceSpecifier SemiColon;

qualifiedNamespaceSpecifier:
	nestedNameSpecifier? namespaceName;

usingDeclaration:
	Using typename? nestedNameSpecifier unqualifiedId SemiColon |
	Using DoubleColon unqualifiedId SemiColon;

usingDirective:
	attributeSpecifierSequence?using namespace nestedNameSpecifier? namespaceName SemiColon;

asmDefinition:
	asm LeftParenthasis stringLiteral RightParenthasis SemiColon;

linkageSpecification:
	Extern stringLiteral LeftBrace declarationSequence? RightBrace |
	Extern stringLiteral declaration;

attributeSpecifierSequence:
	attributeSpecifierSequence? attributeSpecifier;

attributeSpecifier:
	LeftBracket LeftBracket attributeList RightBracket RightBracket |
	alignmentSpecifier;

alignmentSpecifier:
	alignas LeftParenthasis typeId Ellipses? RightParenthasis |
	alignas LeftParenthasis constantExpression Ellipses? RightParenthasis;

attributeList:
	attribute? |
	attributeList Comma attribute? |
	attribute Ellipses |
	attributeList Comma attribute Ellipses;

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
	LeftParenthasis balancedTokenSequence RightParenthasis;

balancedTokenSequence:
	balancedToken? |
	balancedTokenSequence balancedToken;

// TODO : Add final option - any token other than a parenthesis , a bracket , or a brace
balancedToken:
	LeftParenthasis balancedTokenSequence RightParenthasis |
	LeftBracket balancedTokenSequence RightBracket |
	LeftBrace balancedTokenSequence RightBrace;
