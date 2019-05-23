using Antlr4.Runtime;
using Antlr4.Runtime.Misc;
using System;

namespace Soup.StaticAnalysis
{
    /// <summary>
    /// Exception based error listener
    /// </summary>
    public class LexerExceptionErrorListener : IAntlrErrorListener<int>
    {
        /// <summary>
        /// Syntax error
        /// </summary>
        public void SyntaxError(
            [NotNull] IRecognizer recognizer,
            [Nullable] int offendingSymbol,
            int line,
            int charPositionInLine,
            [NotNull] string msg,
            [Nullable] RecognitionException e)
        {
            throw new ParseCanceledException("line " + line + ":" + charPositionInLine + " " + msg);
        }
    }
}
