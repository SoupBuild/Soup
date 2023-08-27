# Simple Minimal Language
Simple Minimal Language (SML) is a simple declarative language that has its roots in [Toml](https://toml.io/en/). SML removes the overly complex Table and dot key syntax from Toml in favor of a more direct hierarchical representation. This enforces a clearer, strict view with less variance for "odd" interleaving of values. It also makes parsing straightforward as tables cannot "spring into existence". Support for date-times is removed, since they are very hard to get right and not that important to build definitions.

In SML newline characters (`\r\n`) and comma (`,`) characters can be used interchangeably for delimiters, however they cannot both be used at the same time. This removes unnecessary commas when it is already clear that the items are separated by a newline. But still allows for the comma to clearly define items that are on the same line. Newline characters can have one or more in a row to allow for vertical whitespace.

## Example.sml
```sml
# Here is a comment
KeyName: "A String Value"
"KeyWithSpecial#@": 1234
IsEnabled: true
AnArray: [
    "Main.cpp"
    1234
]

Table1: {
    ChildArray: [ "value1", "value2" ]
}
```

> Note: SML is very much a work in progress and is a key part of the Beta stabilization phase.

## Grammar
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