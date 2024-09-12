grammar SML;

/*
 * Parser Rules
 */
document            : leadingNewlines tableContent trailingNewlines EOF ;
table               : OPEN_BRACE leadingNewlines tableContent trailingNewlines CLOSE_BRACE ;
tableContent        : (tableValue (delimiter tableValue)*)? ;
tableValue          : key COLON value ;
key                 : KEY_LITERAL # keyLiteral
                    | STRING_LITERAL # keyString
                    ;
array               : OPEN_BRACKET leadingNewlines arrayContent trailingNewlines CLOSE_BRACKET ;
arrayContent        : (value (delimiter value)*)? ;
userName            : (USER_NAME PIPE)? ;
packageReference    : LESS_THAN userName PACKAGE_NAME AT_SIGN (INTEGER | FLOAT) GREATER_THAN ;
value               : FLOAT # valueFloat
                    | INTEGER # valueInteger
                    | packageReference # valuePackageReference
                    | VERSION # valueVersion
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
fragment DIGIT            : [0-9] ;
fragment ALPHA            : [A-Za-z] ;
fragment ALPHANUMERIC     : [A-Za-z0-9] ;
fragment EXT_ALPHANUMERIC : [A-Za-z0-9+#] ;
fragment BASIC_CHAR       : [\t\p{L}\p{N}\p{M}\p{P}\p{S}\p{Z}] ;

COLON               : ':' ;
AT_SIGN             : '@' ;
LESS_THAN           : '<' ;
GREATER_THAN        : '>' ;
OPEN_BRACKET        : '[' ;
CLOSE_BRACKET       : ']' ;
OPEN_BRACE          : '{' ;
CLOSE_BRACE         : '}' ;
PIPE                : '|' ;
COMMA               : ',' ;
TRUE                : 'true' ;
FALSE               : 'false' ;
NEWLINE             : '\r'?'\n' ;
COMMENT             : '#'.*?'\r'?'\n' -> channel(HIDDEN) ;
VERSION             : DIGIT+ '.' DIGIT+ '.' DIGIT+ ;
FLOAT               : DIGIT+ '.' DIGIT+ ;
INTEGER             : DIGIT+ ;
KEY_LITERAL         : ALPHA ALPHANUMERIC* ;
USER_NAME           : ALPHA ALPHANUMERIC* ;
PACKAGE_NAME        : ALPHA EXT_ALPHANUMERIC* ;
STRING_LITERAL      : '\'' BASIC_CHAR*? '\'' ;
WHITESPACE          : (' '|'\t')+ -> channel(HIDDEN) ;
