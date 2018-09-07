
grammar CppParserDeclarators;

init-declarator-list:
init-declarator
init-declarator-list , init-declarator
init-declarator:
declarator initializeropt
declarator:
ptr-declarator
noptr-declarator parameters-and-qualifiers trailing-return-type
ptr-declarator:
noptr-declarator
ptr-operator ptr-declarator
noptr-declarator:
declarator-id attribute-specifier-seqopt
noptr-declarator parameters-and-qualifiers
noptr-declarator [ constant-expressionopt] attribute-specifier-seqopt
( ptr-declarator )
parameters-and-qualifiers:
( parameter-declaration-clause ) cv-qualifier-seqopt
ref-qualifieropt exception-specificationopt attribute-specifier-seqopt
trailing-return-type:
-> trailing-type-specifier-seq abstract-declaratoropt
ptr-operator:
* attribute-specifier-seqopt cv-qualifier-seqopt
& attribute-specifier-seqopt
&& attribute-specifier-seqopt
nested-name-specifier * attribute-specifier-seqopt cv-qualifier-seqopt
cv-qualifier-seq:
cv-qualifier cv-qualifier-seqopt

cv-qualifier:
const
volatile
ref-qualifier:
&
&&
declarator-id:
...opt id-expression
type-id:
type-specifier-seq abstract-declaratoropt
abstract-declarator:
ptr-abstract-declarator
noptr-abstract-declaratoropt parameters-and-qualifiers trailing-return-type
abstract-pack-declarator
ptr-abstract-declarator:
noptr-abstract-declarator
ptr-operator ptr-abstract-declaratoropt
noptr-abstract-declarator:
noptr-abstract-declaratoropt parameters-and-qualifiers
noptr-abstract-declaratoropt[ constant-expressionopt ] attribute-specifier-seqopt
( ptr-abstract-declarator )
abstract-pack-declarator:
noptr-abstract-pack-declarator
ptr-operator abstract-pack-declarator
noptr-abstract-pack-declarator:
noptr-abstract-pack-declarator parameters-and-qualifiers
noptr-abstract-pack-declarator [ constant-expressionopt ] attribute-specifier-seqopt
...
parameter-declaration-clause:
parameter-declaration-listopt...opt
parameter-declaration-list , ...
parameter-declaration-list:
parameter-declaration
parameter-declaration-list , parameter-declaration

										   parameter-declaration:
attribute-specifier-seqopt decl-specifier-seq declarator
attribute-specifier-seqopt decl-specifier-seq declarator = initializer-clause
attribute-specifier-seqopt decl-specifier-seq abstract-declaratoropt
attribute-specifier-seqopt decl-specifier-seq abstract-declaratoropt= initializer-clause
function-definition:
attribute-specifier-seqopt decl-specifier-seqopt declarator virt-specifier-seqopt function-body
function-body:
ctor-initializeropt compound-statement
function-try-block
= default ;
= delete ;
initializer:
brace-or-equal-initializer
( expression-list )
brace-or-equal-initializer:
= initializer-clause
braced-init-list
initializer-clause:
assignment-expression
braced-init-list
initializer-list:
initializer-clause ...opt
initializer-list , initializer-clause ...opt

braced-init-list:
{ initializer-list ,opt }
{ }
