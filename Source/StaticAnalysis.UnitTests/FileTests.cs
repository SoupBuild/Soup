using Antlr4.Runtime;
using System;
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

            lexer.RemoveErrorListeners();
            lexer.AddErrorListener(new LexerExceptionErrorListener());

            var tokens = lexer.GetAllTokens();

            ////var tokenStream = new CommonTokenStream(lexer);
            ////var parser = new Cpp17Parser(tokenStream);

            ////// Setup error handling
            ////parser.RemoveErrorListeners();
            ////parser.AddErrorListener(new ExceptionErrorListener());

            ////// Parse syntax tree
            ////var translationUnit = parser.translationUnit();

            ////// Visit the top level unit
            ////var visitor = new Visitor();
            ////visitor.Visit(translationUnit);
        }
    }
}
