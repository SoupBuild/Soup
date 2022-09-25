grammar SML;

/*
 * Parser Rules
 */
document            : leadingNewlines tableContent trailingNewlines  EOF ;
table               : OPEN_BRACE leadingNewlines tableContent trailingNewlines CLOSE_BRACE ;
tableContent        : (tableValue (delimiter tableValue)*)? ;
tableValue          : KEY COLON value ;
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
COMMENT             : '//'.*?'\r'?'\n' -> channel(HIDDEN) ;
INTEGER             : DIGIT+ ;
KEY                 : [\p{L}\p{N}+#]+ ;
WORD                : (LOWERCASE | UPPERCASE)+ ;
STRING_LITERAL      : '"' BASIC_CHAR*? '"' ;
WHITESPACE          : (' '|'\t')+ -> channel(HIDDEN) ;
