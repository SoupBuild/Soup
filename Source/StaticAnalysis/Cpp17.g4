/*******************************************************************************
 * C++ 17 Grammar for ANTLR v4
 *
 * Based on n4140 draft paper
 * https://github.com/cplusplus/draft/blob/master/papers/n4140.pdf
 ******************************************************************************/

grammar Cpp17;
import CppExpressions;

// A file consists of a single translation unit with a sequence of declarations
translationUnit:
	declarationSequence? EOF;