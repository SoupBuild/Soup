
grammar CppDeclarations;

declaration-seq:
declaration
declaration-seq declaration
declaration:
block-declaration
function-definition
template-declaration
explicit-instantiation
explicit-specialization
linkage-specification
namespace-definition
empty-declaration
attribute-declaration
block-declaration:
simple-declaration
asm-definition
namespace-alias-definition
using-declaration
using-directive
static_assert-declaration
alias-declaration
opaque-enum-declaration
alias-declaration:
using identifier attribute-specifier-seqopt= type-id ;
simple-declaration:
decl-specifier-seqopt init-declarator-listopt;
attribute-specifier-seq decl-specifier-seqopt init-declarator-list ;static_assert-declaration:
static_assert ( constant-expression , string-literal ) ;
empty-declaration:
;
attribute-declaration:
attribute-specifier-seq ;
decl-specifier:
storage-class-specifier
type-specifier
function-specifier
friend
typedef
constexpr
decl-specifier-seq:
decl-specifier attribute-specifier-seqopt
decl-specifier decl-specifier-seq
storage-class-specifier:
register
static
thread_local
extern
mutable
function-specifier:
inline
virtual
explicit
typedef-name:
identifier
type-specifier:
trailing-type-specifier
class-specifier
enum-specifier

		 trailing-type-specifier:
simple-type-specifier
elaborated-type-specifier
typename-specifier
cv-qualifier
type-specifier-seq:
type-specifier attribute-specifier-seqopt
type-specifier type-specifier-seq
trailing-type-specifier-seq:
trailing-type-specifier attribute-specifier-seqopt
trailing-type-specifier trailing-type-specifier-seq

simple-type-specifier:
nested-name-specifieropt type-name
nested-name-specifier template simple-template-id
char
char16_t
char32_t
wchar_t
bool
short
int
long
signed
unsigned
float
double
void
auto
decltype-specifier
type-name:
class-name
enum-name
typedef-name
simple-template-id
decltype-specifier:
decltype ( expression )
decltype ( auto )
elaborated-type-specifier:
class-key attribute-specifier-seqopt nested-name-specifieropt identifier
class-key simple-template-id
class-key nested-name-specifier templateopt simple-template-id
enum nested-name-specifieropt identifier

enum-name:
identifier
enum-specifier:
enum-head { enumerator-listopt}
enum-head { enumerator-list , }
enum-head:
enum-key attribute-specifier-seqopt identifieropt enum-baseopt
enum-key attribute-specifier-seqopt nested-name-specifier identifier
enum-baseopt
opaque-enum-declaration:
enum-key attribute-specifier-seqopt identifier enum-baseopt;
enum-key:
enum
enum class
enum struct
enum-base:
: type-specifier-seq
enumerator-list:
enumerator-definition
enumerator-list , enumerator-definition
enumerator-definition:
enumerator
enumerator = constant-expression

enumerator:
identifier
namespace-name:
original-namespace-name
namespace-alias
original-namespace-name:
identifier
namespace-definition:
named-namespace-definition
unnamed-namespace-definition
named-namespace-definition:
original-namespace-definition
extension-namespace-definition
original-namespace-definition:
inlineopt namespace identifier { namespace-body }
extension-namespace-definition:
inlineopt namespace original-namespace-name { namespace-body }
unnamed-namespace-definition:
inlineopt namespace { namespace-body }
namespace-body:
declaration-seqopt
namespace-alias:
identifier
namespace-alias-definition:
namespace identifier = qualified-namespace-specifier ;
qualified-namespace-specifier:
nested-name-specifieropt namespace-name
using-declaration:
using typenameopt nested-name-specifier unqualified-id ;
using :: unqualified-id ;
using-directive:
attribute-specifier-seqoptusing namespace nested-name-specifieropt namespace-name ;

asm-definition:
asm ( string-literal ) ;
linkage-specification:
extern string-literal { declaration-seqopt}
extern string-literal declaration
attribute-specifier-seq:
attribute-specifier-seqopt attribute-specifier
attribute-specifier:
[ [ attribute-list ] ]
alignment-specifier
alignment-specifier:
alignas ( type-id ...opt)
alignas ( constant-expression ...opt)
attribute-list:
attributeopt
attribute-list , attributeopt
attribute ...
attribute-list , attribute ...
attribute:
attribute-token attribute-argument-clauseopt

attribute-token:
identifier
attribute-scoped-token
attribute-scoped-token:
attribute-namespace :: identifier
attribute-namespace:
identifier
attribute-argument-clause:
( balanced-token-seq )
balanced-token-seq:
balanced-tokenopt
balanced-token-seq balanced-token
balanced-token:
( balanced-token-seq )
[ balanced-token-seq ]
{ balanced-token-seq }
any token other than a parenthesis, a bracket, or a brace