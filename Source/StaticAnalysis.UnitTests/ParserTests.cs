using Antlr4.Runtime;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class ParserTests
    {
        [Theory]
        [InlineData("true")]
        [InlineData("false")]
        public void SingleRule_Literal(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.literal();
            Assert.NotNull(context);
        }

        private CppParser BuildParser(string sourceCode)
        {
            // Parse the file
            var inputStream = new AntlrInputStream(sourceCode);
            var lexer = new CppLexer(inputStream);
            var tokenStream = new CommonTokenStream(lexer);
            var parser = new CppParser(tokenStream);

            // Setup error handling
            lexer.RemoveErrorListeners();
            lexer.AddErrorListener(new LexerExceptionErrorListener());
            parser.RemoveErrorListeners();
            parser.AddErrorListener(new ParserExceptionErrorListener());

            return parser;
        }
    }
}
