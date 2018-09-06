
lexer grammar CppNumberLiterals;

fragment NonZeroDigit: '1'..'9';
fragment Digit: '0'..'9';
fragment OctalDigit: [0-7];

/****************************************/
/* Number Literals
/****************************************/
DecimalLiteral: 
	NonZeroDigit ('\''? Digit)*;

OctalLiteral:
	'0' ('\''? OctalDigit)*;
