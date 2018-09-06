using Antlr4.Runtime;
using Xunit;

namespace Soup.StaticAnalysis.UnitTests
{
    public class LexerOperatorsAndPunctuatorsTests
    {
        [Fact]
        public void SingleToken_LeftBrace()
        {
            RunTest("{", CppLexer.LeftBrace);
        }

        [Fact]
        public void SingleToken_RightBrace()
        {
            RunTest("}", CppLexer.RightBrace);
        }

        [Fact]
        public void SingleToken_LeftBracket()
        {
            RunTest("[", CppLexer.LeftBracket);
        }

        [Fact]
        public void SingleToken_RightBracket()
        {
            RunTest("]", CppLexer.RightBracket);
        }

        [Fact]
        public void SingleToken_LeftParenthesis()
        {
            RunTest("(", CppLexer.LeftParenthesis);
        }

        [Fact]
        public void SingleToken_RightParenthesis()
        {
            RunTest(")", CppLexer.RightParenthesis);
        }

        [Fact]
        public void SingleToken_Semicolon()
        {
            RunTest(";", CppLexer.Semicolon);
        }

        [Fact]
        public void SingleToken_Colon()
        {
            RunTest(":", CppLexer.Colon);
        }

        [Fact]
        public void SingleToken_Ellipsis()
        {
            RunTest("...", CppLexer.Ellipsis);
        }

        [Fact]
        public void SingleToken_New()
        {
            RunTest("new", CppLexer.New);
        }

        [Fact]
        public void SingleToken_Delete()
        {
            RunTest("delete", CppLexer.Delete);
        }

        [Fact]
        public void SingleToken_QuestionMark()
        {
            RunTest("?", CppLexer.QuestionMark);
        }

        [Fact]
        public void SingleToken_DoubleColon()
        {
            RunTest("::", CppLexer.DoubleColon);
        }

        [Fact]
        public void SingleToken_Period()
        {
            RunTest(".", CppLexer.Period);
        }

        [Fact]
        public void SingleToken_PeriodAsterisk()
        {
            RunTest(".*", CppLexer.PeriodAsterisk);
        }

        [Fact]
        public void SingleToken_Plus()
        {
            RunTest("+", CppLexer.Plus);
        }

        [Fact]
        public void SingleToken_Minus()
        {
            RunTest("-", CppLexer.Minus);
        }

        [Fact]
        public void SingleToken_Asterisk()
        {
            RunTest("*", CppLexer.Asterisk);
        }

        [Fact]
        public void SingleToken_ForwardSlash()
        {
            RunTest("/", CppLexer.ForwardSlash);
        }

        [Fact]
        public void SingleToken_Percent()
        {
            RunTest("%", CppLexer.Percent);
        }

        [Fact]
        public void SingleToken_Caret()
        {
            RunTest("^", CppLexer.Caret);
        }

        [Fact]
        public void SingleToken_Ampersand()
        {
            RunTest("&", CppLexer.Ampersand);
        }

        [Fact]
        public void SingleToken_VerticalBar()
        {
            RunTest("|", CppLexer.VerticalBar);
        }

        [Fact]
        public void SingleToken_Tilde()
        {
            RunTest("~", CppLexer.Tilde);
        }

        [Fact]
        public void SingleToken_ExclamationMark()
        {
            RunTest("!", CppLexer.ExclamationMark);
        }

        [Fact]
        public void SingleToken_Equal()
        {
            RunTest("=", CppLexer.Equal);
        }

        [Fact]
        public void SingleToken_LessThan()
        {
            RunTest("<", CppLexer.LessThan);
        }

        [Fact]
        public void SingleToken_GreaterThan()
        {
            RunTest(">", CppLexer.GreaterThan);
        }

        [Fact]
        public void SingleToken_PlusEquals()
        {
            RunTest("+=", CppLexer.PlusEquals);
        }

        [Fact]
        public void SingleToken_MinusEquals()
        {
            RunTest("-=", CppLexer.MinusEquals);
        }

        [Fact]
        public void SingleToken_AsterikEquals()
        {
            RunTest("*=", CppLexer.AsterikEquals);
        }

        [Fact]
        public void SingleToken_ForwardSlashEquals()
        {
            RunTest("/=", CppLexer.ForwardSlashEquals);
        }

        [Fact]
        public void SingleToken_PercentEquals()
        {
            RunTest("%=", CppLexer.PercentEquals);
        }

        [Fact]
        public void SingleToken_CaretEquals()
        {
            RunTest("^=", CppLexer.CaretEquals);
        }

        [Fact]
        public void SingleToken_AmpersandEquals()
        {
            RunTest("&=", CppLexer.AmpersandEquals);
        }

        [Fact]
        public void SingleToken_VerticalBarEquals()
        {
            RunTest("|=", CppLexer.VerticalBarEquals);
        }

        [Fact]
        public void SingleToken_DoubleLessThan()
        {
            RunTest("<<", CppLexer.DoubleLessThan);
        }

        [Fact]
        public void SingleToken_DoubleGreaterThan()
        {
            RunTest(">>", CppLexer.DoubleGreaterThan);
        }

        [Fact]
        public void SingleToken_DoubleLessThanEquals()
        {
            RunTest("<<=", CppLexer.DoubleLessThanEquals);
        }

        [Fact]
        public void SingleToken_DoubleGreaterThanEquals()
        {
            RunTest(">>=", CppLexer.DoubleGreaterThanEquals);
        }

        [Fact]
        public void SingleToken_DoubleEquals()
        {
            RunTest("==", CppLexer.DoubleEquals);
        }

        [Fact]
        public void SingleToken_ExclamationMarkEquals()
        {
            RunTest("!=", CppLexer.ExclamationMarkEquals);
        }

        [Fact]
        public void SingleToken_LessThanEquals()
        {
            RunTest("<=", CppLexer.LessThanEquals);
        }

        [Fact]
        public void SingleToken_GreaterThanEquals()
        {
            RunTest(">=", CppLexer.GreaterThanEquals);
        }

        [Fact]
        public void SingleToken_DoubleAmpersand()
        {
            RunTest("&&", CppLexer.DoubleAmpersand);
        }

        [Fact]
        public void SingleToken_DoubleVerticalBar()
        {
            RunTest("||", CppLexer.DoubleVerticalBar);
        }

        [Fact]
        public void SingleToken_DoublePlus()
        {
            RunTest("++", CppLexer.DoublePlus);
        }

        [Fact]
        public void SingleToken_DoubleMinus()
        {
            RunTest("--", CppLexer.DoubleMinus);
        }

        [Fact]
        public void SingleToken_Comma()
        {
            RunTest(",", CppLexer.Comma);
        }

        [Fact]
        public void SingleToken_ArrowAsteriks()
        {
            RunTest("->*", CppLexer.ArrowAsteriks);
        }

        [Fact]
        public void SingleToken_Arrow()
        {
            RunTest("->", CppLexer.Arrow);
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
