
grammar CppParserClasses;

/**************************/
/* Classes
/**************************/
class-name:
	identifier |
	simple-templateId;

class-specifier:
	class-head { member-specification?};

class-head:
	class-key attribute-specifier-seq? class-head-name class-virt-specifier? base-clause? |
	class-key attribute-specifier-seq? base-clause?;

class-head-name:
	nested-name-specifier? class-name;

class-virt-specifier:
	final;

classKey:
	Class |
	Struct |
	Union;

member-specification:
	member-declaration member-specification? |
	access-specifier : member-specification?;

member-declaration:
	attribute-specifier-seq? decl-specifier-seq? member-declaratorList? SemiColon |
	function-definition |
	using-declaration |
	static_assert-declaration |
	template-declaration |
	alias-declaration |
	empty-declaration;

member-declaratorList:
	member-declarator |
	member-declaratorList , member-declarator;

member-declarator:
	declarator virt-specifier-seq? pure-specifier? |
	declarator brace-or-equalInitializer? |
	identifier? attribute-specifier-seq?: constant-expression;

virt-specifier-seq:
	virt-specifier |
	virt-specifier-seq virt-specifier;

virt-specifier:
	override |
	final;

pure-specifier:
	= 0;

/**************************/
/* Derived Classes
/**************************/
base-clause:
	Colon base-specifierList;

base-specifierList:
	base-specifier ...? |
	base-specifierList , base-specifier ...?;

base-specifier:
	attribute-specifier-seq? base-type-specifier |
	attribute-specifier-seq?virtual access-specifier? base-type-specifier |
	attribute-specifier-seq? access-specifier virtual? base-type-specifier;

class-or-decltype:
	nested-name-specifier? class-name |
	decltype-specifier;

base-type-specifier:
	class-or-decltype;

access-specifier:
	private |
	protected |
	public;

/**************************/
/* Special Member Functions
/**************************/
conversionFunctionId:
	operator conversion-typeId;

conversion-typeId:
	type-specifier-seq conversion-declarator?;

conversion-declarator:
	ptr-operator conversion-declarator?;

ctorInitializer:
	: memInitializerList;

memInitializerList:
	memInitializer ...? |
	memInitializer ...?, memInitializerList;

memInitializer:
	memInitializerId ( expressionList?) |
	memInitializerId bracedInitList;

memInitializerId:
	class-or-decltype
	identifier;

/**************************/
/* Overloading
/**************************/
operatorFunctionId:
	operator operator;

operator:
	New |
	Delete |
	New LeftBracket RightBracket |
	Delete LeftBracket RightBracket |
	Plus |
	Minus |
	Asteriks |
	ForwardSlash |
	Percent |
	Caret |
	Ampersand |
	VerticalBar |
	Tilde |
	ExclamationMark |
	Equals |
	LessThan |
	GreaterThan |
	PlusEquals |
	MinusEquals |
	AsteriksEquals |
	ForwardSlashEquals |
	PercentEquals |
	CaretEquals  |
	AmpersandEquals  |
	VerticalBarEquals |
	DoubleLessThan |
	DoubleGreaterThan |
	DoubleLessThanEquals |
	DoubleGreaterThanEquals |
	DoubleEquals  |
	ExclamationMarkEquals |
	LessThanEquals |
	GreaterThanEquals  |
	DoubleAmpersand |
	DoubleVerticalBar |
	DoublePlus  |
	DoubleMinus |
	Comma |
	ArrowAsteriks |
	Arrow |
	LeftParethasis |
	RightParethasis |
	LeftBracket |
	RightBracket;

literal-operatorId:
	operator stringLiteral identifier |
	operator user-defined-stringLiteral;
