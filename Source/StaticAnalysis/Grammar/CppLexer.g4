/*******************************************************************************
 * C++ 17 Lexer Grammar for ANTLR v4
 *
 * Based on N4659 final working paper
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf
 ******************************************************************************/

lexer grammar CppLexer;

// Source Character Set
// a b c d e f g h i j k l m n o p q r s t u v w x y z
// A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
// 0 1 2 3 4 5 6 7 8 9
// _ { } [ ] # ( ) < > % : ; . ? * + - / ^ & | ~ ! = , \ " ’
// space horizontal-tab vertical-tab form-feed new-line

// Ensure all keywards and operators come before literals
import 
	CppLexerKeywords,
	CppLexerOperatorsAndPunctuators,
	CppLexerIntegerLiterals,
	CppLexerFloatingPointLiterals,
	CppLexerCharacterLiterals,
	CppLexerStringLiterals;

Whitespace: (' ' | '\t')+;
Newline: ('\r' '\n'? | '\n');
