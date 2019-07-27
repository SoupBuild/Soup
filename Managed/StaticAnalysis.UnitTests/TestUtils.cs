using Antlr4.Runtime;
using Soup.StaticAnalysis.AST;
using System.Collections.Generic;
using System.IO;

namespace Soup.StaticAnalysis.UnitTests
{
    public class TestUtils
    {
        public static TranslationUnit GenerateAST(AntlrInputStream inputStream)
        {
            // Parse the file
            var lexer = new CppLexer(inputStream);
            var tokenStream = new CommonTokenStream(lexer);
            var parser = new CppParser(tokenStream);

            // var tokens = lexer.GetAllTokens().Select(token => lexer.Vocabulary.GetSymbolicName(token.Type)).ToList();

            // Setup error handling
            lexer.RemoveErrorListeners();
            lexer.AddErrorListener(new LexerExceptionErrorListener());
            parser.RemoveErrorListeners();
            parser.AddErrorListener(new ParserExceptionErrorListener());

            // Parse the concrete syntax tree
            var translationUnit = parser.translationUnit();

            // Convert the the abstract syntax tree
            var visitor = new ASTVisitor();
            var ast = (TranslationUnit)visitor.Visit(translationUnit);
            return ast;
        }

        public static TranslationUnit CreateSingleDeclaration(Declaration declaration)
        {
            return new TranslationUnit()
            {
                Declarations = new DeclarationSequence(
                    new List<Declaration>()
                    {
                        declaration,
                    }),
            };
        }
    }
}
