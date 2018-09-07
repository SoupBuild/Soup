
lexer grammar CppLexerFragments;

fragment Zero: '0';
fragment BinaryDigit: '0'..'1';
fragment NonZeroDecimalDigit: '1'..'9';
fragment DecimalDigit: '0'..'9';
fragment OctalDigit: '0'..'7';
fragment HexadecimalDigit: '0'..'9' | 'a'..'f' | 'A'..'F';
fragment LowerCaseLetter: 'a'..'z';
fragment UpperCaseLetter: 'A'..'Z';
fragment Letter: LowerCaseLetter | UpperCaseLetter;
fragment NonDigit: Letter | '_';