using Antlr4.Runtime;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class LexerTests
    {
        [Theory]
        [InlineData("alignas", CppLexer.AlignAs)]
        [InlineData("asm", CppLexer.Asm)]
        [InlineData("auto", CppLexer.Auto)]
        [InlineData("bool", CppLexer.Bool)]
        [InlineData("break", CppLexer.Break)]
        [InlineData("case", CppLexer.Case)]
        [InlineData("catch", CppLexer.Catch)]
        [InlineData("char", CppLexer.Char)]
        [InlineData("char16_t", CppLexer.Char16)]
        [InlineData("char32_t", CppLexer.Char32)]
        [InlineData("class", CppLexer.Class)]
        [InlineData("const", CppLexer.Const)]
        [InlineData("constexpr", CppLexer.Constexpr)]
        [InlineData("const_cast", CppLexer.ConstCast)]
        [InlineData("continue", CppLexer.Continue)]
        [InlineData("decltype", CppLexer.Decltype)]
        [InlineData("default", CppLexer.Default)]
        [InlineData("delete", CppLexer.Delete)]
        [InlineData("do", CppLexer.Do)]
        [InlineData("double", CppLexer.Double)]
        [InlineData("dynamic_cast", CppLexer.DynamicCast)]
        [InlineData("else", CppLexer.Else)]
        [InlineData("enum", CppLexer.Enum)]
        [InlineData("explicit", CppLexer.Explicit)]
        [InlineData("export", CppLexer.Export)]
        [InlineData("extern", CppLexer.Extern)]
        [InlineData("false", CppLexer.False)]
        [InlineData("float", CppLexer.Float)]
        [InlineData("for", CppLexer.For)]
        [InlineData("friend", CppLexer.Friend)]
        [InlineData("goto", CppLexer.GoTo)]
        [InlineData("if", CppLexer.If)]
        [InlineData("inline", CppLexer.Inline)]
        [InlineData("int", CppLexer.Int)]
        [InlineData("long", CppLexer.Long)]
        [InlineData("mutable", CppLexer.Mutable)]
        [InlineData("namespace", CppLexer.Namespace)]
        [InlineData("new", CppLexer.New)]
        [InlineData("noexcept", CppLexer.NoExcept)]
        [InlineData("nullptr", CppLexer.Nullptr)]
        [InlineData("operator", CppLexer.Operator)]
        [InlineData("private", CppLexer.Private)]
        [InlineData("protected", CppLexer.Protected)]
        [InlineData("public", CppLexer.Public)]
        [InlineData("register", CppLexer.Register)]
        [InlineData("reinterpret_cast", CppLexer.ReinterpretCast)]
        [InlineData("return", CppLexer.Return)]
        [InlineData("short", CppLexer.Short)]
        [InlineData("signed", CppLexer.Signed)]
        [InlineData("sizeof", CppLexer.SizeOf)]
        [InlineData("static", CppLexer.Static)]
        [InlineData("static_assert", CppLexer.StaticAssert)]
        [InlineData("static_cast", CppLexer.StaticCast)]
        [InlineData("struct", CppLexer.Struct)]
        [InlineData("switch", CppLexer.Switch)]
        [InlineData("template", CppLexer.Template)]
        [InlineData("this", CppLexer.This)]
        [InlineData("thread_local", CppLexer.ThreadLocal)]
        [InlineData("throw", CppLexer.Throw)]
        [InlineData("true", CppLexer.True)]
        [InlineData("try", CppLexer.Try)]
        [InlineData("typedef", CppLexer.TypeDef)]
        [InlineData("typeid", CppLexer.TypeId)]
        [InlineData("typename", CppLexer.TypeName)]
        [InlineData("union", CppLexer.Union)]
        [InlineData("unsigned", CppLexer.Unsigned)]
        [InlineData("using", CppLexer.Using)]
        [InlineData("virtual", CppLexer.Virtual)]
        [InlineData("void", CppLexer.Void)]
        [InlineData("volatile", CppLexer.Volatile)]
        [InlineData("wchar_t", CppLexer.WChar)]
        [InlineData("while", CppLexer.While)]
        public void SingleToken_Keywoard(string value, int expectedToken)
        {
            RunTest(value, expectedToken);
        }

        [Theory]
        [InlineData("{", CppLexer.LeftBrace)]
        [InlineData("}", CppLexer.RightBrace)]
        [InlineData("[", CppLexer.LeftBracket)]
        [InlineData("]", CppLexer.RightBracket)]
        [InlineData("(", CppLexer.LeftParenthesis)]
        [InlineData(")", CppLexer.RightParenthesis)]
        [InlineData(";", CppLexer.Semicolon)]
        [InlineData(":", CppLexer.Colon)]
        [InlineData("...", CppLexer.Ellipsis)]
        [InlineData("new", CppLexer.New)]
        [InlineData("delete", CppLexer.Delete)]
        [InlineData("?", CppLexer.QuestionMark)]
        [InlineData("::", CppLexer.DoubleColon)]
        [InlineData(".", CppLexer.Period)]
        [InlineData(".*", CppLexer.PeriodAsterisk)]
        [InlineData("+", CppLexer.Plus)]
        [InlineData("-", CppLexer.Minus)]
        [InlineData("*", CppLexer.Asterisk)]
        [InlineData("/", CppLexer.ForwardSlash)]
        [InlineData("%", CppLexer.Percent)]
        [InlineData("^", CppLexer.Caret)]
        [InlineData("&", CppLexer.Ampersand)]
        [InlineData("|", CppLexer.VerticalBar)]
        [InlineData("~", CppLexer.Tilde)]
        [InlineData("!", CppLexer.ExclamationMark)]
        [InlineData("=", CppLexer.Equal)]
        [InlineData("<", CppLexer.LessThan)]
        [InlineData(">", CppLexer.GreaterThan)]
        [InlineData("+=", CppLexer.PlusEquals)]
        [InlineData("-=", CppLexer.MinusEquals)]
        [InlineData("*=", CppLexer.AsterikEquals)]
        [InlineData("/=", CppLexer.ForwardSlashEquals)]
        [InlineData("%=", CppLexer.PercentEquals)]
        [InlineData("^=", CppLexer.CaretEquals)]
        [InlineData("&=", CppLexer.AmpersandEquals)]
        [InlineData("|=", CppLexer.VerticalBarEquals)]
        [InlineData("<<", CppLexer.DoubleLessThan)]
        [InlineData(">>", CppLexer.DoubleGreaterThan)]
        [InlineData("<<=", CppLexer.DoubleLessThanEquals)]
        [InlineData(">>=", CppLexer.DoubleGreaterThanEquals)]
        [InlineData("==", CppLexer.DoubleEquals)]
        [InlineData("!=", CppLexer.ExclamationMarkEquals)]
        [InlineData("<=", CppLexer.LessThanEquals)]
        [InlineData(">=", CppLexer.GreaterThanEquals)]
        [InlineData("&&", CppLexer.DoubleAmpersand)]
        [InlineData("||", CppLexer.DoubleVerticalBar)]
        [InlineData("++", CppLexer.DoublePlus)]
        [InlineData("--", CppLexer.DoubleMinus)]
        [InlineData(",", CppLexer.Comma)]
        [InlineData("->*", CppLexer.ArrowAsteriks)]
        [InlineData("->", CppLexer.Arrow)]
        public void SingleToken_OperatorsAndPunctuation(string value, int expectedToken)
        {
            RunTest(value, expectedToken);
        }

        private void RunTest(string sourceCode, int expectedToken)
        {
            // Parse the file
            var inputStream = new AntlrInputStream(sourceCode);
            var lexer = new CppLexer(inputStream);

            lexer.RemoveErrorListeners();
            lexer.AddErrorListener(new LexerExceptionErrorListener());

            var tokens = lexer.GetAllTokens();

            // Verify we got the single expected token
            Assert.Equal(1, tokens.Count);
            Assert.Equal(expectedToken, tokens[0].Type);
        }
    }
}
