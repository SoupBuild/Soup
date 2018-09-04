
grammar CppStatements;
statement:
labeled-statement
attribute-specifier-seqopt expression-statement
attribute-specifier-seqopt compound-statement
attribute-specifier-seqopt selection-statement
attribute-specifier-seqopt iteration-statement
attribute-specifier-seqopt jump-statement
declaration-statement
attribute-specifier-seqopt try-block
labeled-statement:
attribute-specifier-seqopt identifier : statement
attribute-specifier-seqoptcase constant-expression : statement
attribute-specifier-seqoptdefault : statement
expression-statement:
expressionopt;
compound-statement:
{ statement-seqopt}
statement-seq:
statement
statement-seq statement
selection-statement:
if ( condition ) statement
if ( condition ) statement else statement
switch ( condition ) statement
condition:
expression
attribute-specifier-seqopt decl-specifier-seq declarator = initializer-clause
attribute-specifier-seqopt decl-specifier-seq declarator braced-init-list
iteration-statement:
while ( condition ) statement
do statement while ( expression ) ;
for ( for-init-statement conditionopt; expressionopt) statement
for ( for-range-declaration : for-range-initializer ) statement
for-init-statement:
expression-statement
simple-declaration
for-range-declaration:
attribute-specifier-seqopt decl-specifier-seq declarator
for-range-initializer:
expression
braced-init-list
jump-statement:
break ;
continue ;
return expressionopt;
return braced-init-list ;
goto identifier ;
declaration-statement:
block-declaration