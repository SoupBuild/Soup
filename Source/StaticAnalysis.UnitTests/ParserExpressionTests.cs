using Antlr4.Runtime;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class ParserExpressionTests
    {
        [Theory]
        [InlineData("1")] // Literal
        [InlineData("cout")] // Unqualified Identifier
        [InlineData("std::string::npos")] // Qualified Identifier
        [InlineData("[]() { return 1; }]")] // Lambda Expression
        [InlineData("all(true, true, true, false)")] // Fold Expression
        public void SingleRule_PrimaryExpressions(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.primaryExpression();
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
        [InlineData("std::string::npos")]
        [InlineData("::tolower")]
        [InlineData("::std::count")]
        public void SingleRule_QualifiedIdentifier(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.qualifiedIdentifier();
            Assert.NotNull(context);
        }

        [Theory]
        [InlineData("[]{}")]
        [InlineData("[](){}")]
        [InlineData("[]()->int{return 1;}")]
        //[InlineData("[](auto a, auto&& b) { return a < b; }")]
        [InlineData("[=] { printer(ts...); }")]
        //[InlineData("[]<class T>(T a, auto&& b) { return a < b; }")]
        //[InlineData("[](auto* a)->auto& { return *a; }")]
        //[InlineData("[](int i = 6) { return i + 4; }")]
        [InlineData("[&]{ use(i, x); }")]
        public void SingleRule_LambdaExpression(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.lambdaExpression();
            Assert.NotNull(context);
        }

        [Theory]
        [InlineData("=")]
        [InlineData("+=")]
        [InlineData("-=")]
        [InlineData("*=")]
        [InlineData("/=")]
        [InlineData("%=")]
        [InlineData("^=")]
        [InlineData("&=")]
        [InlineData("|=")]
        [InlineData(">>=")]
        [InlineData("<<=")]
        public void SingleRule_AssignmentOperator(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.assignmentOperator();
            Assert.NotNull(context);
        }

        [Theory]
        [InlineData("+")]
        [InlineData("-")]
        [InlineData("&")]
        [InlineData("|")]
        [InlineData("~")]
        [InlineData("!")]
        public void SingleRule_UnaryOperator(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.unaryOperator();
            Assert.NotNull(context);
        }

        [Theory]
        [InlineData("new")]
        [InlineData("delete")]
        [InlineData("new[]")]
        [InlineData("delete[]")]
        [InlineData("+")]
        [InlineData("-")]
        [InlineData("*")]
        [InlineData("/")]
        [InlineData("%")]
        [InlineData("^")]
        [InlineData("&")]
        [InlineData("|")]
        [InlineData("~")]
        [InlineData("!")]
        [InlineData("=")]
        [InlineData("<")]
        [InlineData(">")]
        [InlineData("+=")]
        [InlineData("-=")]
        [InlineData("*=")]
        [InlineData("/=")]
        [InlineData("%=")]
        [InlineData("^=")]
        [InlineData("&=")]
        [InlineData("|=")]
        [InlineData("<<")]
        [InlineData(">>")]
        [InlineData(">>=")]
        [InlineData("<<=")]
        [InlineData("==")]
        [InlineData("!=")]
        [InlineData("<=")]
        [InlineData(">=")]
        [InlineData("&&")]
        [InlineData("||")]
        [InlineData("++")]
        [InlineData("--")]
        [InlineData(",")]
        [InlineData("->*")]
        [InlineData("->")]
        [InlineData("()")]
        [InlineData("[]")]
        public void SingleRule_AnyOperator(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.anyOperator();
            Assert.NotNull(context);
        }

        [Theory]
        [InlineData("operator new")]
        [InlineData("operator delete")]
        [InlineData("operator new[]")]
        [InlineData("operator delete[]")]
        [InlineData("operator+")]
        [InlineData("operator-")]
        [InlineData("operator*")]
        [InlineData("operator/")]
        [InlineData("operator%")]
        [InlineData("operator^")]
        [InlineData("operator&")]
        [InlineData("operator|")]
        [InlineData("operator~")]
        [InlineData("operator!")]
        [InlineData("operator=")]
        [InlineData("operator<")]
        [InlineData("operator>")]
        [InlineData("operator+=")]
        [InlineData("operator-=")]
        [InlineData("operator*=")]
        [InlineData("operator/=")]
        [InlineData("operator%=")]
        [InlineData("operator^=")]
        [InlineData("operator&=")]
        [InlineData("operator |=")]
        [InlineData("operator <<")]
        [InlineData("operator >>")]
        [InlineData("operator >>=")]
        [InlineData("operator <<=")]
        [InlineData("operator ==")]
        [InlineData("operator !=")]
        [InlineData("operator <=")]
        [InlineData("operator >=")]
        [InlineData("operator &&")]
        [InlineData("operator||")]
        [InlineData("operator++")]
        [InlineData("operator--")]
        [InlineData("operator,")]
        [InlineData("operator->*")]
        [InlineData("operator->")]
        [InlineData("operator()")]
        [InlineData("operator[]")]
        public void SingleRule_OperatorFunctionIdentifier(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.operatorFunctionIdentifier();
            Assert.NotNull(context);
        }

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

        [Theory]
        [InlineData("void DoIt(){}")]
        public void SingleRule_FunctionDefinition(string sourceCode)
        {
            var uut = BuildParser(sourceCode);
            var context = uut.functionDefinition();
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
