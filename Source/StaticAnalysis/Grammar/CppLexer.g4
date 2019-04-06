/*******************************************************************************
 * C++ 17 Lexer Grammar for ANTLR v4
 *
 * Based on N4659 final working paper
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf
 ******************************************************************************/

lexer grammar CppLexer;

// Ensure all keywards and operators come before literals and identifiers
import 
	CppLexerKeywords,
	CppLexerOperatorsAndPunctuators,
	CppLexerIntegerLiterals,
	CppLexerFloatingPointLiterals,
	CppLexerCharacterLiterals,
	CppLexerStringLiterals,
	CppLexerUserDefinedLiterals,
	CppLexerIdentifiers;

BlockComment: '/*' .*? '*/' -> channel(1);
LineComment: '//' ~[\r\n]* -> channel(1);

Whitespace: (' ' | '\t')+ -> channel(2);
Newline: ('\r' '\n'? | '\n') -> channel(2);
