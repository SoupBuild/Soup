/*******************************************************************************
 * C++ 17 Parser Grammar for ANTLR v4
 *
 * Based on N4659 final working paper
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/n4659.pdf
 ******************************************************************************/

parser grammar CppParser;
options { tokenVocab = CppLexer; }
import CppParserDeclarations;

// A file consists of a single translation unit with a sequence of declarations
translationUnit:
	declarationSequence? EOF;
