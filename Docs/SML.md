# SML
SML stands for "Simple Minimal Language". It is a simple declarative language that has its roots in [Toml](https://toml.io/en/). SML removes the overly complex Table and dot key syntax from Toml in favor of a more direct hierarchical representation. I believe this leads to a clearer strict view with less variance for "odd" interleaving of values and makes parsing straightforward as tables cannot "spring into existence". It also drops support for date-times, since they are very hard to get right and not that important to build definitions.

Note: SML is very much a work in progress and is a key part of the Beta stabilization phase.

```antlr
grammar SML;

/*
 * Parser Rules
 */
document            : leadingNewlines tableContent trailingNewlines  EOF ;
table               : OPEN_BRACE leadingNewlines tableContent trailingNewlines CLOSE_BRACE ;
tableContent        : (tableValue (delimiter tableValue)*)? ;
tableValue          : key COLON value ;
key                 : KEY_LITERAL # keyLiteral
                    | STRING_LITERAL # keyString
                    ;
array               : OPEN_BRACKET leadingNewlines arrayContent trailingNewlines CLOSE_BRACKET ;
arrayContent        : (value (delimiter value)*)? ;
value               : INTEGER # valueInteger
                    | STRING_LITERAL # valueString
                    | TRUE # valueTrue
                    | FALSE # valueFalse
                    | table # valueTable
                    | array # valueArray
                    ;
delimiter           : NEWLINE+ # newlineDelimiter
                    | COMMA # commaDelimiter
                    ;
leadingNewlines     : NEWLINE* ;
trailingNewlines    : NEWLINE* ;

/*
 * Lexer Rules
 */
fragment DIGIT      : [0-9] ;
fragment LOWERCASE  : [a-z] ;
fragment UPPERCASE  : [A-Z] ;
fragment BASIC_CHAR : [\t\p{L}\p{N}\p{M}\p{P}\p{S}\p{Z}] ;

COLON               : ':' ;
OPEN_BRACKET        : '[' ;
CLOSE_BRACKET       : ']' ;
OPEN_BRACE          : '{' ;
CLOSE_BRACE         : '}' ;
COMMA               : ',' ;
TRUE                : 'true' ;
FALSE               : 'false' ;
NEWLINE             : '\r'?'\n' ;
COMMENT             : '#'.*?'\r'?'\n' -> channel(HIDDEN) ;
INTEGER             : DIGIT+ ;
KEY_LITERAL         : [\p{L}\p{N}]+ ;
WORD                : (LOWERCASE | UPPERCASE)+ ;
STRING_LITERAL      : '"' BASIC_CHAR*? '"' ;
WHITESPACE          : (' '|'\t')+ -> channel(HIDDEN) ;
```