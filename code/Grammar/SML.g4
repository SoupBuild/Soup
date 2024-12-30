grammar SML;

/*
 * Parser Rules
 */
document            : leadingNewlines tableContent trailingNewlines EOF ;
table               : OPEN_BRACE leadingNewlines tableContent trailingNewlines CLOSE_BRACE ;
tableContent        : (tableValue (delimiter tableValue)*)? ;
tableValue          : key COLON value ;
key                 : ALPHA_LITERAL # keyLiteral
                    | STRING_LITERAL # keyString
                    ;
array               : OPEN_BRACKET leadingNewlines arrayContent trailingNewlines CLOSE_BRACKET ;
arrayContent        : (value (delimiter value)*)? ;
languageName        : (ALPHA_LITERAL | ALPHA_EXT3_LITERAL);
userName            : (ALPHA_LITERAL | ALPHA_EXT1_LITERAL) ;
packageName         : (ALPHA_LITERAL | ALPHA_EXT2_LITERAL) ;
language            : OPEN_PARENTHESIS languageName CLOSE_PARENTHESIS ;
languageReference   : OPEN_PARENTHESIS languageName AT_SIGN (INTEGER | FLOAT) CLOSE_PARENTHESIS ;
packageReference    : LESS_THAN language? userName PIPE packageName AT_SIGN (INTEGER | FLOAT) GREATER_THAN ;
value               : FLOAT # valueFloat
                    | INTEGER # valueInteger
                    | languageReference # valueLanguageReference
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
fragment BASIC_CHAR       : [\t\p{L}\p{N}\p{M}\p{P}\p{S}\p{Z}] ;

COLON               : ':' ;
AT_SIGN             : '@' ;
OPEN_PARENTHESIS    : '(' ;
CLOSE_PARENTHESIS   : ')' ;
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
ALPHA_LITERAL       : ALPHA ALPHANUMERIC* ;
ALPHA_EXT1_LITERAL  : ALPHA (ALPHANUMERIC | [-])* ;
ALPHA_EXT2_LITERAL  : ALPHA (ALPHANUMERIC | [.])* ;
ALPHA_EXT3_LITERAL  : ALPHA (ALPHANUMERIC | [+#])* ;
STRING_LITERAL      : '\'' BASIC_CHAR*? '\'' ;
WHITESPACE          : (' '|'\t')+ -> channel(HIDDEN) ;
