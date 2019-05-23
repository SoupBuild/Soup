using Antlr4.Runtime;
using Antlr4.Runtime.Atn;
using Antlr4.Runtime.Dfa;
using Antlr4.Runtime.Misc;
using Antlr4.Runtime.Sharpen;
using System;
using System.Diagnostics;

namespace Soup.StaticAnalysis
{
    /// <summary>
    /// Exception based error listener
    /// </summary>
    public class ParserExceptionErrorListener : BaseErrorListener
    {
        /// <summary>
        /// Report ambiguity
        /// </summary>
        public override void ReportAmbiguity(
            [NotNull] Parser recognizer,
            [NotNull] DFA dfa,
            int startIndex,
            int stopIndex,
            bool exact,
            [Nullable] BitSet ambigAlts,
            [NotNull] ATNConfigSet configs)
        {
            throw new ParseCanceledException();
        }

        /// <summary>
        /// Report attempting full context
        /// </summary>
        public override void ReportAttemptingFullContext(
            [NotNull] Parser recognizer,
            [NotNull] DFA dfa,
            int startIndex,
            int stopIndex,
            [Nullable] BitSet conflictingAlts,
            [NotNull] SimulatorState conflictState)
        {
            throw new ParseCanceledException();
        }

        /// <summary>
        /// Report context sensitivity
        /// </summary>
        public override void ReportContextSensitivity(
            [NotNull] Parser recognizer, [NotNull] DFA dfa, int startIndex, int stopIndex, int prediction, [NotNull] SimulatorState acceptState)
        {
            Debug.WriteLine("Hmm");
            //throw new ParseCanceledException();
        }

        /// <summary>
        /// Syntax error
        /// </summary>
        public override void SyntaxError(
            [NotNull] IRecognizer recognizer,
            [Nullable] IToken offendingSymbol,
            int line,
            int charPositionInLine,
            [NotNull] string msg,
            [Nullable] RecognitionException e)
        {
            throw new ParseCanceledException("line " + line + ":" + charPositionInLine + " " + msg);
        }
    }
}
