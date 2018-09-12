using Antlr4.Runtime;
using System.IO;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class FileTests
    {
        [Fact]
        public void BlockComment()
        {
            RunTest(@"Examples/BlockComment.cpp");
        }

        [Fact]
        public void HelloWorld()
        {
            RunTest(@"Examples/HelloWorld.cpp");
        }

        [Fact]
        public void LineComment()
        {
            RunTest(@"Examples/LineComment.cpp");
        }

        [Fact]
        public void Namespace()
        {
            RunTest(@"Examples/Namespace.cpp");
        }

        private void RunTest(string filename)
        {
            using (var file = File.OpenRead(filename))
            {
                RunTest(file);
            }
        }

        private void RunTest(Stream sourceCode)
        {
            // Parse the file
            var inputStream = new AntlrInputStream(sourceCode);
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
            var ast = visitor.Visit(translationUnit);
            Assert.NotNull(ast);
        }
    }
}
