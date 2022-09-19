grammar SML;

/*
 * Parser Rules
 */
document            : tableContent EOF ;
table               : OPEN_BRACE tableContent CLOSE_BRACE ;
tableContent        : NEWLINE* (tableValue (delimiter tableValue)* NEWLINE*)? ;
tableValue          : KEY COLON value ;
array               : OPEN_BRACKET arrayContent CLOSE_BRACKET ;
arrayContent        : NEWLINE* (value (delimiter value)* NEWLINE*)? ;
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
