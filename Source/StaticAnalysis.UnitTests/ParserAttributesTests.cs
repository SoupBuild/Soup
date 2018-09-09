using Antlr4.Runtime;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class ParserAttributesTests
    {
        [Theory]
        // Standard Attributes
        [InlineData("[[noreturn]]")]
        [InlineData("[[carries_dependency]]")]
        [InlineData("[[deprecated]]")]
        [InlineData("[[deprecated(\"reason\")]]")]
        [InlineData("[[fallthrough]]")]
        [InlineData("[[nodiscard]]")]
        [InlineData("[[maybe_unused]]")]
        public void SingleRule_AttributeSpecifier(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.attributeSpecifier();
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
