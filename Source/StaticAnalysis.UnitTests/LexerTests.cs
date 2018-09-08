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

        [Theory]
        [InlineData("0")]
        [InlineData("1")]
        [InlineData("42")]
        [InlineData("052")]
        [InlineData("0x2a")]
        [InlineData("0X2A")]
        [InlineData("0b101010")]
        [InlineData("18446744073709550592ull")]
        [InlineData("18'446'744'073'709'550'592llu")]
        [InlineData("1844'6744'0737'0955'0592uLL")]
        [InlineData("184467'440737'0'95505'92LLU")]
        [InlineData("0xDeAdBeEfU")]
        [InlineData("0XdeadBEEFu")]
        public void SingleToken_IntegerLiteral(string value)
        {
            RunTest(value, CppLexer.IntegerLiteral);
        }

        [Theory]
        [InlineData("0.0f")]
        [InlineData("1.0")]
        [InlineData("1'00.1f")]
        [InlineData("0x1.2p3")]
        [InlineData("58.")]
        [InlineData("4e2")]
        [InlineData("123.456e-67")]
        [InlineData(".1E4f")]
        [InlineData("0x10.1p0")]
        public void SingleToken_FloatingPointLiteral(string value)
        {
            RunTest(value, CppLexer.FloatingPointLiteral);
        }

        [Theory]
        [InlineData("'1'")]
        [InlineData("'\\0'")]
        [InlineData("L'A'")]
        [InlineData("u'A'")]
        [InlineData("u8'A'")]
        [InlineData("U'A'")]
        public void SingleToken_CharacterLiteral(string value)
        {
            RunTest(value, CppLexer.CharacterLiteral);
        }

        [Theory]
        [InlineData("\"\"")]
        [InlineData("\" \"")]
        [InlineData("\"Some cool text!\tYay.\"")]
        [InlineData("U\" \"")]
        [InlineData("u\" \"")]
        [InlineData("u8\" \"")]
        [InlineData("L\" \"")]
        public void SingleToken_StringLiteral(string value)
        {
            RunTest(value, CppLexer.StringLiteral);
        }

        [Theory]
        [InlineData("/**/")]
        [InlineData("/* */")]
        [InlineData("/* Hey a comment\n On many lines\n */")]
        public void SingleToken_BlockComment(string value)
        {
            RunTest(value, CppLexer.BlockComment);
        }

        [Theory]
        [InlineData("//")]
        [InlineData("// ")]
        [InlineData("// A longer comment text")]
        public void SingleToken_LineComment(string value)
        {
            RunTest(value, CppLexer.LineComment);
        }

        [Theory]
        [InlineData("a")]
        [InlineData("x")]
        [InlineData("myVariable")]
        [InlineData("ClassName")]
        [InlineData("NameSpace")]
        public void SingleToken_Identifier(string value)
        {
            RunTest(value, CppLexer.Identifier);
        }

        private void RunTest(string sourceCode, int expectedToken)
        {
            // Parse the file
            var inputStream = new AntlrInputStream(sourceCode);
            var lexer = new CppLexer(inputStream);

            // Setup error handling
            lexer.RemoveErrorListeners();
            lexer.AddErrorListener(new LexerExceptionErrorListener());

            // Match all tokens
            var tokens = lexer.GetAllTokens();

            // Verify we got the single expected token
            Assert.Equal(1, tokens.Count);
            Assert.Equal(expectedToken, tokens[0].Type);
        }
    }
}
