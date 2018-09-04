using Antlr4.Runtime;
using System;
using System.IO;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class UnitTest1
    {
        [Fact]
        public void Test1()
        {
            RunTest(@"Examples\Namespace.cpp");
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
            var lexer = new Cpp17Lexer(inputStream);
            var tokenStream = new CommonTokenStream(lexer);
            var parser = new Cpp17Parser(tokenStream);

            // Setup error handling
            parser.RemoveErrorListeners();
            parser.AddErrorListener(new ExceptionErrorListener());

            // Parse syntax tree
            var translationUnit = parser.translationUnit();

            // Visit the top level unit
            var visitor = new Visitor();
            visitor.Visit(translationUnit);
        }
    }
}
