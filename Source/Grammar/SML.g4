grammar SML;

/*
 * Parser Rules
 */
document            : table_content EOF ;
table_content       : assign_value* ;
array_content       : (value (COMMA value)* COMMA?)? ;
assign_value        : KEY EQUALS value ;
value               : INTEGER # valueInteger
                    | STRING_LITERAL # valueString
                    | TRUE # valueTrue
                    | FALSE # valueFalse
                    | table # valueTable
                    | array # valueArray
                    ;
table               : OPEN_BRACE table_content CLOSE_BRACE ;
array               : OPEN_BRACKET array_content CLOSE_BRACKET ;

/*
 * Lexer Rules
 */
fragment DIGIT      : [0-9] ;
fragment LOWERCASE  : [a-z] ;
fragment UPPERCASE  : [A-Z] ;
fragment BASIC_CHAR : [\t\p{L}\p{N}\p{M}\p{P}\p{S}\p{Z}] ;

EQUALS              : '=' ;
OPEN_BRACKET        : '[' ;
CLOSE_BRACKET       : ']' ;
OPEN_BRACE          : '{' ;
CLOSE_BRACE         : '}' ;
COMMA               : ',' ;
TRUE                : 'true' ;
FALSE               : 'false' ;
COMMENT             : '//'.*?'\r'?'\n' -> skip ;
INTEGER             : DIGIT+ ;
KEY                 : [\p{L}\p{N}+#]+ ;
WORD                : (LOWERCASE | UPPERCASE)+ ;
STRING_LITERAL      : '"' BASIC_CHAR*? '"' ;
WHITESPACE          : (' '|'\t'|'\n'|'\r')+ -> skip ;
