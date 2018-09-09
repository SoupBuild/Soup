using Antlr4.Runtime;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class ParserTests
    {
        [Theory]
        [InlineData("true")]
        [InlineData("false")]
        public void SingleRule_BooleanLiterals(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.booleanLiteral();
            Assert.NotNull(context);
        }

        [Theory]
        [InlineData("nullptr")]
        public void SingleRule_PointerLiterals(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.pointerLiteral();
            Assert.NotNull(context);
        }

        [Theory]
        [InlineData("1")] // Integer
        [InlineData("1.0")] // FloatingPoint
        [InlineData("'c'")] // Character
        [InlineData("\"s\"")] // String
        [InlineData("true")] // Boolean
        [InlineData("nullptr")] // Pointer
        public void SingleRule_Literals(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.literal();
            Assert.NotNull(context);
        }

        [Theory]
        [InlineData("cout")] // Identifier
        [InlineData("operator new")] // Overloaded Operator Name
        [InlineData("operator bool")] // User defined conversion function
        [InlineData("operator \"\" _km")] // User defined literla operator
        [InlineData("MyTemplate<int>")] // Template
        [InlineData("~MyClass")] // Tilde class name
        [InlineData("~decltype(str)")] // Tilde decltype
        public void SingleRule_UnqualifiedIdentifier(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.unqualifiedIdentifier();
            Assert.NotNull(context);
        }

        [Theory]
        [InlineData("1")] // Literal
        [InlineData("cout")] // Unqualified Identifier
        public void SingleRule_PrimaryExpressions(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.primaryExpression();
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
