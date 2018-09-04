/*******************************************************************************
 * C++ 17 Grammar for ANTLR v4
 *
 * Based on n4140 draft paper
 * https://github.com/cplusplus/draft/blob/master/papers/n4140.pdf
 ******************************************************************************/

grammar Cpp17;
import CppKeywords;

// A file consists of a single translational unit with a sequence of declarations
translationUnit : 
	declarationSequence? EOF;

// A delaration sequence is one or more declarations
declarationSequence :
	declaration |
	declarationSequence declaration;

// A list of all acceptable declarations
declaration :
	namespaceDefinition;

namespaceDefinition :
	Namespace;
